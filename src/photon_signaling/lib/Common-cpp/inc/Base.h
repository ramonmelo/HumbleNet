/* Exit Games Common - C++ Client Lib
 * Copyright (C) 2004-2017 by Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#pragma once

#include "Common-cpp/inc/Logger.h"
#include "Common-cpp/inc/MemoryManagement/Allocate.h"

namespace ExitGames
{
	namespace Common
	{
		class Base : public ToString
		{
		public:
			virtual ~Base(void);
			static void setListener(const BaseListener* baseListener);
			static int getDebugOutputLevel(void);
			static bool setDebugOutputLevel(int debugLevel);
		protected:
			static Logger mLogger;
		};
	}
}