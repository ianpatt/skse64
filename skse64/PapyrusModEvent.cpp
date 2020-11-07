#include "PapyrusModEvent.h"

#include <vector>

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

#include "PapyrusEvents.h"
#include "PapyrusObjects.h"
#include "PapyrusArgs.h"

///
/// PapyrusModEvent
///

namespace
{
	class PapyrusModEvent :
		public ISKSEObject,
		public IFunctionArguments
	{
	private:
		struct Argument
		{
			enum
			{
				kType_None =		0,
				kType_Int =			1,
				kType_Float =		2,
				kType_Bool =		3,
				kType_String =		4,
				kType_Form =		5
			};

			Argument() : type(kType_None) {}

			explicit Argument(SInt32 arg) : type(kType_Int)				{ data.i = arg; }
			explicit Argument(float arg) : type(kType_Float)			{ data.f = arg; }
			explicit Argument(bool arg) : type(kType_Bool)				{ data.b = arg; }
			explicit Argument(BSFixedString arg) : type(kType_String)	{ data.str = arg.data; }
			explicit Argument(TESForm* arg) : type(kType_Form)			{ data.form = arg; }

			UInt32	type;

			union Data
			{
				SInt32		i;
				float		f;
				bool		b;
				const char*	str;
				TESForm*	form;
			}
			data;
		};

	public:
		typedef std::vector<Argument>	ArgVectorT;

		explicit PapyrusModEvent(BSFixedString name) :
			eventName_( name )
		{}

		explicit PapyrusModEvent(SerializationTag)
		{}

		virtual ~PapyrusModEvent()
		{
			// LOL
			CALL_MEMBER_FN(&eventName_, Release)();
		}

		// ISKSEObject
		virtual const char* ClassName() const		{ return "PapyrusModEvent"; }
		virtual UInt32		ClassVersion() const	{ return 1; }

		virtual bool Save(SKSESerializationInterface* intfc);
		virtual bool Load(SKSESerializationInterface* intfc, UInt32 version);
		// ~ISKSEObject

		// IFunctionArguments
		virtual bool Copy(Output * dst)
		{
			VMClassRegistry * registry = (*g_skyrimVM)->GetClassRegistry();

			UInt32 i = 0;
			//dst->Resize(1 + args_.size());
			dst->Resize(args_.size());

			//dst->Get(i++)->SetString(eventName_.data);

			for (ArgVectorT::iterator it = args_.begin(); it != args_.end(); ++it, ++i)
			{
				switch (it->type)
				{
				case Argument::kType_Int:
					dst->Get(i)->SetInt(it->data.i);
					break;
				case Argument::kType_Float:
					dst->Get(i)->SetFloat(it->data.f);
					break;
				case Argument::kType_Bool:
					dst->Get(i)->SetBool(it->data.b);
					break;
				case Argument::kType_String:
					dst->Get(i)->SetString(it->data.str);
					break;
				case Argument::kType_Form:
					PackValue(dst->Get(i), &it->data.form, registry);
					break;
				};
			}

			return true;
		}
		// ~IFunctionArguments

		template <typename T>
		void PushArg(T arg)
		{
			args_.push_back(Argument(arg));
		}

		void operator() (const EventRegistration<ModCallbackParameters>& reg)
		{
			VMClassRegistry * registry = (*g_skyrimVM)->GetClassRegistry();
			registry->QueueEvent(reg.handle, &reg.params.callbackName, this);
		}

		BSFixedString	EventName() const	{ return eventName_; }

	private:
		BSFixedString			eventName_;
		std::vector<Argument>	args_;
	};

	bool PapyrusModEvent::Save(SKSESerializationInterface* intfc)
	{
		using namespace Serialization;

		if (! WriteData(intfc, &eventName_))
			return false;

		UInt32 argsSize = args_.size();
		if (! WriteData(intfc, &argsSize))
			return false;

		for (UInt32 i=0; i<argsSize; i++)
		{
			Argument& e = args_[i];
			UInt32 type = e.type;
			if (! WriteData(intfc, &type))
				return false;

			switch (type)
			{
			case Argument::kType_Int:
				if (! WriteData(intfc, &e.data.i))
					return false;
				break;
			case Argument::kType_Float:
				if (! WriteData(intfc, &e.data.f))
					return false;
				break;
			case Argument::kType_Bool:
				if (! WriteData(intfc, &e.data.b))
					return false;
				break;
			case Argument::kType_String:
				if (! WriteData(intfc, &e.data.str))
					return false;
				break;
			case Argument::kType_Form:
				if (! WriteData(intfc, &e.data.form->formID))
					return false;
				break;
			}
		}

		return true;
	}

	bool PapyrusModEvent::Load(SKSESerializationInterface* intfc, UInt32 version)
	{
		using namespace Serialization;

		if (! ReadData(intfc, &eventName_))
			return false;

		UInt32 argsSize;
		if (! ReadData(intfc, &argsSize))
			return false;

		for (UInt32 i=0; i<argsSize; i++)
		{
			Argument e;
			if (! ReadData(intfc, &e.type))
				return false;

			switch (e.type)
			{
			case Argument::kType_Int:
				if (! ReadData(intfc, &e.data.i))
					return false;
				break;
			case Argument::kType_Float:
				if (! ReadData(intfc, &e.data.f))
					return false;
				break;
			case Argument::kType_Bool:
				if (! ReadData(intfc, &e.data.b))
					return false;
				break;
			case Argument::kType_String:
				if (! ReadData(intfc, &e.data.str))
					return false;
				break;
			case Argument::kType_Form:
			{
				UInt32 formId;
				if (! ReadData(intfc, &formId))
					return false;

				UInt32 fixedFormId;
				if (intfc->ResolveFormId(formId, &fixedFormId))
					formId = fixedFormId;
				
				e.data.form = LookupFormByID(formId);
				break;
			}
			}

			args_.push_back(e);
		}

		return true;
	}
}

///
/// Native functions
///

namespace papyrusModEvent
{
	SInt32 Create(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* thisInput, BSFixedString eventName)
	{
		ERROR_AND_RETURN_0_IF(!eventName.data, "Event name must not be empty.", registry, stackId)

		PapyrusModEvent* evn = new PapyrusModEvent(eventName);

		SInt32 handle = SKSEObjectStorageInstance().StoreObject(evn, stackId);
		return handle;
	}

	bool Send(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* thisInput, SInt32 handle)
	{
		ERROR_AND_RETURN_0_IF(handle <= 0, "Invalid handle.", registry, stackId)

		PapyrusModEvent* evn = SKSEObjectStorageInstance().TakeObject<PapyrusModEvent>(handle);
		
		ERROR_AND_RETURN_0_IF(evn == NULL, "Failed to lookup object for given handle.", registry, stackId)

		g_modCallbackRegs.ForEach(evn->EventName(), *evn);

		// Stuff has been copied, we can delete this
		delete evn;

		return true;
	}

	void Release(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* thisInput, SInt32 handle)
	{
		ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId)

		PapyrusModEvent* evn = SKSEObjectStorageInstance().TakeObject<PapyrusModEvent>(handle);
		
		ERROR_AND_RETURN_IF(evn == NULL, "Failed to lookup object for given handle.", registry, stackId)

		delete evn;
	}

	template <typename T>
	void Push(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag* thisInput, SInt32 handle, T arg)
	{
		ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId)

		PapyrusModEvent* evn = SKSEObjectStorageInstance().AccessObject<PapyrusModEvent>(handle);
		
		ERROR_AND_RETURN_IF(evn == NULL, "Failed to lookup object for given handle.", registry, stackId)
		
		evn->PushArg<T>(arg);
	}
}

void papyrusModEvent::RegisterFuncs(VMClassRegistry* registry)
{
	SKSEObjectRegistry& skseObjRegistry = SKSEObjectRegistryInstance();
	skseObjRegistry.RegisterClass<PapyrusModEvent>();

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, SInt32, BSFixedString> ("Create", "ModEvent", papyrusModEvent::Create, registry));
	
	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, bool, SInt32> ("Send", "ModEvent", papyrusModEvent::Send, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, SInt32> ("Release", "ModEvent", papyrusModEvent::Release, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, SInt32, bool> ("PushBool", "ModEvent", papyrusModEvent::Push<bool>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, SInt32, SInt32> ("PushInt", "ModEvent", papyrusModEvent::Push<SInt32>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, SInt32, float> ("PushFloat", "ModEvent", papyrusModEvent::Push<float>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, SInt32, BSFixedString> ("PushString", "ModEvent", papyrusModEvent::Push<BSFixedString>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, SInt32, TESForm*> ("PushForm", "ModEvent", papyrusModEvent::Push<TESForm*>, registry));

	registry->SetFunctionFlags("ModEvent", "Create", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ModEvent", "Send", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ModEvent", "Release", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ModEvent", "PushBool", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ModEvent", "PushInt", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ModEvent", "PushFloat", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ModEvent", "PushString", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ModEvent", "PushForm", VMClassRegistry::kFunctionFlag_NoWait);
}