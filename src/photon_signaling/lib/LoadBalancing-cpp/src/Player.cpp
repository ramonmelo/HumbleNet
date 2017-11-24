/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2017 by Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#include "LoadBalancing-cpp/inc/Player.h"
#include "LoadBalancing-cpp/inc/MutableRoom.h"
#include "LoadBalancing-cpp/inc/Internal/Utils.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/Properties/Player.h"

/** @file inc/Player.h */

namespace ExitGames
{
	namespace LoadBalancing
	{
		using namespace Common;
		using namespace Internal;

		Player::Player(void)
			: mNumber(-1)
			, mpRoom(NULL)
			, mIsInactive(false)
		{
		}

		Player::Player(int number, const Hashtable& properties, const MutableRoom* const pRoom)
			: mNumber(number)
			, mpRoom(pRoom)
			, mIsInactive(false)
		{
			cacheProperties(properties);
		}

		Player::~Player(void)
		{
		}

		Player::Player(const Player& toCopy)
		{
			*this = toCopy;
		}

		Player& Player::operator=(const Player& toCopy)
		{
			return assign(toCopy);
		}

		bool Player::getIsMutable(void) const
		{
			return false;
		}

		Player& Player::assign(const Player& toCopy)
		{
			mNumber = toCopy.mNumber;
			mName = toCopy.mName;
			mUserID = toCopy.mUserID;
			mCustomProperties = toCopy.mCustomProperties;
			mpRoom = toCopy.mpRoom;
			return *this;
		}

		int Player::getNumber(void) const
		{
			return mNumber;
		}

		const JString& Player::getName(void) const
		{
			return mName;
		}

		const JString& Player::getUserID(void) const
		{
			return mUserID;
		}

		const Hashtable& Player::getCustomProperties(void) const
		{
			return mCustomProperties;
		}

		void Player::setMutableRoomPointer(const MutableRoom* pRoom)
		{
			mpRoom = pRoom;
		}

		bool Player::getIsInactive(void) const
		{
			return mIsInactive;
		}

		void Player::setIsInactive(bool isInActive)
		{
			mIsInactive = isInActive;
		}

		bool Player::getIsMasterClient(void) const
		{
			return mpRoom?mNumber==mpRoom->getMasterClientID():false;
		}

		void Player::cacheProperties(const Hashtable& properties)
		{
			if(properties.contains(static_cast<nByte>(Properties::Player::PLAYERNAME)))
				mName = ValueObject<JString>(properties.getValue(static_cast<nByte>(Properties::Player::PLAYERNAME))).getDataCopy();
			if(properties.contains(static_cast<nByte>(Properties::Player::IS_INACTIVE)))
				mIsInactive = ValueObject<bool>(properties.getValue(static_cast<nByte>(Properties::Player::IS_INACTIVE))).getDataCopy();
			if(properties.contains(static_cast<nByte>(Properties::Player::USER_ID)))
				mUserID = ValueObject<JString>(properties.getValue(static_cast<nByte>(Properties::Player::USER_ID))).getDataCopy();
			mCustomProperties.put(Utils::stripToCustomProperties(properties));
			mCustomProperties = Utils::stripKeysWithNullValues(mCustomProperties);
		}

		bool Player::operator==(const Player& player) const
		{
			return getNumber() == player.getNumber();
		}

		JString& Player::toString(JString& retStr, bool withTypes) const
		{
			return retStr += toString(withTypes, false);
		}

		JString Player::toString(bool withTypes, bool withCustomProperties) const
		{
			return JString() + mNumber + L"={" + payloadToString(withCustomProperties, withTypes) + L"}";
		}

		JString Player::payloadToString(bool withTypes, bool withCustomProperties) const
		{
			JString res = JString(L"num: ") + mNumber + L" name: " + mName + L" userID: " + mUserID;
			if(withCustomProperties && mCustomProperties.getSize())
				res += L" props: " + mCustomProperties.toString(withTypes);
			return res;
		}
	}
}