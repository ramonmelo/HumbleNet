/*
* @Author: ramonmelo
* @Date:   2017-11-09
* @Last Modified by:   Ramon Melo
* @Last Modified time: 2017-12-15
*/

#include "PhotonClient.h"

using namespace Photon;

// Signaling Provider

PhotonSignalingProvider::PhotonSignalingProvider(const ExitGames::Common::JString& appID, const ExitGames::Common::JString& appVersion) :
    mClient(*this, appID, appVersion),
    mState(INITIALIZED),
    mCurrentDebugLevel(DebugLevel::INFO),
    running(false)
{
    mSendCount = 0;
    mReceiveCount = 0;
}

int PhotonSignalingProvider::getId(void) {
    return mClient.getLocalPlayer().getNumber();
}


void PhotonSignalingProvider::service(void) {

    switch(mState)
    {
        case State::INITIALIZED:

            if(mClient.connect()) {
                mState = State::CONNECTING;
            } else {
                mState = State::DISCONNECTED;
            }

            break;
        case State::CONNECTED:

            mClient.opJoinOrCreateRoom(gameName);
            mState = State::JOINING;

            break;
        case State::JOINED:

            // sendData();

            break;
        case State::RECEIVED_DATA:

            // mClient.opLeaveRoom();
            // mState = State::LEAVING;
            // logger.log(getStateString());

            break;
        case State::LEFT:

            mClient.disconnect();
            mState = State::DISCONNECTING;

            break;
        case State::DISCONNECTED:

            // mState = State::INITIALIZED;

            break;
        default:
            break;
    }

    // debugReturn(DebugLevel::INFO, getStateString());

    mClient.service();
}

/**
 * ISignalingProvider
 */

ha_bool PhotonSignalingProvider::is_connected() {
    return mState == State::JOINED;
}

// Commands
ha_bool PhotonSignalingProvider::connect(void* info) {
    debugReturn(DebugLevel::WARNINGS, ExitGames::Common::JString(L"call connect"));

    while( !is_connected() ) {
        service();
    }

    humblenet_p2p_set_my_peer_id( getId() );

    // startLoop();

    return is_connected();
}

void PhotonSignalingProvider::disconnect() {
    debugReturn(DebugLevel::WARNINGS, ExitGames::Common::JString(L"call disconnect"));

    // stopLoop();
}

int PhotonSignalingProvider::send(const uint8_t* buff, size_t length)
{
    if (buff != NULL) {

        ExitGames::Common::Hashtable event;

        // put data
        event.put(static_cast<nByte>(0), buff, length);
        event.put(static_cast<nByte>(1), (int) length);

        // send to ourselves only
        int myPlayerNumber = getId();

        nByte totalPlayers = mClient.getCurrentlyJoinedRoom().getPlayerCount();
        nByte totalTargets = totalPlayers - 1;

        debugReturn(DebugLevel::INFO, ExitGames::Common::JString(L"My id: ") + myPlayerNumber);
        debugReturn(DebugLevel::INFO, ExitGames::Common::JString(L"Total players in room: ") + totalPlayers);

        int *targets = new int[totalTargets];
        nByte currentIndex = 0;

        for (int i = 0; i < totalPlayers; i++)
        {
            int currentPlayer = mClient.getCurrentlyJoinedRoom().getPlayers()[i]->getNumber();

            if (myPlayerNumber == currentPlayer) {
                continue;
            }

            targets[currentIndex] = currentPlayer;
            currentIndex += 1;

            debugReturn(DebugLevel::INFO, ExitGames::Common::JString(L"\tsending msg to: ") + currentPlayer);
        }

        ExitGames::LoadBalancing::RaiseEventOptions options = ExitGames::LoadBalancing::RaiseEventOptions();

        options.setTargetPlayers( targets );
        options.setNumTargetPlayers( totalTargets );

        mClient.opRaiseEvent(
            true,
            event,
            EventType::DATA,
            options);

        delete[] targets;

        return length;

    } else {
        return 0;
    }
}

/**
 * LoadBalancing::Client
 */

int PhotonSignalingProvider::receive(const uint8_t* buff, size_t length, void* info) {
    debugReturn(DebugLevel::INFO, ExitGames::Common::JString(L"to receive data"));

    return 0;
}

// receive and print out debug out here
void PhotonSignalingProvider::debugReturn(int debugLevel, const ExitGames::Common::JString& string) {

    if ( debugLevel <= mCurrentDebugLevel ) {
        std::wcout << "\nLOG: " << string.cstr() << std::endl;
    }
}

// implement your error-handling here
void PhotonSignalingProvider::connectionErrorReturn(int errorCode) {
    debugReturn(DebugLevel::ERRORS, ExitGames::Common::JString(L"received connection error ") + errorCode);

    mState = State::DISCONNECTED;
}

void PhotonSignalingProvider::clientErrorReturn(int errorCode) {
    debugReturn(DebugLevel::ERRORS, ExitGames::Common::JString(L"received error ") + errorCode + L" from client");
}

void PhotonSignalingProvider::warningReturn(int warningCode) {
    debugReturn(DebugLevel::WARNINGS, ExitGames::Common::JString(L"received warning ") + warningCode + L" from client");
}

void PhotonSignalingProvider::serverErrorReturn(int errorCode) {
    debugReturn(DebugLevel::ERRORS, ExitGames::Common::JString(L"received error ") + errorCode + L" from server");
}

// events, triggered by certain operations of all players in the same room
void PhotonSignalingProvider::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) {

    debugReturn(DebugLevel::INFO, ExitGames::Common::JString(L"player ") + playerNr + L" " + player.getName() + L" has joined the game");
}

void PhotonSignalingProvider::leaveRoomEventAction(int playerNr, bool isInactive) {
    debugReturn(DebugLevel::INFO, ExitGames::Common::JString(L"player ") + playerNr + L" has left the game");
}

void PhotonSignalingProvider::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj) {

    if (playerNr == getId()) {
        return;
    }

    debugReturn(DebugLevel::WARNINGS, ExitGames::Common::JString(L"received data from ") + playerNr);

    ExitGames::Common::Hashtable event = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContentObj).getDataCopy();

    switch(eventCode)
    {
    case EventType::DATA:

        if(event.contains((nByte)0) && event.contains((nByte)1)) {

            uint8_t* buff = ExitGames::Common::ValueObject<uint8_t*>(event.getValue((nByte)0)).getDataCopy();
            int length = ExitGames::Common::ValueObject<int>(event.getValue((nByte)1)).getDataCopy();

            debugReturn(DebugLevel::WARNINGS, ExitGames::Common::JString(L"size: ") + length);

            this->recvBuf.insert(this->recvBuf.end()
                             , reinterpret_cast<const char *>(buff)
                             , reinterpret_cast<const char *>(buff) + length);

            humblenet::parseMessage(this->recvBuf, p2pSignalProcess, NULL);
        }

        break;
    case EventType::INFO:

        debugReturn(DebugLevel::WARNINGS, ExitGames::Common::JString(L"received info from ") + playerNr);

        break;
    default:
        break;
    }
}

// callbacks for operations on PhotonLoadBalancing server
void PhotonSignalingProvider::connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& cluster) {

    if(errorCode)
    {
        debugReturn(DebugLevel::ERRORS, ExitGames::Common::JString(L"Not connected to cluster: ") + errorString);
        mState = State::DISCONNECTING;

        return;
    }

    debugReturn(DebugLevel::INFO, ExitGames::Common::JString(L"Connected to cluster: ") + cluster);
    mState = State::CONNECTED;
}

void PhotonSignalingProvider::disconnectReturn(void) {
    debugReturn(DebugLevel::INFO, ExitGames::Common::JString(L"Disconnected"));

    mState = State::DISCONNECTED;
}

void PhotonSignalingProvider::createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) {

    if(errorCode)
    {
        debugReturn(DebugLevel::ERRORS, ExitGames::Common::JString(L"opCreateRoom() failed: ") + errorString);
        mState = State::CONNECTED;

        return;
    }

    debugReturn(DebugLevel::INFO, ExitGames::Common::JString(L"Room created: ") + mClient.getCurrentlyJoinedRoom().getName());

    mState = State::JOINED;
}

void PhotonSignalingProvider::joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) {

    if(errorCode)
    {
        debugReturn(DebugLevel::ERRORS, ExitGames::Common::JString(L"opJoinOrCreateRoom() failed: ") + errorString);
        mState = State::CONNECTED;

        return;
    }

    debugReturn(DebugLevel::INFO, ExitGames::Common::JString(L"Room joined: ") + mClient.getCurrentlyJoinedRoom().getName());

    mState = State::JOINED;
}

void PhotonSignalingProvider::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) {

    if(errorCode)
    {
        debugReturn(DebugLevel::ERRORS, ExitGames::Common::JString(L"opJoinRoom() failed: ") + errorString);
        mState = State::CONNECTED;
        return;
    }

    debugReturn(DebugLevel::INFO, ExitGames::Common::JString(L"Room joined: ") + mClient.getCurrentlyJoinedRoom().getName());

    mState = State::JOINED;
}

void PhotonSignalingProvider::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) {

    if(errorCode)
    {
        debugReturn(DebugLevel::ERRORS, ExitGames::Common::JString(L"opJoinRandomRoom() failed: ") + errorString);
        mState = State::CONNECTED;
        return;
    }

    debugReturn(DebugLevel::INFO, ExitGames::Common::JString(L"Room joined: ") + mClient.getCurrentlyJoinedRoom().getName());

    mState = State::JOINED;
}

void PhotonSignalingProvider::leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString) {

    if(errorCode)
    {
        debugReturn(DebugLevel::ERRORS, ExitGames::Common::JString(L"opLeaveRoom() failed: ") + errorString);
        mState = State::DISCONNECTING;
        return;
    }

    mState = State::LEFT;

    debugReturn(DebugLevel::INFO, ExitGames::Common::JString(L"room has been successfully left"));
}

void PhotonSignalingProvider::joinLobbyReturn(void) {
    debugReturn(DebugLevel::INFO, ExitGames::Common::JString(L"joined lobby"));
}

void PhotonSignalingProvider::leaveLobbyReturn(void) {
    debugReturn(DebugLevel::INFO, ExitGames::Common::JString(L"left lobby"));
}

// Utils

void PhotonSignalingProvider::startLoop(void) {

    debugReturn(DebugLevel::WARNINGS, ExitGames::Common::JString(L"startLoop"));

    running = true;

    internalLoop = std::thread( [this] {
        while(this->running) {
            this->service();

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
}

void PhotonSignalingProvider::stopLoop(void) {
    debugReturn(DebugLevel::WARNINGS, ExitGames::Common::JString(L"stopLoop"));

    running = false;
    internalLoop.join();
}

ExitGames::Common::JString PhotonSignalingProvider::getStateString(void)
{
    switch(mState)
    {
        case State::INITIALIZED:
            return L"disconnected";
        case State::CONNECTING:
            return L"connecting";
        case State::CONNECTED:
            return L"connected";
        case State::JOINING:
            return L"joining";
        case State::JOINED:
            return ExitGames::Common::JString(L"ingame\nsent event Nr. ") + mSendCount + L"\nreceived event Nr. " + mReceiveCount;
        case State::SENT_DATA:
            return ExitGames::Common::JString(L"sending completed") + L"\nreceived event Nr. " + mReceiveCount;
        case State::RECEIVED_DATA:
            return L"receiving completed";
        case State::LEAVING:
            return L"leaving";
        case State::LEFT:
            return L"left";
        case State::DISCONNECTING:
            return L"disconnecting";
        case State::DISCONNECTED:
            return L"disconnected";
        default:
            return L"unknown state";
    }
}
