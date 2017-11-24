/* Exit Games Common - C++ Client Lib
 * Copyright (C) 2004-2017 by Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#pragma once

#include "Common-cpp/inc/BaseListener.h"

namespace ExitGames
{
	namespace Common
	{
		class Logger
		{
		public:
			Logger(int debugLevel=DebugLevel::WARNINGS);
			~Logger(void);
			void log(int debugLevel, const EG_CHAR* file, const EG_CHAR* function, bool printBrackets, unsigned int line, const EG_CHAR* dbgMsg, ...) const;
			void vlog(int debugLevel, const EG_CHAR* file, const EG_CHAR* function, bool printBrackets, unsigned int line, const EG_CHAR* dbgMsg, va_list args) const;
			int getDebugOutputLevel(void) const;
			bool setDebugOutputLevel(int debugLevel);
			void setListener(const BaseListener& listener);
		private:
			int mDebugLevel;
#ifdef EG_LOGGING
			BaseListener* mpListener;
#	ifdef _EG_SWITCH_WINDOWS_PLATFORM
			mutable bool mInitialized;
#	endif
#endif
		};
	}
}

// public logging API
#ifdef EG_LOGGING
#	define EGLOG(debugLevel, ...) mLogger.log(debugLevel, __WFILE__, __WFUNCTION__, true, __LINE__, __VA_ARGS__)
#else
#	define EGLOG(debugLevel, ...) ((void)0)
#endif