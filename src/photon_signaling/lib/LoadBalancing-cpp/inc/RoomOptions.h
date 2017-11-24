/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2017 by Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#pragma once

#include "Photon-cpp/inc/PhotonPeer.h"
#include "Photon-cpp/inc/Enums/EventCode.h"
#include "Photon-cpp/inc/Enums/EventCache.h"
#include "Photon-cpp/inc/Enums/EventKey.h"
#include "Photon-cpp/inc/Enums/OperationCode.h"
#include "Photon-cpp/inc/Enums/ParameterCode.h"
#include "Photon-cpp/inc/Enums/ReceiverGroup.h"
#include "LoadBalancing-cpp/inc/Enums/ErrorCode.h"
#include "LoadBalancing-cpp/inc/Enums/PeerStates.h"
#include "LoadBalancing-cpp/inc/Enums/LobbyType.h"
#include "LoadBalancing-cpp/inc/Enums/MatchmakingMode.h"
#include "LoadBalancing-cpp/inc/AuthenticationValues.h"
#include "LoadBalancing-cpp/inc/LobbyStatsRequest.h"
#include "LoadBalancing-cpp/inc/LobbyStatsResponse.h"

namespace ExitGames
{
	namespace LoadBalancing
	{
		class RoomOptions : public Common::Base
		{
		public:
			RoomOptions(bool isVisible=true, bool isOpen=true, nByte maxPlayers=0, const Common::Hashtable& customRoomProperties=Common::Hashtable(), const Common::JVector<Common::JString>& propsListedInLobby=Common::JVector<Common::JString>(), const Common::JString& lobbyName=Common::JString(), nByte lobbyType=LobbyType::DEFAULT, int playerTtl=0, int emptyRoomTtl=0, bool suppressRoomEvents=false, const Common::JVector<Common::JString>* pPlugins=NULL, bool publishUserID=false);
			~RoomOptions(void);

			RoomOptions(const RoomOptions& toCopy);
			RoomOptions& operator=(const RoomOptions& toCopy);

			bool getIsVisible(void) const;
			RoomOptions& setIsVisible(bool isVisible);
			bool getIsOpen(void) const;
			RoomOptions& setIsOpen(bool isOpen);
			nByte getMaxPlayers(void) const;
			RoomOptions& setMaxPlayers(nByte maxPlayers);
			const Common::Hashtable& getCustomRoomProperties(void) const;
			RoomOptions& setCustomRoomProperties(const Common::Hashtable& customRoomProperties);
			const Common::JVector<Common::JString>& getPropsListedInLobby(void) const;
			RoomOptions& setPropsListedInLobby(const Common::JVector<Common::JString>& propsListedInLobby);
			const Common::JString& getLobbyName(void) const;
			RoomOptions& setLobbyName(const Common::JString& lobbyName);
			nByte getLobbyType(void) const;
			RoomOptions& setLobbyType(nByte lobbyType);
			int getPlayerTtl(void) const;
			RoomOptions& setPlayerTtl(int playerTtl);
			int getEmptyRoomTtl(void) const;
			RoomOptions& setEmptyRoomTtl(int emptyRoomTtl);
			bool getSuppressRoomEvents(void) const;
			RoomOptions& setSuppressRoomEvents(bool suppressRoomEvents);
			const Common::JVector<Common::JString>* getPlugins(void) const;
			RoomOptions& setPlugins(const Common::JVector<Common::JString>* pPlugins);
			bool getPublishUserID(void) const;
			RoomOptions& setPublishUserID(bool publishUserID);

			virtual Common::JString& toString(Common::JString& retStr, bool withTypes=false) const;
		private:
			bool mIsVisible;
			bool mIsOpen;
			nByte mMaxPlayers;
			Common::Hashtable mCustomRoomProperties;
			Common::JVector<Common::JString> mPropsListedInLobby;
			Common::JString mLobbyName;
			nByte mLobbyType;
			int mPlayerTtl;
			int mEmptyRoomTtl;
			bool mSuppressRoomEvents;
			Common::Helpers::SharedPointer<Common::JVector<Common::JString> > mpPlugins;
			bool mPublishUserID;
		};
	}
}