
#include "humblenet_p2p_internal.h"
#include "humblenet_alias.h"
#include "humblenet_p2p_internal_signaling_provider.h"

namespace humblenet {

    ha_bool HumblenetSignalProvider::is_connected() {
        return wsi != NULL;
    }

    ha_bool HumblenetSignalProvider::connect(void* info) {
        disconnect();

        std::string* serverAddr = reinterpret_cast<std::string*>(info);

        wsi = internal_connect_websocket((*serverAddr).c_str(), "humblepeer");

        return is_connected();
    }

    void HumblenetSignalProvider::disconnect() {
        if( wsi ) {
            internal_close_socket(wsi);
        }
        wsi = NULL;
    }

    int HumblenetSignalProvider::send(const uint8_t* buff, size_t length) {
        if (buff == NULL) {

            if (sendBuf.empty()) {
                // no data in sendBuf
                return 0;
            }

            int retval = internal_write_socket(wsi, &sendBuf[0], sendBuf.size() );
            if (retval < 0) {
                // error while sending, close the connection
                // TODO: should try to reopen after some time
                // humbleNetState.p2pConn.reset();
                return -1;
            }

            // successful write
            sendBuf.erase(sendBuf.begin(), sendBuf.begin() + retval);

            return 0;

        } else {
            return internal_write_socket(wsi, (const void*)buff, length );
        }
    }

    int HumblenetSignalProvider::receive(const uint8_t* buff, size_t length, void* info) {
        this->recvBuf.insert(this->recvBuf.end()
                             , reinterpret_cast<const char *>(buff)
                             , reinterpret_cast<const char *>(buff) + length);

        return parseMessage(this->recvBuf, p2pSignalProcess, NULL);
    }

    ha_bool HumblenetSignalProvider::p2pSignalProcess(const humblenet::HumblePeer::Message *msg, void *user_data) {

        using namespace humblenet;

        auto msgType = msg->message_type();

        switch (msgType) {
            case HumblePeer::MessageType::P2POffer:
            {
                auto p2p = reinterpret_cast<const HumblePeer::P2POffer*>(msg->message());
                PeerId peer = static_cast<PeerId>(p2p->peerId());

                if (humbleNetState.pendingPeerConnectionsIn.find(peer) != humbleNetState.pendingPeerConnectionsIn.end()) {
                    // error, already a pending incoming connection to this peer
                    LOG("p2pSignalProcess: already a pending connection to peer %u\n", peer);
                    return true;
                }

                bool emulated = ( p2p->flags() & 0x1 );

                if (emulated || !humbleNetState.webRTCSupported) {
                    // webrtc connections not supported
                    assert(humbleNetState.p2pConn);
                    sendPeerRefused(humbleNetState.p2pConn.get(), peer);
                }

                Connection *connection = new Connection(Incoming);
                connection->status = HUMBLENET_CONNECTION_CONNECTING;
                connection->otherPeer = peer;
                {
                    if (!humbleNetState.usingExternalSignaling) { HUMBLENET_UNGUARD(); }

                    connection->socket = internal_create_webrtc(humbleNetState.context);
                }
                internal_set_data( connection->socket, connection );

                humbleNetState.pendingPeerConnectionsIn.insert(std::make_pair(peer, connection));

                auto offer = p2p->offer();

                LOG("P2PConnect SDP got %u's offer = \"%s\"\n", peer, offer->c_str());
                int ret;
                {
                    if (!humbleNetState.usingExternalSignaling) { HUMBLENET_UNGUARD(); }

                    ret = internal_set_offer( connection->socket, offer->c_str() );
                }
                if( ! ret )
                {
                    humblenet_connection_close( connection );

                    sendPeerRefused(humbleNetState.p2pConn.get(), peer);
                }
            }
                break;

            case HumblePeer::MessageType::P2PAnswer:
            {
                auto p2p = reinterpret_cast<const HumblePeer::P2PAnswer*>(msg->message());
                PeerId peer = static_cast<PeerId>(p2p->peerId());

                auto it = humbleNetState.pendingPeerConnectionsOut.find(peer);
                if (it == humbleNetState.pendingPeerConnectionsOut.end()) {
                    LOG("P2PResponse for connection we didn't initiate: %u\n", peer);
                    return true;
                }

                Connection *conn = it->second;
                assert(conn != NULL);
                assert(conn->otherPeer == peer);

                assert(conn->socket != NULL);
                // TODO: deal with _CLOSED
                assert(conn->status == HUMBLENET_CONNECTION_CONNECTING);

                auto offer = p2p->offer();

                LOG("P2PResponse SDP got %u's response offer = \"%s\"\n", peer, offer->c_str());
                int ret;
                {
                    if (!humbleNetState.usingExternalSignaling) { HUMBLENET_UNGUARD(); }

                    ret = internal_set_answer( conn->socket, offer->c_str() );
                }
                if( !ret ) {
                    humblenet_connection_set_closed( conn );
                }
            }
                break;

            case HumblePeer::MessageType::HelloClient:
            {
                auto hello = reinterpret_cast<const HumblePeer::HelloClient*>(msg->message());
                PeerId peer = static_cast<PeerId>(hello->peerId());

                if (humbleNetState.myPeerId != 0) {
                    LOG("Error: got HelloClient but we already have a peer id\n");
                    return true;
                }
                LOG("My peer id is %u\n", peer);
                humbleNetState.myPeerId = peer;

                humbleNetState.iceServers.clear();

                if (hello->iceServers()) {
                    auto iceList = hello->iceServers();
                    for (const auto& it : *iceList) {
                        if (it->type() == HumblePeer::ICEServerType::STUNServer) {
                            humbleNetState.iceServers.emplace_back(it->server()->str());
                        } else if (it->type() == HumblePeer::ICEServerType::TURNServer) {
                            auto username = it->username();
                            auto pass = it->password();
                            if (pass && username) {
                                humbleNetState.iceServers.emplace_back(it->server()->str(), username->str(), pass->str());
                            }
                        }
                    }
                } else {
                    LOG("No STUN/TURN credentials provided by the server\n");
                }

                std::vector<const char*> stunServers;
                for (auto& it : humbleNetState.iceServers) {
                    if (it.type == HumblePeer::ICEServerType::STUNServer) {
                        stunServers.emplace_back(it.server.c_str());
                    }
                }
                internal_set_stun_servers(humbleNetState.context, stunServers.data(), stunServers.size());
                //            internal_set_turn_server( server.c_str(), username.c_str(), password.c_str() );
            }
                break;

            case HumblePeer::MessageType::ICECandidate:
            {
                auto iceCandidate = reinterpret_cast<const HumblePeer::ICECandidate*>(msg->message());
                PeerId peer = static_cast<PeerId>(iceCandidate->peerId());

                auto it = humbleNetState.pendingPeerConnectionsIn.find( peer );

                if( it == humbleNetState.pendingPeerConnectionsIn.end() )
                {
                    it = humbleNetState.pendingPeerConnectionsOut.find( peer );
                    if( it == humbleNetState.pendingPeerConnectionsOut.end() )
                    {
                        // no connection waiting on a peer.
                        return true;
                    }
                }

                if( it->second->socket && it->second->status == HUMBLENET_CONNECTION_CONNECTING ) {
                    auto offer = iceCandidate->offer();
                    LOG("Got ice candidate from peer: %d, %s\n", it->second->otherPeer, offer->c_str() );

                    {
                        if (!humbleNetState.usingExternalSignaling) { HUMBLENET_UNGUARD(); }

                        internal_add_ice_candidate( it->second->socket, offer->c_str() );
                    }
                }
            }
                break;

            case HumblePeer::MessageType::P2PReject:
            {
                auto reject = reinterpret_cast<const HumblePeer::P2PReject*>(msg->message());
                PeerId peer = static_cast<PeerId>(reject->peerId());

                auto it = humbleNetState.pendingPeerConnectionsOut.find(peer);
                if (it == humbleNetState.pendingPeerConnectionsOut.end()) {
                    switch (reject->reason()) {
                        case HumblePeer::P2PRejectReason::NotFound:
                            LOG("Peer %u does not exist\n", peer);
                            break;
                        case HumblePeer::P2PRejectReason::PeerRefused:
                            LOG("Peer %u rejected our connection\n", peer);
                            break;
                    }
                    return true;
                }

                Connection *conn = it->second;
                assert(conn != NULL);

                blacklist_peer(peer);
                humblenet_connection_set_closed(conn);
            }
                break;

            case HumblePeer::MessageType::P2PConnected:
            {
                auto connect = reinterpret_cast<const HumblePeer::P2PConnected*>(msg->message());

                LOG("Established connection to peer %u", connect->peerId());
            }
                break;

            case HumblePeer::MessageType::P2PDisconnect:
            {
                auto disconnect = reinterpret_cast<const HumblePeer::P2PDisconnect*>(msg->message());

                LOG("Disconnecting peer %u", disconnect->peerId());
            }
                break;

            case HumblePeer::MessageType::P2PRelayData:
            {
                auto relay = reinterpret_cast<const HumblePeer::P2PRelayData*>(msg->message());
                auto peer = relay->peerId();
                auto data = relay->data();

                LOG("Got %d bytes relayed from peer %u\n", data->Length(), peer );

                // Sequentially look for the other peer
                auto it = humbleNetState.connections.begin();
                for( ; it != humbleNetState.connections.end(); ++it ) {
                    if( it->second->otherPeer == peer )
                        break;
                }
                if( it == humbleNetState.connections.end() ) {
                    LOG("Peer %u does not exist\n", peer);
                } else {
                    Connection* conn = it->second;

                    conn->recvBuffer.insert(conn->recvBuffer.end()
                                            , reinterpret_cast<const char *>(data->Data())
                                            , reinterpret_cast<const char *>(data->Data()) + data->Length());
                    humbleNetState.pendingDataConnections.insert( conn );

                    signal();
                }
            }
                break;
            case HumblePeer::MessageType::AliasResolved:
            {
                auto resolved = reinterpret_cast<const HumblePeer::AliasResolved*>(msg->message());

                internal_alias_resolved_to( resolved->alias()->c_str(), resolved->peerId() );
            }
                break;

            default:
                LOG("p2pSignalProcess unhandled %s\n", HumblePeer::EnumNameMessageType(msgType));
                break;
        }

        return true;
    }
}
