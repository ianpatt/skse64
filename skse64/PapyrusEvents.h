#pragma once

#include "GameForms.h"
#include "GameTypes.h"
#include "GameEvents.h"
#include "GameInput.h"
#include "GameCamera.h"
#include <map>
#include <unordered_map>
#include <set>
#include "Serialization.h"
#include "PapyrusVM.h"

// Performance: Hash function for BSFixedString to enable unordered_map
// BSFixedString uses StringCache, so identical strings have same pointer
// We can hash the pointer directly for O(1) lookup instead of comparing string contents
namespace std {
	template<>
	struct hash<BSFixedString> {
		size_t operator()(const BSFixedString& str) const {
			// Hash the cached string pointer (not contents) since StringCache guarantees
			// identical strings have the same pointer address
			return hash<const char*>()(str.data);
		}
	};
}

template <typename D>
class EventRegistration
{
public:

	UInt64	handle;
	D		params;

	bool operator<(const EventRegistration & rhs) const	{ return handle < rhs.handle; }

	bool Save(SKSESerializationInterface * intfc, UInt32 version) const
	{
		if (! intfc->WriteRecordData(&handle, sizeof(handle)))
			return false;
		if (! params.Save(intfc, version))
			return false;
		return true;
	}

	bool Load(SKSESerializationInterface * intfc, UInt32 version)
	{
		if (! intfc->ReadRecordData(&handle, sizeof(handle)))
			return false;
		if (! params.Load(intfc, version))
			return false;
		return true;
	}

	void Dump(void)
	{
		_MESSAGE("> handle:\t%016I64x", handle);
		params.Dump();
	}
};

class NullParameters
{
public:
	bool Save(SKSESerializationInterface * intfc, UInt32 version) const { return true; }
	bool Load(SKSESerializationInterface * intfc, UInt32 version) { return true; }
	void Dump(void) {}
};

class ModCallbackParameters
{
public:
	BSFixedString callbackName;

	bool Save(SKSESerializationInterface * intfc, UInt32 version) const
	{
		return Serialization::WriteData<BSFixedString>(intfc, &callbackName);
	}

	bool Load(SKSESerializationInterface * intfc, UInt32 version)
	{
		return Serialization::ReadData<BSFixedString>(intfc, &callbackName);
	}

	void Dump(void)
	{
		_MESSAGE("> callbackName:\t%s", callbackName);
	}
};

// Performance: Data-oriented storage for event registrations
// Stores handles and params in contiguous arrays for cache-friendly iteration
template <typename D>
class DataOrientedRegistrations
{
public:
	std::vector<UInt64> handles;  // All handles in contiguous memory
	std::vector<D> params;         // All params in contiguous memory

	// Register a new handler (maintains sorted order by handle)
	bool insert(UInt64 handle, const D& param)
	{
		// Binary search for insertion point
		auto it = std::lower_bound(handles.begin(), handles.end(), handle);

		// Already exists?
		if (it != handles.end() && *it == handle)
			return false;

		// Insert at correct position to maintain sorted order
		size_t index = it - handles.begin();
		handles.insert(it, handle);
		params.insert(params.begin() + index, param);
		return true;
	}

	// Unregister a handler
	bool erase(UInt64 handle)
	{
		auto it = std::lower_bound(handles.begin(), handles.end(), handle);
		if (it == handles.end() || *it != handle)
			return false;

		size_t index = it - handles.begin();
		handles.erase(it);
		params.erase(params.begin() + index);
		return true;
	}

	// Get registration count
	size_t size() const { return handles.size(); }

	// Check if empty
	bool empty() const { return handles.empty(); }

	// Clear all registrations
	void clear()
	{
		handles.clear();
		params.clear();
	}

	// Iterator for compatibility with existing code
	struct Iterator
	{
		DataOrientedRegistrations* owner;
		size_t index;

		EventRegistration<D> operator*() const
		{
			EventRegistration<D> reg;
			reg.handle = owner->handles[index];
			reg.params = owner->params[index];
			return reg;
		}

		Iterator& operator++() { ++index; return *this; }
		bool operator!=(const Iterator& other) const { return index != other.index; }
	};

	Iterator begin() { return Iterator{this, 0}; }
	Iterator end() { return Iterator{this, handles.size()}; }
};

template <typename K, typename D = NullParameters>
class RegistrationMapHolder : public SafeDataHolder<std::unordered_map<K,DataOrientedRegistrations<D>>>
{
	typedef DataOrientedRegistrations<D>	RegSet;
	// Performance: Use unordered_map for O(1) event dispatch instead of O(log n)
	// Critical for input events, action events, and other high-frequency game events
	// Data-oriented storage: handles and params in contiguous arrays for cache locality
	typedef std::unordered_map<K,RegSet>	RegMap;

public:

	void Register(K & key, UInt64 handle, D * params = NULL)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

		D param_copy;
		if (params)
			param_copy = *params;

		this->Lock();

		// Performance: Data-oriented storage with contiguous arrays
		if (this->m_data[key].insert(handle, param_copy))
			policy->AddRef(handle);

		this->Release();
	}

	template <typename T>
	void Register(K & key, UInt32 type, T * classType, D * params = NULL)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

		UInt64 handle = policy->Create(type, (void *)classType);
		D param_copy;
		if (params)
			param_copy = *params;

#ifdef _DEBUG
		_MESSAGE("Executed PapyrusEvents::Register - %016llX", handle);
#endif

		this->Lock();

		// Performance: Data-oriented storage with contiguous arrays
		if (this->m_data[key].insert(handle, param_copy))
			policy->AddRef(handle);

		this->Release();
	}

	void Unregister(K & key, UInt64 handle)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

		this->Lock();

		// Performance: Data-oriented storage with binary search for removal
		if (this->m_data[key].erase(handle))
			policy->Release(handle);

		this->Release();
	}

	template <typename T>
	void Unregister(K & key, UInt32 type, T * classType)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

		UInt64 handle = policy->Create(type, (void *)classType);

		this->Lock();

		// Performance: Data-oriented storage with binary search for removal
		if (this->m_data[key].erase(handle))
			policy->Release(handle);

		this->Release();
	}

	void UnregisterAll(UInt64 handle)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

		this->Lock();

		// Performance: Data-oriented storage - iterate through all event types
		for (auto iter = this->m_data.begin(); iter != this->m_data.end(); ++iter)
			if (iter->second.erase(handle))
				policy->Release(handle);

		this->Release();
	}

	template <typename T>
	void UnregisterAll(UInt32 type, T * classType)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

		UInt64 handle = policy->Create(type, (void *)classType);

		this->Lock();

		// Performance: Data-oriented storage - iterate through all event types
		for (auto iter = this->m_data.begin(); iter != this->m_data.end(); ++iter)
			if (iter->second.erase(handle))
				policy->Release(handle);

		this->Release();
	}

	template <typename F>
	void ForEach(K & key, F & functor)
	{
		this->Lock();

		auto handles = this->m_data.find(key);

		if (handles != this->m_data.end())
			for (auto iter = handles->second.begin(); iter != handles->second.end(); ++iter)
				functor(*iter);

		this->Release();
	}

	void Clear(void)
	{
		this->Lock();
		this->m_data.clear();
		this->Release();
	}

	bool Save(SKSESerializationInterface * intfc, UInt32 type, UInt32 version)
	{
		intfc->OpenRecord(type, version);

		this->Lock();

		for (auto iter = this->m_data.begin(); iter != this->m_data.end(); ++iter)
		{
			UInt32 numRegs = iter->second.size();

			if (!numRegs)
				continue;

			intfc->OpenRecord('REGS', version);

			// Key
			Serialization::WriteData(intfc, &iter->first);
			// Reg count
			intfc->WriteRecordData(&numRegs, sizeof(numRegs));

			// Performance: Data-oriented storage - iterate contiguous arrays
			for (size_t i = 0; i < numRegs; i++)
			{
				// Save handle
				if (!intfc->WriteRecordData(&iter->second.handles[i], sizeof(UInt64)))
					return false;
				// Save params
				if (!iter->second.params[i].Save(intfc, version))
					return false;
			}
		}

		intfc->OpenRecord('REGE', version);

		this->Release();

		return true;
	}

	bool Load(SKSESerializationInterface* intfc, UInt32 version)
	{
		UInt32 type, length, curVersion;

		while (intfc->GetNextRecordInfo(&type, &curVersion, &length))
		{
			switch (type)
			{
				case 'REGS':
				{
					K curKey;
					// Key
					if (! Serialization::ReadData(intfc, &curKey))
					{
						_MESSAGE("Error loading reg key");
						return false;
					}

					// Reg count
					UInt32 numRegs = 0;
					if (! intfc->ReadRecordData(&numRegs, sizeof(numRegs)))
					{
						_MESSAGE("Error loading reg count");
						return false;
					}

					// Performance: Data-oriented storage - load into contiguous arrays
					for (UInt32 i=0; i<numRegs; i++)
					{
						UInt64 handle;
						D params;

						// Load handle
						if (!intfc->ReadRecordData(&handle, sizeof(handle)))
						{
							_MESSAGE("Error loading handle");
							return false;
						}

						// Load params
						if (!params.Load(intfc, version))
						{
							_MESSAGE("Error loading params");
							return false;
						}

						VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
						IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

						UInt64 newHandle = 0;

						// Skip if handle is no longer valid.
						if (! intfc->ResolveHandle(handle, &newHandle))
							continue;

						this->Lock();

						if (this->m_data[curKey].insert(newHandle, params))
							policy->AddRef(newHandle);

						this->Release();
					}

					break;
				}
				case 'REGE':
				{
					return true;
				}
				default:
				{
					_MESSAGE("Error loading unexpected chunk type %08X (%.4s)", type, &type);
					return false;
				}
			}
		}

		_MESSAGE("Missing record data for type %08X (%.4s)", type, &type);
		return false;
	}
};

template <typename D = NullParameters>
class RegistrationSetHolder : public SafeDataHolder<DataOrientedRegistrations<D>>
{
	typedef DataOrientedRegistrations<D>	RegSet;

public:

	void Register(UInt64 handle, D * params = NULL)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

		D param_copy;
		if (params)
			param_copy = *params;

		this->Lock();

		// Performance: Data-oriented storage with contiguous arrays
		if (this->m_data.insert(handle, param_copy))
			policy->AddRef(handle);

		this->Release();
	}

	template <typename T>
	void Register(UInt32 type, T * classType, D * params = NULL)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

		UInt64 handle = policy->Create(type, (void *)classType);
		D param_copy;
		if (params)
			param_copy = *params;

		this->Lock();

		// Performance: Data-oriented storage with contiguous arrays
		if (this->m_data.insert(handle, param_copy))
			policy->AddRef(handle);

		this->Release();
	}

	void Unregister(UInt64 handle)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

		this->Lock();

		// Performance: Data-oriented storage with binary search for removal
		if (this->m_data.erase(handle))
			policy->Release(handle);

		this->Release();
	}

	template <typename T>
	void Unregister(UInt32 type, T * classType)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

		UInt64 handle = policy->Create(type, (void *)classType);

		this->Lock();

		// Performance: Data-oriented storage with binary search for removal
		if (this->m_data.erase(handle))
			policy->Release(handle);

		this->Release();
	}

	template <typename F>
	void ForEach(F & functor)
	{
		this->Lock();

		for (auto iter = this->m_data.begin(); iter != this->m_data.end(); ++iter)
			functor(*iter);

		this->Release();
	}

	void Clear(void)
	{
		this->Lock();
		this->m_data.clear();
		this->Release();
	}

	bool Save(SKSESerializationInterface * intfc, UInt32 type, UInt32 version)
	{
		intfc->OpenRecord(type, version);

		this->Lock();

		UInt32 numRegs = this->m_data.size();

		// Reg count
		intfc->WriteRecordData(&numRegs, sizeof(numRegs));

		// Performance: Data-oriented storage - iterate contiguous arrays
		for (size_t i = 0; i < numRegs; i++)
		{
			// Save handle
			if (!intfc->WriteRecordData(&this->m_data.handles[i], sizeof(UInt64)))
				return false;
			// Save params
			if (!this->m_data.params[i].Save(intfc, version))
				return false;
		}

		this->Release();

		return true;
	}

	bool Load(SKSESerializationInterface* intfc, UInt32 version)
	{
		// Reg count
		UInt32 numRegs = 0;
		if (! intfc->ReadRecordData(&numRegs, sizeof(numRegs)))
		{
			_MESSAGE("Error loading reg count");
			return false;
		}

		// Performance: Data-oriented storage - load into contiguous arrays
		for (UInt32 i=0; i<numRegs; i++)
		{
			UInt64 handle;
			D params;

			// Load handle
			if (!intfc->ReadRecordData(&handle, sizeof(handle)))
			{
				_MESSAGE("Error loading handle");
				return false;
			}

			// Load params
			if (!params.Load(intfc, version))
			{
				_MESSAGE("Error loading params");
				return false;
			}

			VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
			IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

			UInt64 newHandle = 0;

			// Skip if handle is no longer valid.
			if (! intfc->ResolveHandle(handle, &newHandle))
				continue;

			this->Lock();

			if (this->m_data.insert(newHandle, params))
				policy->AddRef(newHandle);

			this->Release();
		}

		return true;
	}
};

extern RegistrationMapHolder<BSFixedString>							g_menuOpenCloseRegs;
extern RegistrationMapHolder<UInt32>								g_inputKeyEventRegs;
extern RegistrationMapHolder<BSFixedString>							g_inputControlEventRegs;
extern RegistrationMapHolder<BSFixedString,ModCallbackParameters>	g_modCallbackRegs;

extern RegistrationSetHolder<NullParameters>						g_cameraEventRegs;
extern RegistrationSetHolder<NullParameters>						g_crosshairRefEventRegs;
extern RegistrationMapHolder<UInt32>								g_actionEventRegs;
extern RegistrationSetHolder<NullParameters>						g_ninodeUpdateEventRegs;

struct SKSEModCallbackEvent
{
	BSFixedString	eventName;
	BSFixedString	strArg;
	float			numArg;
	TESForm *		sender;

	SKSEModCallbackEvent(BSFixedString a_eventName, BSFixedString a_strArg, float a_numArg, TESForm * a_sender) :
		eventName(a_eventName.data), strArg(a_strArg.data), numArg(a_numArg), sender(a_sender) {}
};

template <>
class BSTEventSink <SKSEModCallbackEvent>
{
public:
	virtual ~BSTEventSink() {}; // todo?
	virtual	EventResult ReceiveEvent(SKSEModCallbackEvent * evn, EventDispatcher<SKSEModCallbackEvent> * dispatcher) = 0;
};

extern EventDispatcher<SKSEModCallbackEvent> g_modCallbackEventDispatcher;

struct SKSECameraEvent
{
	TESCameraState * oldState;
	TESCameraState * newState;

	SKSECameraEvent(TESCameraState * a_oldState, TESCameraState * a_newState) :
		oldState(a_oldState), newState(a_newState) {}
};

template <>
class BSTEventSink <SKSECameraEvent>
{
public:
	virtual ~BSTEventSink() {}; // todo?
	virtual	EventResult ReceiveEvent(SKSECameraEvent * evn, EventDispatcher<SKSECameraEvent> * dispatcher) = 0;
};

extern EventDispatcher<SKSECameraEvent> g_cameraEventDispatcher;

struct SKSECrosshairRefEvent
{
	NiPointer<TESObjectREFR>	crosshairRef;

	SKSECrosshairRefEvent(NiPointer<TESObjectREFR> & a_crosshairRef);
};

template <>
class BSTEventSink <SKSECrosshairRefEvent>
{
public:
	virtual ~BSTEventSink() {}; // todo?
	virtual	EventResult ReceiveEvent(SKSECrosshairRefEvent * evn, EventDispatcher<SKSECrosshairRefEvent> * dispatcher) = 0;
};

extern EventDispatcher<SKSECrosshairRefEvent> g_crosshairRefEventDispatcher;

struct SKSEActionEvent
{
	enum {
		kType_WeaponSwing = 0,
		kType_SpellCast = 1,
		kType_SpellFire = 2,
		kType_VoiceCast = 3,
		kType_VoiceFire = 4,
		kType_BowDraw = 5,
		kType_BowRelease = 6,
		kType_BeginDraw = 7,
		kType_EndDraw = 8,
		kType_BeginSheathe = 9,
		kType_EndSheathe = 10
	};
	enum {
		kSlot_Left = 0,
		kSlot_Right = 1,
		kSlot_Voice = 2
	};
	UInt32 type;
	Actor * actor;
	TESForm	* sourceForm;
	UInt32	slot;

	SKSEActionEvent(UInt32 a_type, Actor * a_actor, TESForm * a_source, UInt32 a_slot) :
	type(a_type), actor(a_actor), sourceForm(a_source), slot(a_slot) {}
};

struct SKSENiNodeUpdateEvent
{
	TESObjectREFR * reference;

	SKSENiNodeUpdateEvent(TESObjectREFR * a_reference) : reference(a_reference) {}
};

template <>
class BSTEventSink <SKSENiNodeUpdateEvent>
{
public:
	virtual ~BSTEventSink() {}; // todo?
	virtual	EventResult ReceiveEvent(SKSENiNodeUpdateEvent * evn, EventDispatcher<SKSENiNodeUpdateEvent> * dispatcher) = 0;
};

extern EventDispatcher<SKSENiNodeUpdateEvent> g_ninodeUpdateEventDispatcher;

template <>
class BSTEventSink <SKSEActionEvent>
{
public:
	virtual ~BSTEventSink() {}; // todo?
	virtual	EventResult ReceiveEvent(SKSEActionEvent * evn, EventDispatcher<SKSEActionEvent> * dispatcher) = 0;
};

extern EventDispatcher<SKSEActionEvent> g_actionEventDispatcher;


class MenuEventHandler : public BSTEventSink <MenuOpenCloseEvent>
{
public:
	virtual EventResult		ReceiveEvent(MenuOpenCloseEvent * evn, EventDispatcher<MenuOpenCloseEvent> * dispatcher);
};

class InputEventHandler : public BSTEventSink <InputEvent>
{
public:
	virtual EventResult		ReceiveEvent(InputEvent ** evns, InputEventDispatcher * dispatcher);
};

class ModCallbackEventHandler :	public BSTEventSink <SKSEModCallbackEvent>
{
public:
	virtual	EventResult		ReceiveEvent(SKSEModCallbackEvent * evn, EventDispatcher<SKSEModCallbackEvent> * dispatcher);
};

class CameraEventHandler :	public BSTEventSink <SKSECameraEvent>
{
public:
	virtual	EventResult		ReceiveEvent(SKSECameraEvent * evn, EventDispatcher<SKSECameraEvent> * dispatcher);
};

class CrosshairRefEventHandler : public BSTEventSink <SKSECrosshairRefEvent>
{
public:
	virtual	EventResult		ReceiveEvent(SKSECrosshairRefEvent * evn, EventDispatcher<SKSECrosshairRefEvent> * dispatcher);
};

class ActionEventHandler : public BSTEventSink <SKSEActionEvent>
{
public:
	virtual	EventResult		ReceiveEvent(SKSEActionEvent * evn, EventDispatcher<SKSEActionEvent> * dispatcher);
};

class NiNodeUpdateEventHandler : public BSTEventSink <SKSENiNodeUpdateEvent>
{
public:
	virtual	EventResult		ReceiveEvent(SKSENiNodeUpdateEvent * evn, EventDispatcher<SKSENiNodeUpdateEvent> * dispatcher);
};

extern MenuEventHandler				g_menuEventHandler;
extern InputEventHandler			g_inputEventHandler;
extern ModCallbackEventHandler		g_modCallbackEventHandler;
extern CameraEventHandler			g_cameraEventHandler;
extern CrosshairRefEventHandler		g_crosshairRefEventHandler;
extern ActionEventHandler			g_actionEventHandler;
extern NiNodeUpdateEventHandler		g_ninodeUpdateEventHandler;
