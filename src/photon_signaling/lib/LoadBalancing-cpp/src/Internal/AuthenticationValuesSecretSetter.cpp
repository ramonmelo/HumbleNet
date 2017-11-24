/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2017 by Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#include "LoadBalancing-cpp/inc/AuthenticationValues.h"
#include "LoadBalancing-cpp/inc/Internal/AuthenticationValuesSecretSetter.h"

namespace ExitGames
{
	namespace LoadBalancing
	{
		namespace Internal
		{
			void AuthenticationValuesSecretSetter::setSecret(AuthenticationValues& authenticationValues, const Common::JString& secret)
			{
				authenticationValues.setSecret(secret);
			}
		}
	}
}