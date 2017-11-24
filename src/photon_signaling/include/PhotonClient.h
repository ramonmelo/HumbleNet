#pragma once

#include <stdio.h>
#include "Logger.h"
#include "LoadBalancing-cpp/inc/Client.h"
#include "humblenet_p2p_signaling_provider.h"

enum State
{
    INITIALIZED = 0,
    CONNECTING,
    CONNECTED,
    JOINING,
    JOINED,
    SENT_DATA,
    RECEIVED_DATA,
    LEAVING,
    LEFT,
    DISCONNECTING,
    DISCONNECTED
};

class PhotonClient : public ExitGames::LoadBalancing::Listener {

public:
    const ExitGames::Common::JString gameName = L"Basics";
    const int MAX_SENDCOUNT = 100;

    PhotonClient(const ExitGames::Common::JString& appID, const ExitGames::Common::JString& appVersion);
    void service(void);

    bool isConnected();

    void sendData(void);

private:
    Logger logger;

    ExitGames::LoadBalancing::Client mClient;
    State mState;
    State mLastState;
    int64 mSendCount;
    int64 mReceiveCount;

    // receive and print out debug out here
    virtual void debugReturn(int debugLevel, const ExitGames::Common::JString& string);

    // implement your error-handling here
    virtual void connectionErrorReturn(int errorCode);
    virtual void clientErrorReturn(int errorCode);
    virtual void warningReturn(int warningCode);
    virtual void serverErrorReturn(int errorCode);

    // events, triggered by certain operations of all players in the same room
    virtual void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player);
    virtual void leaveRoomEventAction(int playerNr, bool isInactive);
    virtual void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent);

    // callbacks for operations on PhotonLoadBalancing server
    virtual void connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& cluster);
    virtual void disconnectReturn(void);
    virtual void createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
    virtual void joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
    virtual void joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
    virtual void joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
    virtual void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString);
    virtual void joinLobbyReturn(void);
    virtual void leaveLobbyReturn(void);

    // Utils

    ExitGames::Common::JString getStateString(void);
};

class PhotonSignalingProvider : public ISignalingProvider {

private:
    PhotonClient mClient;

public:
    PhotonSignalingProvider(const ExitGames::Common::JString& appID, const ExitGames::Common::JString& appVersion);

    void service();

    // State
    ha_bool is_connected();

    // Commands
    ha_bool connect(void* info);
    void disconnect();

    int send(const uint8_t* buff, size_t length);
    int receive(const uint8_t* buff, size_t length, void* info);

};
