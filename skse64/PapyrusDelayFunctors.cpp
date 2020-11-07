#include "PapyrusDelayFunctors.h"

#include <algorithm>

#include "GameAPI.h"
#include "PapyrusVM.h"
#include "Serialization.h"

///
/// LatentSKSEDelayFunctor
///

LatentSKSEDelayFunctor::LatentSKSEDelayFunctor(UInt32 stackId) :
	stackId_( stackId )
{}

LatentSKSEDelayFunctor::LatentSKSEDelayFunctor(SerializationTag) :
	stackId_( 0 )
{}

bool LatentSKSEDelayFunctor::ShouldReschedule(SInt32& delayMSOut)
{
	return false;
}

bool LatentSKSEDelayFunctor::ShouldResumeStack(UInt32& stackIdOut)
{
	stackIdOut = stackId_;
	return true;
}

bool LatentSKSEDelayFunctor::Save(SKSESerializationInterface* intfc)
{
	using namespace Serialization;

	WriteData(intfc, &stackId_);

	//_MESSAGE("Serialized STACKID %d", stackId_);

	return true;
}

bool LatentSKSEDelayFunctor::Load(SKSESerializationInterface* intfc, UInt32 version)
{
	using namespace Serialization;

	if (! ReadData(intfc, &stackId_))
		return false;

	//_MESSAGE("De-serialized STACKID %d", stackId_);

	return true;
}

void* LatentSKSEDelayFunctor::operator new(std::size_t size)
{
	return Heap_Allocate(size);
}

void* LatentSKSEDelayFunctor::operator new(std::size_t size, const std::nothrow_t&)
{
	return Heap_Allocate(size);
}

// placement new
void* LatentSKSEDelayFunctor::operator new(std::size_t size, void* ptr)
{
	return ptr;
}

void LatentSKSEDelayFunctor::operator delete(void* ptr)
{
	Heap_Free(ptr);
}

void LatentSKSEDelayFunctor::operator delete(void* ptr, const std::nothrow_t&)
{
	Heap_Free(ptr);
}

void LatentSKSEDelayFunctor::operator delete(void*, void*)
{
	// placement delete
}

///
/// SKSEDelayFunctorQueue
///

SKSEDelayFunctorQueue::~SKSEDelayFunctorQueue()
{
	ClearAndRelease();
}

void SKSEDelayFunctorQueue::Push(ISKSEDelayFunctor* func)
{// lock_
	IScopedCriticalSection scopedLock( &lock_ );

	data_.push_back(func);
}// ~lock_

ISKSEDelayFunctor* SKSEDelayFunctorQueue::Pop()
{
	ISKSEDelayFunctor* result = NULL;

	{// lock_
		IScopedCriticalSection scopedLock( &lock_ );

		if (! data_.empty())
		{
			result = data_.front();
			data_.pop_front();
		}
	}// ~lock_			

	return result;
}

void SKSEDelayFunctorQueue::ClearAndRelease()
{
	for (DataT::iterator it = data_.begin(); it != data_.end(); ++it)
		delete *it;

	data_.clear();
}

bool SKSEDelayFunctorQueue::Save(SKSESerializationInterface* intfc)
{
	using namespace Serialization;

	// Save data
	UInt32 dataSize = data_.size();
	if (! WriteData(intfc,&dataSize))
		return false;

	for (UInt32 i=0; i<dataSize; i++)
	{
		ISKSEDelayFunctor* functor = data_[i];
	
		if (! WriteSKSEObject(intfc, functor))
			return false;
	}

	return true;
}

bool SKSEDelayFunctorQueue::Load(SKSESerializationInterface* intfc, UInt32 loadedVersion)
{
	using namespace Serialization;

	UInt32 dataSize;
	if (! ReadData(intfc,&dataSize))
		return false;

	for (UInt32 i=0; i<dataSize; i++)
	{
		ISKSEObject* obj = NULL;
		if (! ReadSKSEObject(intfc, obj))
			continue;

		// Wront type? Can't happen but who knows.
		ISKSEDelayFunctor* functor = dynamic_cast<ISKSEDelayFunctor*>(obj);
		if (functor == NULL)
		{
			// Throw the loaded object away.
			delete obj;
			continue;
		}

		data_.push_back(functor);
	}

	return true;
}

///
/// SKSEDelayFunctorWaitList
///

SKSEDelayFunctorWaitList::SKSEDelayFunctorWaitList() :
	lastTickTime_( GetPerfCounter() )
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq); 
	freq.QuadPart /= 1000;

	msToCountMult_ = freq.QuadPart;
}

SKSEDelayFunctorWaitList::~SKSEDelayFunctorWaitList()
{
	ClearAndRelease();
}

void SKSEDelayFunctorWaitList::Add(SInt32 delayMS, ISKSEDelayFunctor* func)
{// inLock_
	IScopedCriticalSection scopedLock( &inLock_ );

	WaitEntryT t( msToCountMult_ * delayMS, func );
	inData_.push_back(t);
}// ~inLock_

void SKSEDelayFunctorWaitList::Update()
{
	// Move items from thread-safe input buffer to non-thread safe main queue

	{// inLock_
		IScopedCriticalSection scopedLock( &inLock_ );

		waitData_.insert(waitData_.end(), inData_.begin(), inData_.end());
		inData_.clear();
	}// ~inLock

	SInt64 curTime = GetPerfCounter();
	SInt64 dt      = curTime - lastTickTime_;

	lastTickTime_ = curTime;

	if (dt <= 0)
		return;

	// Decrement wait times by time delta
	for (WaitDataT::iterator it = waitData_.begin(); it != waitData_.end(); ++it)
		if (it->first > 0)
			it->first -= dt;

	struct IsStillWaiting_
	{
		bool operator()(const WaitEntryT& e) { return e.first > 0; }
	};
	IsStillWaiting_ pred;

	// Swap all ready entries to end of the vector#1, add them to vector#2, and truncate #1
	WaitDataT::iterator r = std::partition(waitData_.begin(), waitData_.end(), pred);

	for (WaitDataT::iterator it = r; it != waitData_.end(); ++it)
		readyData_.push_back(it->second );

	waitData_.resize(std::distance(waitData_.begin(), r));
}

ISKSEDelayFunctor* SKSEDelayFunctorWaitList::PopReady()
{
	ISKSEDelayFunctor* result = NULL;

	if (! readyData_.empty())
	{
		result = readyData_.back();
		readyData_.pop_back();
	}			

	return result;
}

void SKSEDelayFunctorWaitList::ClearAndRelease()
{
	for (WaitDataT::iterator it = inData_.begin(); it != inData_.end(); ++it)
		delete it->second;

	inData_.clear();

	for (WaitDataT::iterator it = waitData_.begin(); it != waitData_.end(); ++it)
		delete it->second;

	waitData_.clear();

	for (ReadyDataT::iterator it = readyData_.begin(); it != readyData_.end(); ++it)
		delete *it;

	readyData_.clear();

	// Avoid interval spanning two sessions
	lastTickTime_= GetPerfCounter();
}

bool SKSEDelayFunctorWaitList::Save(SKSESerializationInterface* intfc)
{
	using namespace Serialization;

	// inData_
	UInt32 inDataSize = inData_.size();
	if (! WriteData(intfc,&inDataSize))
		return false;

	for (UInt32 i=0; i<inDataSize; i++)
	{
		SInt64				delay	= inData_[i].first;
		ISKSEDelayFunctor*	functor = inData_[i].second;
	
		if (! WriteSKSEObject(intfc, functor))
			return false;

		if (! WriteData(intfc, &delay))
			return false;
	}

	// waitData_
	UInt32 waitDataSize = waitData_.size();
	if (! WriteData(intfc,&waitDataSize))
		return false;

	for (UInt32 i=0; i<waitDataSize; i++)
	{
		SInt64				delay	= waitData_[i].first;
		ISKSEDelayFunctor*	functor = waitData_[i].second;
	
		if (! WriteSKSEObject(intfc, functor))
			return false;

		if (! WriteData(intfc, &delay))
			return false;
	}

	// readyData_
	UInt32 readyDataSize = readyData_.size();
	if (! WriteData(intfc,&readyDataSize))
		return false;

	for (UInt32 i=0; i<readyDataSize; i++)
	{
		ISKSEDelayFunctor* functor = readyData_[i];
	
		if (! WriteSKSEObject(intfc, functor))
			return false;
	}

	return true;
}

bool SKSEDelayFunctorWaitList::Load(SKSESerializationInterface* intfc, UInt32 loadedVersion)
{
	using namespace Serialization;

	// inData_
	UInt32 inDataSize;
	if (! ReadData(intfc,&inDataSize))
		return false;

	inData_.reserve(inDataSize);

	for (UInt32 i=0; i<inDataSize; i++)
	{
		ISKSEObject* obj = NULL;
		if (! ReadSKSEObject(intfc, obj))
			continue;

		// Wront type? Can't happen but who knows.
		ISKSEDelayFunctor* functor = dynamic_cast<ISKSEDelayFunctor*>(obj);
		if (functor == NULL)
		{
			// Throw the loaded object away.
			delete obj;
			continue;
		}

		SInt64 delay;
		if (! ReadData(intfc, &delay))
			return false;
		
		WaitEntryT t( delay, functor );
		inData_.push_back(t);
	}

	// waitData_
	UInt32 waitDataSize;
	if (! ReadData(intfc,&waitDataSize))
		return false;

	waitData_.reserve(waitDataSize);

	for (UInt32 i=0; i<waitDataSize; i++)
	{
		ISKSEObject* obj = NULL;
		if (! ReadSKSEObject(intfc, obj))
			continue;

		// Wront type? Can't happen but who knows.
		ISKSEDelayFunctor* functor = dynamic_cast<ISKSEDelayFunctor*>(obj);
		if (functor == NULL)
		{
			// Throw the loaded object away.
			delete obj;
			continue;
		}

		SInt64 delay;
		if (! ReadData(intfc, &delay))
			return false;
		
		WaitEntryT t( delay, functor );
		waitData_.push_back(t);
	}

	// readyData_
	UInt32 readyDataSize;
	if (! ReadData(intfc,&readyDataSize))
		return false;

	readyData_.reserve(readyDataSize);

	for (UInt32 i=0; i<readyDataSize; i++)
	{
		ISKSEObject* obj = NULL;
		if (! ReadSKSEObject(intfc, obj))
			continue;

		// Wront type? Can't happen but who knows.
		ISKSEDelayFunctor* functor = dynamic_cast<ISKSEDelayFunctor*>(obj);
		if (functor == NULL)
		{
			// Throw the loaded object away.
			delete obj;
			continue;
		}

		readyData_.push_back(functor);
	}

	return true;
}

///
/// SKSEDelayFunctorManager
///

SKSEDelayFunctorManager::SKSEDelayFunctorManager()
{
	LARGE_INTEGER t;
	QueryPerformanceFrequency(&t);
	budgetFreqScale_ = double(t.QuadPart) * 0.001;
}

void SKSEDelayFunctorManager::Enqueue(ISKSEDelayFunctor* func, SInt32 delayMS)
{
	if (delayMS < 0)
		queue_.Push(func);
	else
		waitList_.Add(delayMS, func);
}

void SKSEDelayFunctorManager::OnPreTick()
{
	waitList_.Update();

	ISKSEDelayFunctor* func;
	while ((func = waitList_.PopReady()) != NULL)
		queue_.Push(func);
}

void SKSEDelayFunctorManager::OnTick(SInt64 startTime, float budget)
{
	SInt64 budgetTime = budget * budgetFreqScale_;

	do
	{
		ISKSEDelayFunctor* functor = queue_.Pop();
		if (functor == NULL)
			break;
	
		VMValue result;
		functor->Run(result);

		SInt32 delayMS;
		if (functor->ShouldReschedule(delayMS))
		{
			if (delayMS > 0)
				waitList_.Add(delayMS, functor);
			else
				queue_.Push(functor);
			continue;
		}

		UInt32 stackId;
		if (functor->ShouldResumeStack(stackId))
		{
			(*g_skyrimVM)->GetClassRegistry()->ResumeStack(stackId, &result);
		}

		delete functor;
	}
	while (GetPerfCounter() - startTime <= budgetTime);
}

void SKSEDelayFunctorManager::OnRevert()
{
	queue_.ClearAndRelease();
	waitList_.ClearAndRelease();
}

bool SKSEDelayFunctorManager::Save(SKSESerializationInterface* intfc)
{
	using namespace Serialization;

	if (! SaveClassHelper(intfc, 'FUNQ', queue_))
		return false;

	if (! SaveClassHelper(intfc, 'WLST', waitList_))
		return false;

	intfc->OpenRecord('____', 1);

	return true;
}

bool SKSEDelayFunctorManager::Load(SKSESerializationInterface* intfc, UInt32 loadedVersion)
{
	using namespace Serialization;

	UInt32 type, length, version;

	while (intfc->GetNextRecordInfo(&type, &version, &length))
	{
		switch (type)
		{
		case 'FUNQ':
			if (! queue_.Load(intfc, version))
				return false;
			break;

		case 'WLST':
			if (! waitList_.Load(intfc, version))
				return false;
			break;

		// Done
		case '____':	
			return true;

		default:
			_MESSAGE("Error loading unexpected chunk type %08X (%.4s)", type, &type);
			break;
		}
	}

	_MESSAGE("Missing record data for SKSEDelayFunctorManager");
	return false;
}

///
/// Global instances
///

SKSEDelayFunctorManager& SKSEDelayFunctorManagerInstance()
{
	static SKSEDelayFunctorManager instance;
	return instance;
}
