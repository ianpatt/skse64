#pragma once

#include "GameForms.h"
#include "GameTypes.h"
#include "GameEvents.h"
#include "GameInput.h"
#include "GameCamera.h"
#include <map>
#include <set>
#include "Serialization.h"
#include "PapyrusVM.h"

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

template <typename K, typename D = NullParameters>
class RegistrationMapHolder : public SafeDataHolder<std::map<K,std::set<EventRegistration<D>>>>
{
	typedef std::set<EventRegistration<D>>	RegSet;
	typedef std::map<K,RegSet>				RegMap;

public:

	void Register(K & key, UInt64 handle, D * params = NULL)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

		EventRegistration<D> reg;
		reg.handle = handle;
		if (params)
			reg.params = *params;
		
		Lock();

		if (m_data[key].insert(reg).second)
			policy->AddRef(handle);

		Release();
	}

	template <typename T>
	void Register(K & key, UInt32 type, T * classType, D * params = NULL)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

		EventRegistration<D> reg;
		reg.handle = policy->Create(type, (void *)classType);
		if (params)
			reg.params = *params;

#ifdef _DEBUG
		_MESSAGE("Executed PapyrusEvents::Register - %016llX", reg.handle);
#endif
		
		Lock();

		if (m_data[key].insert(reg).second)
			policy->AddRef(reg.handle);

		Release();
	}

	void Unregister(K & key, UInt64 handle)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

		EventRegistration<D> reg;
		reg.handle = handle;

		Lock();

		if (m_data[key].erase(reg))
			policy->Release(handle);

		Release();
	}

	template <typename T>
	void Unregister(K & key, UInt32 type, T * classType)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

		EventRegistration<D> reg;
		reg.handle = policy->Create(type, (void *)classType);

		Lock();

		if (m_data[key].erase(reg))
			policy->Release(reg.handle);

		Release();
	}

	void UnregisterAll(UInt64 handle)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

		EventRegistration<D> reg;
		reg.handle = handle;

		Lock();

		for (RegMap::iterator iter = m_data.begin(); iter != m_data.end(); ++iter)
			if (iter->second.erase(reg))
				policy->Release(handle);

		Release();
	}

	template <typename T>
	void UnregisterAll(UInt32 type, T * classType)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

		EventRegistration<D> reg;
		reg.handle = policy->Create(type, (void *)classType);

		Lock();

		for (RegMap::iterator iter = m_data.begin(); iter != m_data.end(); ++iter)
			if (iter->second.erase(reg))
				policy->Release(reg.handle);

		Release();
	}

	template <typename F>
	void ForEach(K & key, F & functor)
	{
		Lock();

		RegMap::iterator handles = m_data.find(key);

		if (handles != m_data.end())
			for (RegSet::iterator iter = handles->second.begin(); iter != handles->second.end(); ++iter)
				functor(*iter);

		Release();
	}

	void Clear(void)
	{
		Lock();
		m_data.clear();
		Release();
	}

	bool Save(SKSESerializationInterface * intfc, UInt32 type, UInt32 version)
	{
		intfc->OpenRecord(type, version);

		Lock();

		for (RegMap::iterator iter = m_data.begin(); iter != m_data.end(); ++iter)
		{
			UInt32 numRegs = iter->second.size();

			if (!numRegs)
				continue;

			intfc->OpenRecord('REGS', version);

			// Key
			Serialization::WriteData(intfc, &iter->first);
			// Reg count
			intfc->WriteRecordData(&numRegs, sizeof(numRegs));
			// Regs
			for (RegSet::iterator elems = iter->second.begin(); elems != iter->second.end(); ++elems)
				elems->Save(intfc, version);
		}

		intfc->OpenRecord('REGE', version);

		Release();

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

					for (UInt32 i=0; i<numRegs; i++)
					{
						EventRegistration<D> reg;
						if (reg.Load(intfc, version))
						{
							VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
							IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

							UInt64 newHandle = 0;

							// Skip if handle is no longer valid.
							if (! intfc->ResolveHandle(reg.handle, &newHandle))
								continue;

							reg.handle = newHandle;

							Lock();

							if (m_data[curKey].insert(reg).second)
								policy->AddRef(reg.handle);

							Release();
							
						}
						else
						{
							_MESSAGE("Error loading regs");
							return false;
						}
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
class RegistrationSetHolder : public SafeDataHolder<std::set<EventRegistration<D>>>
{
	typedef std::set<EventRegistration<D>>	RegSet;

public:

	void Register(UInt64 handle, D * params = NULL)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

		EventRegistration<D> reg;
		reg.handle = handle;
		if (params)
			reg.params = *params;
		
		Lock();

		if (m_data.insert(reg).second)
			policy->AddRef(handle);

		Release();
	}

	template <typename T>
	void Register(UInt32 type, T * classType, D * params = NULL)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

		EventRegistration<D> reg;
		reg.handle = policy->Create(type, (void *)classType);
		if (params)
			reg.params = *params;
		
		Lock();

		if (m_data.insert(reg).second)
			policy->AddRef(reg.handle);

		Release();
	}

	void Unregister(UInt64 handle)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

		EventRegistration<D> reg;
		reg.handle = handle;

		Lock();

		if (m_data.erase(reg))
			policy->Release(handle);

		Release();
	}

	template <typename T>
	void Unregister(UInt32 type, T * classType)
	{
		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

		EventRegistration<D> reg;
		reg.handle = policy->Create(type, (void *)classType);

		Lock();

		if (m_data.erase(reg))
			policy->Release(reg.handle);

		Release();
	}

	template <typename F>
	void ForEach(F & functor)
	{
		Lock();

		for (RegSet::iterator iter = m_data.begin(); iter != m_data.end(); ++iter)
			functor(*iter);

		Release();
	}

	void Clear(void)
	{
		Lock();
		m_data.clear();
		Release();
	}

	bool Save(SKSESerializationInterface * intfc, UInt32 type, UInt32 version)
	{
		intfc->OpenRecord(type, version);

		Lock();

		UInt32 numRegs = m_data.size();

		// Reg count
		intfc->WriteRecordData(&numRegs, sizeof(numRegs));
			
		// Regs
		for (RegSet::iterator iter = m_data.begin(); iter != m_data.end(); ++iter)
			iter->Save(intfc, version);

		Release();

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

		for (UInt32 i=0; i<numRegs; i++)
		{
			EventRegistration<D> reg;
			if (reg.Load(intfc, version))
			{
				VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
				IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();

				UInt64 newHandle = 0;

				// Skip if handle is no longer valid.
				if (! intfc->ResolveHandle(reg.handle, &newHandle))
					continue;

				reg.handle = newHandle;

				Lock();

				if (m_data.insert(reg).second)
					policy->AddRef(reg.handle);

				Release();
				
			}
			else
			{
				_MESSAGE("Error loading regs");
				return false;
			}
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
