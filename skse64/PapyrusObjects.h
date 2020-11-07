#pragma once

#include <map>
#include <string>
#include <vector>

#include "common/ICriticalSection.h"

struct SKSESerializationInterface;
class TESForm;
class VMClassRegistry;

// Serializable classes deriving from ISKSEObject must define a
// serialization constructor that takes SerializationTag as the single argument.
// After it has been called, either Load() is used to fill in the data,
// or in case the load failed, the instance is destroyed again.
struct SerializationTag {};

///
/// ISKSEObject - Interface for a heap allocated, co-save serializable object.
///

class ISKSEObject
{
public:
	virtual ~ISKSEObject() {}

	virtual const char*	ClassName() const    = 0;
	virtual UInt32		ClassVersion() const = 0;

	virtual bool Save(SKSESerializationInterface* intfc) = 0;
	virtual bool Load(SKSESerializationInterface* intfc, UInt32 version) = 0;
};

///
/// SKSEObjectFactory
///

class ISKSEObjectFactory
{
public:
	virtual ~ISKSEObjectFactory() {}

	virtual ISKSEObject* Create() const = 0;
	virtual const char*	ClassName() const = 0;
};

template <typename T>
class ConcreteSKSEObjectFactory : public ISKSEObjectFactory
{
public:
	virtual ISKSEObject* Create() const
	{
		SerializationTag tag;
		return new T( tag );
	}
	virtual const char*	ClassName() const
	{
		SerializationTag tag;
		T tempInstance(tag);
		return tempInstance.ClassName();
	}
};

///
/// SKSEObjectRegistry
///

class SKSEObjectRegistry
{
private:
	typedef std::map<std::string,uintptr_t> FactoryMapT;

public:
	template <typename T>
	bool RegisterClass()
	{
		ConcreteSKSEObjectFactory<T> factory;
		RegisterFactory(&factory);
		return true;
	}

	virtual void RegisterFactory(ISKSEObjectFactory * factory);
	virtual const ISKSEObjectFactory* GetFactoryByName(const char* name) const;

private:
	// Stores the vtables directly
	FactoryMapT factoryMap_;
};

///
/// SKSEPersistentObjectStorage
///

class SKSEPersistentObjectStorage
{
	struct Entry;

	// Note: handle = index + 1
	// +1, because in previous versions the invalid handle was 0 so people
	// might test for != 0
public:

	// Transfer ownership to registry
	template <typename T>
	SInt32 StoreObject(T* obj, UInt32 owningStackId)
	{// lock_
		return Store(obj, owningStackId);
	}// ~lock_

	// Access object while keeping in storage
	template <typename T>
	T* AccessObject(SInt32 handle)
	{// lock_
		return reinterpret_cast<T*>(Access(handle));
	}// ~lock_

	// Remove object from storage and take ownership of the pointer
	template <typename T>
	T* TakeObject(SInt32 handle)
	{// lock_
		return reinterpret_cast<T*>(Take(handle));
	}// lock_

	virtual SInt32 Store(ISKSEObject* obj, UInt32 owningStackId);
	virtual ISKSEObject* Access(SInt32 handle);
	virtual ISKSEObject* Take(SInt32 handle);

	bool Save(SKSESerializationInterface* intfc);
	bool Load(SKSESerializationInterface* intfc, UInt32 version);

	void CleanDroppedStacks();
	void ClearAndRelease();

	enum { kSaveVersion = 1 };

private:
	struct Entry
	{
		ISKSEObject*	obj;
		UInt32			owningStackId;
	};

	typedef std::vector<Entry>		DataT;
	typedef std::vector<size_t>		IndexCacheT;

	ICriticalSection	lock_;
	DataT				data_;
	IndexCacheT			freeIndices_;
};

///
/// Serialization helpers
///

bool WriteSKSEObject(SKSESerializationInterface* intfc, ISKSEObject* obj);
bool ReadSKSEObject(SKSESerializationInterface* intfc, ISKSEObject*& objOut);

///
/// Global instances
///

SKSEObjectRegistry& SKSEObjectRegistryInstance();

SKSEPersistentObjectStorage& SKSEObjectStorageInstance();
