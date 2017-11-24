/* Exit Games Common - C++ Client Lib
 * Copyright (C) 2004-2017 by Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#pragma once

#include "Common-cpp/inc/defines.h"

namespace ExitGames
{
	namespace Common
	{
		namespace MemoryManagement
		{
			namespace Internal
			{
				namespace Interface
				{
					void* malloc(size_t size);
					void free(void* p);
					void* realloc(void* p, size_t size);
					void* calloc(size_t num, size_t size);
				}
			}
		}
	}
}