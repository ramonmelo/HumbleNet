#pragma once

#ifndef EG_LOGGING
#define EG_LOGGING true
#endif

#include <iostream>
#include <stdio.h>
#include <thread>
#include <chrono>

#include "LoadBalancing-cpp/inc/Client.h"
#include "Common-cpp/inc/Logger.h"
#include "Common-cpp/inc/Enums/DebugLevel.h"

#include "humblenet_p2p.h"
#include "humblenet_p2p_signaling_provider.h"
#include "humblenet_p2p_internal_signaling_provider.h"

using namespace ExitGames::Common;
using namespace ExitGames::LoadBalancing;

namespace Photon {

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

    enum EventType {
        DATA = 0,
        INFO = 1
    };

    class PhotonSignalingProvider : public ExitGames::LoadBalancing::Listener, public humblenet::HumblenetSignalProvider {

    public:
        const ExitGames::Common::JString gameName = L"Basics";
        const int MAX_SENDCOUNT = 100;

        PhotonSignalingProvider(
            const ExitGames::Common::JString& appID,
            const ExitGames::Common::JString& appVersion);

        // LoadBalancing::Client

        void service(void);
        int getId(void);

        // ISignalingProvider

        // :: State
        ha_bool is_connected();

        // :: Commands
        ha_bool connect(void* info);
        void disconnect();

        int send(const uint8_t* buff, size_t length);
        int receive(const uint8_t* buff, size_t length, void* info);

    private:
        int mCurrentDebugLevel;

        Client mClient;
        State mState;
        int64 mSendCount;
        int64 mReceiveCount;

        std::thread internalLoop;
        volatile bool running;

        void startLoop(void);
        void stopLoop(void);

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

}
