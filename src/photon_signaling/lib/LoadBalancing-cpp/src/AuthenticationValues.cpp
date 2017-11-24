/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2017 by Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#include "LoadBalancing-cpp/inc/AuthenticationValues.h"

/** @file LoadBalancing-cpp/inc/AuthenticationValues.h */

namespace ExitGames
{
	namespace LoadBalancing
	{
		using namespace Common;

		AuthenticationValues::AuthenticationValues(void)
			: mType(CustomAuthenticationType::NONE)
			, mParameters(JString())
			, mData(JVector<nByte>())
			, mSecret(JString())
			, mUserID(JString())
		{
		}

		nByte AuthenticationValues::getType(void) const
		{
			return mType;
		}

		AuthenticationValues& AuthenticationValues::setType(nByte type)
		{
			mType = type;
			return *this;
		}

		const JString& AuthenticationValues::getParameters(void) const
		{
			return mParameters;
		}

		AuthenticationValues& AuthenticationValues::setParameters(const JString& parameters)
		{
			mParameters = parameters;
			return *this;
		}

		AuthenticationValues& AuthenticationValues::setParametersWithUsernameAndToken(const JString& username, const JString&  token)
		{
			mParameters = L"username=" + username + "&token=" + token;
			return *this;
		}

		const JVector<nByte>& AuthenticationValues::getData(void) const
		{
			return mData;
		}

		AuthenticationValues& AuthenticationValues::setData(const JVector<nByte>& data)
		{
			mData = data;
			return *this;
		}

		const JString& AuthenticationValues::getSecret(void) const
		{
			return mSecret;
		}

		AuthenticationValues& AuthenticationValues::setSecret(const JString& secret)
		{
			mSecret = secret;
			return *this;
		}
		
		/**
		   @returns the unique user id*/
		const JString& AuthenticationValues::getUserID(void) const
		{
			return mUserID;
		}

		AuthenticationValues& AuthenticationValues::setUserID(const JString& userID)
		{
			mUserID = userID;
			return *this;
		}
		
		JString& AuthenticationValues::toString(JString& retStr, bool withTypes) const
		{
			return retStr += JString(L"{") + static_cast<int>(getType()) + L" " + getParameters().toString() + L" " + getData().toString(withTypes) + L" " + getSecret().toString() + L" " + getUserID().toString() + L"}";
		}
	}
}