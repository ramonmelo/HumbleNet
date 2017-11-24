/* Exit Games Common - C++ Client Lib
 * Copyright (C) 2004-2017 by Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#pragma once

#include "Common-cpp/inc/MemoryManagement/Allocate.h"

namespace ExitGames
{
	namespace Common
	{
		namespace Helpers
		{
			template<typename Etype>
			class SharedPointer
			{
			public:
				SharedPointer(void);
				SharedPointer(Etype* pData);
				~SharedPointer(void);

				SharedPointer(const SharedPointer<Etype>& toCopy);
				SharedPointer& operator=(const SharedPointer<Etype>& toCopy);

				Etype* operator->(void);
				const Etype* operator->(void) const;
				Etype& operator*(void);
				const Etype& operator*(void) const;
				operator Etype*(void);
				operator const Etype*(void) const;
			private:
				Etype* mpData;
				unsigned long long* mpRefCount;
			};




			template<typename Etype>
			SharedPointer<Etype>::SharedPointer(void)
				: mpData(NULL)
				, mpRefCount(MemoryManagement::allocate<unsigned long long>(1))
			{
			}

			template<typename Etype>
			SharedPointer<Etype>::SharedPointer(Etype* pData)
				: mpData(pData)
				, mpRefCount(MemoryManagement::allocate<unsigned long long>(1))
			{
			}

			template<typename Etype>
			SharedPointer<Etype>::~SharedPointer(void)
			{
				if(--*mpRefCount)
					return;
				MemoryManagement::deallocate(mpData);
				MemoryManagement::deallocate(mpRefCount);
			}

			template<typename Etype>
			SharedPointer<Etype>::SharedPointer(const SharedPointer<Etype>& toCopy)
				: mpData(NULL)
				, mpRefCount(NULL)
			{
				*this = toCopy;
			}

			template<typename Etype>
			SharedPointer<Etype>& SharedPointer<Etype>::operator=(const SharedPointer<Etype>& toCopy)
			{
				if(mpRefCount && !--*mpRefCount)
				{
					MemoryManagement::deallocate(mpData);
					MemoryManagement::deallocate(mpRefCount);
				}
				mpData = toCopy.mpData;
				++*(mpRefCount=toCopy.mpRefCount);
				return *this;
			}

			template<typename Etype>
			Etype* SharedPointer<Etype>::operator->(void)
			{
				return mpData;
			}

			template<typename Etype>
			const Etype* SharedPointer<Etype>::operator->(void) const
			{
				return mpData;
			}

			template<typename Etype>
			Etype& SharedPointer<Etype>::operator*(void)
			{
				return *mpData;
			}

			template<typename Etype>
			const Etype& SharedPointer<Etype>::operator*(void) const
			{
				return *mpData;
			}

			template<typename Etype>
			SharedPointer<Etype>::operator Etype*(void)
			{
				return mpData;
			}

			template<typename Etype>
			SharedPointer<Etype>::operator const Etype*(void) const
			{
				return mpData;
			}
		}
	}
}