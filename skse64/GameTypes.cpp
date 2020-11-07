#include "skse64/GameTypes.h"

BSString::~BSString()
{
	if (m_data)
	{
		Heap_Free(m_data);
		m_data = NULL;
	}
}

const char * BSString::Get(void) const
{
	return m_data ? m_data : "";
}

StringCache::Ref::Ref()
{
	CALL_MEMBER_FN(this, ctor)("");
}


StringCache::Ref::Ref(const char * buf)
{
	CALL_MEMBER_FN(this, ctor)(buf);
}

void SimpleLock::Lock(UInt32 pauseAttempts)
{
	SInt32 myThreadID = GetCurrentThreadId();

	_mm_lfence();
	if (threadID == myThreadID)
	{
		InterlockedIncrement(&lockCount);
	}
	else
	{
		UInt32 attempts = 0;
		if (InterlockedCompareExchange(&lockCount, 1, 0))
		{
			do
			{
				++attempts;
				_mm_pause();
				if (attempts >= pauseAttempts) {
					UInt32 spinCount = 0;
					while (InterlockedCompareExchange(&lockCount, 1, 0))
						Sleep(++spinCount < kFastSpinThreshold ? 0 : 1);
					break;
				}
			} while (InterlockedCompareExchange(&lockCount, 1, 0));
			_mm_lfence();
		}

		threadID = myThreadID;
		_mm_sfence();
	}
}

void SimpleLock::Release(void)
{
	SInt32 myThreadID = GetCurrentThreadId();

	_mm_lfence();
	if (threadID == myThreadID)
	{
		if (lockCount == 1)
		{
			threadID = 0;
			_mm_mfence();
			InterlockedCompareExchange(&lockCount, 0, 1);
		}
		else
		{
			InterlockedDecrement(&lockCount);
		}
	}
}

/*** using runtime internal implementation rather than our reimplementation so it can be hooked by other plugins

void BSReadWriteLock::LockForRead()
{
	SInt32 myThreadID = GetCurrentThreadId();

	if (threadID == myThreadID)
	{
		InterlockedIncrement(&lockValue);
	}
	else
	{
		UInt32 lockCount = lockValue & kLockCountMask;
		UInt32 spinCount = 0;
		UInt32 lockResult = InterlockedCompareExchange(&lockValue, lockCount + 1, lockCount);
		while (lockResult != lockCount + 1)
		{
			if ((lockResult & kLockWrite) != 0)
			{
				Sleep(++spinCount < kFastSpinThreshold ? 0 : 1);
				lockResult = lockValue;
			}
				
			lockCount = lockValue & kLockCountMask;
			lockResult = InterlockedCompareExchange(&lockValue, lockCount + 1, lockCount);
		}

		threadID = myThreadID;
	}
}

void BSReadWriteLock::LockForWrite()
{
	SInt32 myThreadID = GetCurrentThreadId();

	if (threadID == myThreadID)
	{
		InterlockedIncrement(&lockValue);
	}
	else
	{
		UInt32 spinCount = 0;
		while (InterlockedCompareExchange(&lockValue, UInt32(1 | kLockWrite), 0) != UInt32(1 | kLockWrite))
			Sleep(++spinCount < kFastSpinThreshold ? 0 : 1);

		threadID = myThreadID;
		_mm_mfence();
	}
}

void BSReadWriteLock::LockForReadAndWrite()
{
	SInt32 myThreadID = GetCurrentThreadId();

	if (threadID == myThreadID)
	{
		InterlockedIncrement(&lockValue);
	}
	else
	{
		UInt32 spinCount = 0;
		while (InterlockedCompareExchange(&lockValue, 1, 0) != 1)
			Sleep(++spinCount >= kFastSpinThreshold ? 1 : 0);
	}
}

bool BSReadWriteLock::TryLockForWrite()
{
	SInt32 myThreadID = GetCurrentThreadId();

	bool result = false;
	if (threadID == myThreadID)
	{
		InterlockedIncrement(&lockValue);
		result = true;
	}
	else
	{
		result = InterlockedCompareExchange(&lockValue, UInt32(1 | kLockWrite), 0) == UInt32(1 | kLockWrite);
		if (result)
		{
			threadID = myThreadID;
			_mm_mfence();
		}
	}
	return result;
}
bool BSReadWriteLock::TryLockForRead()
{
	SInt32 myThreadID = GetCurrentThreadId();

	bool result = false;
	if (threadID == myThreadID)
	{
		InterlockedIncrement(&lockValue);
		result = true;
	}
	else
	{
		UInt32 lockCount = lockValue & kLockCountMask;
		UInt32 lockResult = InterlockedCompareExchange(&lockValue, lockCount + 1, lockCount);
		while ((lockResult & kLockWrite) == 0)
		{
			if (lockResult == lockCount)
				break;

			lockCount = lockResult & kLockCountMask;
			lockResult = InterlockedCompareExchange(&lockValue, lockCount + 1, lockCount);
		}

		result = ~(lockResult >> 31) & 1;
	}

	return result;
}

void BSReadWriteLock::Unlock()
{
	SInt32 myThreadID = GetCurrentThreadId();
	if (threadID == myThreadID)
	{
		UInt32 lockCount = lockValue - 1;
		if (lockValue == kLockWrite)
		{
			threadID = 0;
			_mm_mfence();
			InterlockedExchange(&lockValue, 0);
		}
		else
		{
			InterlockedDecrement(&lockValue);
		}
	}
	else
	{
		InterlockedDecrement(&lockValue);
	}
}*/

void UpdateRegistrationHolder::Order(UInt32 index)
{
	UInt32 count = m_regs.count;
	UInt32 pivotIndex = count >> 1;

	if (index >= pivotIndex)
		return;

	Registration * pOldReg = m_regs[index];
	UInt32 startIndex = index;
	UInt32 cmpIndex;
	do
	{
		cmpIndex = 2 * index + 1;

		if (cmpIndex < (count - 1))
		{
			Registration * pCur = m_regs[cmpIndex];
			Registration * pSucc = m_regs[cmpIndex + 1];
			if (!pCur || !pSucc || pCur->schedTime > pSucc->schedTime)
				cmpIndex++;
		}

		Registration * pCmpReg = m_regs[cmpIndex];
		if (!pCmpReg || !pOldReg || pCmpReg->schedTime > pOldReg->schedTime)
			break;

		m_regs[index] = pCmpReg;
		index = cmpIndex;

	} while (cmpIndex < pivotIndex);

	if (index != startIndex)
		m_regs[index] = pOldReg;

}

bool UpdateRegistrationHolder::Remove(UInt64 & handle)
{
	if (m_regs.count == 0)
		return false;

	for (UInt32 i = 0; i<m_regs.count; i++)
	{
		Registration * pReg = m_regs[i];

		if (pReg && pReg->handle == handle)
		{
			UInt32 lastIndex = m_regs.count - 1;

			// Remove last entry => no reorganizing necessary
			if (i == lastIndex)
			{
				pReg->Release();
				m_regs.count--;
			}
			else
			{
				m_regs[i] = m_regs[lastIndex];
				m_regs[lastIndex] = pReg;	// seems pointless, but the original code does it

				pReg->Release();
				m_regs.count--;

				Order(i);
			}
			return true;
		}
	}
	return false;
}
