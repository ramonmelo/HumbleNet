/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2017 by Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#include "LoadBalancing-cpp/inc/WebFlags.h"

/** @file LoadBalancing-cpp/inc/WebFlags.h */

namespace ExitGames
{
	namespace LoadBalancing
	{


		WebFlags::WebFlags(void)
			: mWebFlags(0)
		{
			mWebFlags = 0;
		}

		WebFlags::WebFlags(nByte webFlags)
			: mWebFlags(webFlags)
		{
		}

		nByte WebFlags::getFlags(void) const
		{
			return mWebFlags; 
		}

		WebFlags& WebFlags::setFlags(nByte webFlags)
		{
			mWebFlags = webFlags;
			return *this;
		}

		bool WebFlags::getHttpForward(void) const
		{
			return !!(mWebFlags&M_HTTP_FORWARD); 
		}

		WebFlags& WebFlags::setHttpForward(bool httpWebForward)
		{
			mWebFlags = httpWebForward?mWebFlags|M_HTTP_FORWARD:mWebFlags&M_HTTP_FORWARD;
			return *this;
		}

		bool WebFlags::getSendAuthCookie(void) const
		{
			return !!(mWebFlags&M_SEND_AUTH_COOKIE);
		}

		WebFlags& WebFlags::setSendAuthCookie(bool sendAuthCookie)
		{
			mWebFlags = sendAuthCookie?mWebFlags|M_SEND_AUTH_COOKIE:mWebFlags&M_SEND_AUTH_COOKIE;
			return *this;
		}

		bool WebFlags::getSendSync(void) const
		{
			return !!(mWebFlags&M_SEND_SYNC);
		}

		WebFlags& WebFlags::setSendSync(bool sendSync)
		{
			mWebFlags = sendSync?mWebFlags|M_SEND_SYNC:mWebFlags&M_SEND_SYNC;
			return *this;
		}

		bool WebFlags::getSendState(void) const
		{
			return !!(mWebFlags&M_SEND_STATE);
		}

		WebFlags& WebFlags::setSendState(bool sendState)
		{
			mWebFlags = sendState?mWebFlags|M_SEND_STATE:mWebFlags&M_SEND_STATE;
			return *this;
		}

		Common::JString& WebFlags::toString(Common::JString& retStr, bool withTypes) const
		{
			return retStr += Common::JString() + mWebFlags + L" {" + L"httpForward: " + getHttpForward() + L", SendAuthCookie: " + getSendAuthCookie() + L", sendSync: " + getSendSync() + L", sendState: " + getSendState() + L"}";
		}
	}
}