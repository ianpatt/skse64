#pragma once

#include "common/ICriticalSection.h"

#include "PapyrusObjects.h"

#include <deque>
#include <new>
#include <utility>
#include <vector>


class VMValue;
struct SKSESerializationInterface;

///
/// ISKSEDelayFunctor
///
class ISKSEDelayFunctor : public ISKSEObject
{
public:
	virtual ~ISKSEDelayFunctor() {}
	
	virtual void Run(VMValue& resultOut) = 0;
	
	virtual bool ShouldReschedule(SInt32& delayMSOut)  = 0;
	virtual bool ShouldResumeStack(UInt32& stackIdOut) = 0;
};

///
/// LatentSKSEDelayFunctor
///
class LatentSKSEDelayFunctor : public ISKSEDelayFunctor
{
public:
	explicit LatentSKSEDelayFunctor(UInt32 stackId);

	explicit LatentSKSEDelayFunctor(SerializationTag);

	virtual ~LatentSKSEDelayFunctor() {}

	virtual const char*	ClassName() const    = 0;
	virtual UInt32		ClassVersion() const = 0;
	
	virtual void Run(VMValue& resultOut) = 0;
	
	virtual bool ShouldReschedule(SInt32& delayMSOut);
	virtual bool ShouldResumeStack(UInt32& stackIdOut);

	virtual bool Save(SKSESerializationInterface* intfc);
	virtual bool Load(SKSESerializationInterface* intfc, UInt32 version);

	UInt32 StackId() const { return stackId_; }

	// Redirect to formheap
	// Note: this is not required, but why not
	static void* operator new(std::size_t size);
	static void* operator new(std::size_t size, const std::nothrow_t&);
	static void* operator new(std::size_t size, void* ptr);

	static void operator delete(void* ptr);
	static void operator delete(void* ptr, const std::nothrow_t&);
	static void operator delete(void*, void*);

protected:
	UInt32 stackId_;
};

///
/// SKSEDelayFunctorQueue
///

class SKSEDelayFunctorQueue
{
	typedef std::deque<ISKSEDelayFunctor*> DataT;

public:
	~SKSEDelayFunctorQueue();

	void				Push(ISKSEDelayFunctor* func);
	ISKSEDelayFunctor*	Pop();

	void ClearAndRelease();

	enum { kSaveVersion = 1 };

	bool Save(SKSESerializationInterface* intfc);
	bool Load(SKSESerializationInterface* intfc, UInt32 version);

private:
	ICriticalSection	lock_;
	DataT				data_;
};

///
/// SKSEDelayFunctorWaitList
///

class SKSEDelayFunctorWaitList
{
private:
	typedef std::pair<SInt64,ISKSEDelayFunctor*>	WaitEntryT;
	typedef std::vector<WaitEntryT>					WaitDataT;
	typedef std::vector<ISKSEDelayFunctor*>			ReadyDataT;

public:
	SKSEDelayFunctorWaitList();

	~SKSEDelayFunctorWaitList();

	void	Add(SInt32 delayMS, ISKSEDelayFunctor* func);

	// 1. Update
	// 2. PopReady until NULL

	void				Update();
	ISKSEDelayFunctor*	PopReady();

	void ClearAndRelease();

	enum { kSaveVersion = 1 };

	bool Save(SKSESerializationInterface* intfc);
	bool Load(SKSESerializationInterface* intfc, UInt32 version);

private:
	SInt64				lastTickTime_;
	SInt64				msToCountMult_;

	ICriticalSection	inLock_;
	WaitDataT			inData_;
	WaitDataT			waitData_;
	ReadyDataT			readyData_;
};

///
/// SKSEDelayFunctorManager
///

class SKSEDelayFunctorManager
{
public:
	SKSEDelayFunctorManager();

	// Takes ownership of passed pointer.
	virtual void Enqueue(ISKSEDelayFunctor* func, SInt32 delayMS = 0);

	void OnPreTick();
	void OnTick(SInt64 startTime, float budget);
	void OnRevert();

	enum { kSaveVersion = 1 };

	bool Save(SKSESerializationInterface* intfc);
	bool Load(SKSESerializationInterface* intfc, UInt32 version);

private:
	float	budgetFreqScale_;

	SKSEDelayFunctorQueue		queue_;
	SKSEDelayFunctorWaitList	waitList_;
};

///
/// Global instances
///

SKSEDelayFunctorManager&  SKSEDelayFunctorManagerInstance();
