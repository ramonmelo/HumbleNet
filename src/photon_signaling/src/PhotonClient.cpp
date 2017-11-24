/*
* @Author: ramonmelo
* @Date:   2017-11-09
* @Last Modified by:   Ramon Melo
* @Last Modified time: 2017-11-24
*/

#include "PhotonClient.h"

// Signaling Provider

PhotonSignalingProvider::PhotonSignalingProvider(const ExitGames::Common::JString& appID, const ExitGames::Common::JString& appVersion) :
    mClient(appID, appVersion)
{
}

void PhotonSignalingProvider::service() {
    mClient.service();
}

ha_bool PhotonSignalingProvider::is_connected() {

    return false;
}

// Commands
ha_bool PhotonSignalingProvider::connect(void* info) {

    return false;
}

void PhotonSignalingProvider::disconnect() {

}

int PhotonSignalingProvider::send(const uint8_t* buff, size_t length) {

    return 0;
}

int PhotonSignalingProvider::receive(const uint8_t* buff, size_t length, void* info) {

    return 0;
}

// Photon Client Impl

PhotonClient::PhotonClient(const ExitGames::Common::JString& appID, const ExitGames::Common::JString& appVersion) :
    mClient(*this, appID, appVersion),
    mState(INITIALIZED),
    mLastState(INITIALIZED)
{
    mSendCount = 0;
    mReceiveCount = 0;
}

// API

bool PhotonClient::isConnected() {
    return mState == State::JOINED;
}

void PhotonClient::sendData(void)
{
    ExitGames::Common::Hashtable event;
    event.put(static_cast<nByte>(0), ++mSendCount);
    // send to ourselves only
    int myPlayerNumber = mClient.getLocalPlayer().getNumber();

    mClient.opRaiseEvent(true, event, 0, ExitGames::LoadBalancing::RaiseEventOptions().setTargetPlayers(&myPlayerNumber).setNumTargetPlayers(1));

    if(mSendCount >= MAX_SENDCOUNT) {
        mState = State::SENT_DATA;
    }
}

// Low API

void PhotonClient::service(void) {

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
            sendData();
            break;
        case State::RECEIVED_DATA:
            mClient.opLeaveRoom();
            mState = State::LEAVING;

            logger.log(getStateString());

            break;
        case State::LEFT:
            mClient.disconnect();
            mState = State::DISCONNECTING;
            break;
        case State::DISCONNECTED:
            mState = State::INITIALIZED;
            break;
        default:
            break;
    }

    // if (mState != mLastState) {
        logger.log(getStateString());
        // mLastState = mState;
    // }

    mClient.service();
}

// receive and print out debug out here
void PhotonClient::debugReturn(int debugLevel, const ExitGames::Common::JString& string) {
    logger.log(string);
}

// implement your error-handling here
void PhotonClient::connectionErrorReturn(int errorCode) {
    logger.log(ExitGames::Common::JString(L"received connection error ") + errorCode);
    mState = State::DISCONNECTED;
}

void PhotonClient::clientErrorReturn(int errorCode) {
    logger.log(ExitGames::Common::JString(L"received error ") + errorCode + L" from client");
}

void PhotonClient::warningReturn(int warningCode) {
    logger.log(ExitGames::Common::JString(L"received warning ") + warningCode + L" from client");
}

void PhotonClient::serverErrorReturn(int errorCode) {
    logger.log(ExitGames::Common::JString(L"received error ") + errorCode + L" from server");
}

// events, triggered by certain operations of all players in the same room
void PhotonClient::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) {
    logger.log(L"");
    logger.log(ExitGames::Common::JString(L"player ") + playerNr + L" " + player.getName() + L" has joined the game");
}

void PhotonClient::leaveRoomEventAction(int playerNr, bool isInactive) {
    logger.log(L"");
    logger.log(ExitGames::Common::JString(L"player ") + playerNr + L" has left the game");
}

void PhotonClient::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj) {

    ExitGames::Common::Hashtable event = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContentObj).getDataCopy();

    switch(eventCode)
    {
    case 0:
        if(event.getValue((nByte)0))
            mReceiveCount = ((ExitGames::Common::ValueObject<int64>*)(event.getValue((nByte)0)))->getDataCopy();
        if(mState == State::SENT_DATA && mReceiveCount >= mSendCount)
        {
            mState = State::RECEIVED_DATA;
            mSendCount = 0;
            mReceiveCount = 0;
        }
        break;
    default:
        break;
    }
}

// callbacks for operations on PhotonLoadBalancing server
void PhotonClient::connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& cluster) {

    if(errorCode)
    {
        logger.log(L"connected to cluster " + errorString);
        mState = State::DISCONNECTING;

        return;
    }

    logger.log(L"connected to cluster " + cluster);
    mState = State::CONNECTED;
}

void PhotonClient::disconnectReturn(void) {

    logger.log(L"disconnected");
    mState = State::DISCONNECTED;

}

void PhotonClient::createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) {

    if(errorCode)
    {
        logger.log(L"opCreateRoom() failed: " + errorString);
        mState = State::CONNECTED;

        return;
    }

    logger.log(L"... room " + mClient.getCurrentlyJoinedRoom().getName() + " has been created");
    logger.log(L"regularly sending dummy events now");

    mState = State::JOINED;
}

void PhotonClient::joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) {

    if(errorCode)
    {
        logger.log(L"opJoinOrCreateRoom() failed: " + errorString);
        mState = State::CONNECTED;

        return;
    }

    logger.log(L"... room " + mClient.getCurrentlyJoinedRoom().getName() + " has been entered");
    logger.log(L"regularly sending dummy events now");

    mState = State::JOINED;
}

void PhotonClient::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) {

    if(errorCode)
    {
        logger.log(L"opJoinRoom() failed: " + errorString);
        mState = State::CONNECTED;
        return;
    }
    logger.log(L"... room " + mClient.getCurrentlyJoinedRoom().getName() + " has been successfully joined");
    logger.log(L"regularly sending dummy events now");

    mState = State::JOINED;
}

void PhotonClient::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) {

    if(errorCode)
    {
        logger.log(L"opJoinRandomRoom() failed: " + errorString);
        mState = State::CONNECTED;
        return;
    }

    logger.log(L"... room " + mClient.getCurrentlyJoinedRoom().getName() + " has been successfully joined");
    logger.log(L"regularly sending dummy events now");
    mState = State::JOINED;

}

void PhotonClient::leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString) {

    if(errorCode)
    {
        logger.log(L"opLeaveRoom() failed: " + errorString);
        mState = State::DISCONNECTING;
        return;
    }
    mState = State::LEFT;
    logger.log(L"room has been successfully left");
}

void PhotonClient::joinLobbyReturn(void) {
    logger.log(L"joined lobby");
}

void PhotonClient::leaveLobbyReturn(void) {
    logger.log(L"left lobby");
}

// Utils

ExitGames::Common::JString PhotonClient::getStateString(void)
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
