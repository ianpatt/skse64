#include "PapyrusUI.h"
#include "PapyrusVM.h"
#include "ScaleformCallbacks.h"
#include "ScaleformMovie.h"
#include "ScaleformAPI.h"
#include "ScaleformExtendedData.h"
#include "GameMenus.h"
#include "GameEvents.h"
#include "GameForms.h"
#include "GameInput.h"
#include "CustomMenu.h"
#include <float.h>

#include "Serialization.h"

template <> void SetGFxValue<bool> (GFxValue * val, bool arg)						{ val->SetBool(arg); }
template <> void SetGFxValue<float> (GFxValue * val, float arg)						{ val->SetNumber(arg); }
template <> void SetGFxValue<UInt32> (GFxValue * val, UInt32 arg)					{ val->SetNumber(arg); }
template <> void SetGFxValue<SInt32> (GFxValue * val, SInt32 arg)					{ val->SetNumber(arg); }
template <> void SetGFxValue<BSFixedString> (GFxValue * val, BSFixedString arg)
{
	// lifetime of this string will not be managed by the scaleform runtime
	val->SetString(arg.data);
}

template <> bool GetGFxValue<bool> (GFxValue * val)						{ return (val->GetType() == GFxValue::kType_Bool ? val->GetBool() : false); }
template <> float GetGFxValue<float> (GFxValue * val)					{ return (val->GetType() == GFxValue::kType_Number ? val->GetNumber() : 0); }
template <> UInt32 GetGFxValue<UInt32> (GFxValue * val)					{ return (val->GetType() == GFxValue::kType_Number ? (UInt32)val->GetNumber() : 0); }
template <> SInt32 GetGFxValue<SInt32> (GFxValue * val)					{ return (val->GetType() == GFxValue::kType_Number ? (SInt32)val->GetNumber() : 0); }
template <> BSFixedString GetGFxValue<BSFixedString> (GFxValue * val)
{
	return (val->GetType() == GFxValue::kType_String ? BSFixedString(val->GetString()) : BSFixedString());
}

///
/// UIInvokeDelegate
///

UIInvokeDelegate::UIInvokeDelegate(const char * nameBuf, const char * targetBuf) :
	menuName_( nameBuf ),
	target_( targetBuf )
{}

UIInvokeDelegate::UIInvokeDelegate(SerializationTag tag)
{}

void UIInvokeDelegate::Run()
{
	MenuManager * mm = MenuManager::GetSingleton();
	if (!mm)
		return;

	BSFixedString t(menuName_.c_str());
	GFxMovieView * view = mm->GetMovieView(&t);
	if (!view)
		return;

	GFxValue * value = NULL;
	if (args.size() > 0)
		value = &args[0];

	view->Invoke(target_.c_str(), NULL, value, args.size());
}

bool UIInvokeDelegate::Save(SKSESerializationInterface* intfc)
{
	using namespace Serialization;

	if (! WriteData(intfc, &menuName_))
		return false;

	if (! WriteData(intfc, &target_))
		return false;

	UInt32 argsSize = args.size();

	if (! WriteData(intfc, &argsSize))
		return false;

	for (UInt32 i=0; i<argsSize; i++)
		if (! WriteData(intfc, &args[i]))
			return false;

	return true;
}

bool UIInvokeDelegate::Load(SKSESerializationInterface* intfc, UInt32 version)
{
	using namespace Serialization;

	if (! ReadData(intfc, &menuName_))
		return false;

	if (! ReadData(intfc, &target_))
		return false;

	UInt32 argsSize;;

	if (! ReadData(intfc, &argsSize))
		return false;

	args.resize(argsSize);

	for (UInt32 i=0; i<argsSize; i++)
		if (! ReadData(intfc, &args[i]))
			return false;

	return true;
}

///
/// UIInvokeFormDelegate
///

UIInvokeFormDelegate::UIInvokeFormDelegate(const char * nameBuf, const char * targetBuf, UInt32 type, UInt64 handle) :
	menuName_( nameBuf ),
	target_( targetBuf ),
	type_( type ),
	handle_( handle )
{}

UIInvokeFormDelegate::UIInvokeFormDelegate(SerializationTag tag)
{}

void UIInvokeFormDelegate::Run()
{
	VMClassRegistry*		registry =	(*g_skyrimVM)->GetClassRegistry();
	IObjectHandlePolicy*	policy =	registry->GetHandlePolicy();
	
	TESForm* form = (TESForm*) policy->Resolve(type_, handle_);
	if (!form)
		return;

	MenuManager* mm = MenuManager::GetSingleton();
	if (!mm)
		return;

	BSFixedString t(menuName_.c_str());
	GFxMovieView * view = mm->GetMovieView(&t);
	if (!view)
		return;

	GFxValue args;
	view->CreateObject(&args);
	scaleformExtend::FormData(&args, view, form, false, false);

	view->Invoke(target_.c_str(), NULL, &args, 1);
}

bool UIInvokeFormDelegate::Save(SKSESerializationInterface* intfc)
{
	using namespace Serialization;

	if (! WriteData(intfc, &menuName_))
		return false;

	if (! WriteData(intfc, &target_))
		return false;

	if (! WriteData(intfc, &type_))
		return false;

	if (! WriteData(intfc, &handle_))
		return false;

	return true;
}

bool UIInvokeFormDelegate::Load(SKSESerializationInterface* intfc, UInt32 version)
{
	using namespace Serialization;

	if (! ReadData(intfc, &menuName_))
		return false;

	if (! ReadData(intfc, &target_))
		return false;

	if (! ReadData(intfc, &type_))
		return false;

	if (! ReadData(intfc, &handle_))
		return false;

	UInt64 fixedHandle;
	if (intfc->ResolveHandle(handle_, &fixedHandle))
		handle_ = fixedHandle;

	return true;
}

///
/// Native functions
///
namespace papyrusUI
{
	template <typename T>
	void SetT(StaticFunctionTag* thisInput, BSFixedString menuName, BSFixedString targetStr, T value)
	{
		if (!menuName.data || !targetStr.data)
			return;

		MenuManager * mm = MenuManager::GetSingleton();
		if (!mm)
			return;

		GFxMovieView * view = mm->GetMovieView(&menuName);
		if (!view)
			return;

		GFxValue fxValue;
		SetGFxValue<T>(&fxValue, value);

		view->SetVariable(targetStr.data, &fxValue, 1);
	}

	template <typename T>
	T GetT(StaticFunctionTag* thisInput, BSFixedString menuName, BSFixedString sourceStr)
	{
		if (!menuName.data || !sourceStr.data)
			return 0;

		MenuManager * mm = MenuManager::GetSingleton();
		if (!mm)
			return 0;

		GFxMovieView * view = mm->GetMovieView(&menuName);
		if (!view)
			return 0;

		GFxValue fxResult;
		if (! view->GetVariable(&fxResult, sourceStr.data))
			return 0;

		return GetGFxValue<T>(&fxResult);
	}

	template <typename T>
	void InvokeArgT(StaticFunctionTag* thisInput, BSFixedString menuName, BSFixedString targetStr, T arg)
	{
		if (!menuName.data || !targetStr.data)
			return;

		UIManager* uiManager = UIManager::GetSingleton();
		if (!uiManager)
			return;

		UIInvokeDelegate* cmd = new UIInvokeDelegate(menuName.data, targetStr.data);

		cmd->args.resize(1);
		SetGFxValue<T>(&cmd->args[0], arg);

		uiManager->QueueCommand(cmd);
	}

	template <typename T>
	void InvokeArrayT(StaticFunctionTag* thisInput, BSFixedString menuName, BSFixedString targetStr, VMArray<T> args)
	{
		if (!menuName.data || !targetStr.data)
			return;

		UIManager* uiManager = UIManager::GetSingleton();
		if (!uiManager)
			return;

		UInt32 argCount = args.Length();

		UIInvokeDelegate* cmd = new UIInvokeDelegate(menuName.data, targetStr.data);
		
		cmd->args.resize(argCount);
		for (UInt32 i=0; i<argCount; i++)
		{
			T arg;
			args.Get(&arg, i);
			SetGFxValue<T>(&cmd->args[i], arg);
		}

		uiManager->QueueCommand(cmd);
	}

	void InvokeForm(StaticFunctionTag* thisInput, BSFixedString menuName, BSFixedString targetStr, TESForm * form)
	{
		if (!form || !menuName.data || !targetStr.data)
			return;

		UIManager * uiManager = UIManager::GetSingleton();
		if (!uiManager)
			return;

		VMClassRegistry		* registry =	(*g_skyrimVM)->GetClassRegistry();
		IObjectHandlePolicy	* policy =		registry->GetHandlePolicy();
		
		UInt64 handle = policy->Create(form->formType, form);
		if (handle == policy->GetInvalidHandle())
			return;

		UIInvokeFormDelegate * cmd = new UIInvokeFormDelegate(
			menuName.data, targetStr.data, form->formType, handle);

		uiManager->QueueCommand(cmd);
	}

	bool IsMenuOpen(StaticFunctionTag* thisInput, BSFixedString menuName)
	{
		if (!menuName.data)
			return 0;

		MenuManager * mm = MenuManager::GetSingleton();
		if (!mm)
			return false;

		return mm->IsMenuOpen(&menuName);
	}

	bool IsTextInputEnabled(StaticFunctionTag * thisInput)
	{
		InputManager	* inputManager = InputManager::GetSingleton();

		if(!inputManager) return false;

		return inputManager->allowTextInput != 0;
	}

	void OpenCustomMenu(StaticFunctionTag* thisInput, BSFixedString swfPath, SInt32 flags)
	{
		CustomMenuCreator::SetSwfPath(swfPath.data);

		BSFixedString s("CustomMenu");
		CALL_MEMBER_FN(UIManager::GetSingleton(), AddMessage)(&s, UIMessage::kMessage_Open, NULL);
	}

	void CloseCustomMenu(StaticFunctionTag* thisInput)
	{
		BSFixedString s("CustomMenu");
		CALL_MEMBER_FN(UIManager::GetSingleton(), AddMessage)(&s, UIMessage::kMessage_Close, NULL);
	}
};

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusUI::RegisterFuncs(VMClassRegistry* registry)
{
	SKSEObjectRegistry& skseObjRegistry = SKSEObjectRegistryInstance();
	skseObjRegistry.RegisterClass<UIInvokeDelegate>();
	skseObjRegistry.RegisterClass<UIInvokeFormDelegate>();

	MenuManager * mm = MenuManager::GetSingleton();
	if (mm)
		mm->Register("CustomMenu", CustomMenuCreator::Create);

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, bool> ("SetBool", "UI", papyrusUI::SetT<bool>, registry));
	
	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, UInt32> ("SetInt", "UI", papyrusUI::SetT<UInt32>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, float> ("SetFloat", "UI", papyrusUI::SetT<float>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, BSFixedString> ("SetString", "UI", papyrusUI::SetT<BSFixedString>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, bool, BSFixedString, BSFixedString> ("GetBool", "UI", papyrusUI::GetT<bool>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, UInt32, BSFixedString, BSFixedString> ("GetInt", "UI", papyrusUI::GetT<UInt32>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, float, BSFixedString, BSFixedString> ("GetFloat", "UI", papyrusUI::GetT<float>, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, BSFixedString, BSFixedString, BSFixedString> ("GetString", "UI", papyrusUI::GetT<BSFixedString>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, bool> ("InvokeBool", "UI", papyrusUI::InvokeArgT<bool>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, float> ("InvokeFloat", "UI", papyrusUI::InvokeArgT<float>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, UInt32> ("InvokeInt", "UI", papyrusUI::InvokeArgT<UInt32>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, BSFixedString> ("InvokeString", "UI", papyrusUI::InvokeArgT<BSFixedString>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, VMArray<bool>> ("InvokeBoolA", "UI", papyrusUI::InvokeArrayT<bool>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, VMArray<UInt32>> ("InvokeIntA", "UI", papyrusUI::InvokeArrayT<UInt32>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, VMArray<float>> ("InvokeFloatA", "UI", papyrusUI::InvokeArrayT<float>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, VMArray<BSFixedString>> ("InvokeStringA", "UI", papyrusUI::InvokeArrayT<BSFixedString>, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, void, BSFixedString, BSFixedString, TESForm*> ("InvokeForm", "UI", papyrusUI::InvokeForm, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, bool, BSFixedString> ("IsMenuOpen", "UI", papyrusUI::IsMenuOpen, registry));

	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, bool> ("IsTextInputEnabled", "UI", papyrusUI::IsTextInputEnabled, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, void, BSFixedString, SInt32> ("OpenCustomMenu", "UI", papyrusUI::OpenCustomMenu, registry));

	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, void> ("CloseCustomMenu", "UI", papyrusUI::CloseCustomMenu, registry));

	registry->SetFunctionFlags("UI", "InvokeBool", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "InvokeInt", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "InvokeFloat", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "InvokeString", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "InvokeBoolA", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "InvokeIntA", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "InvokeFloatA", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "InvokeStringA", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "InvokeForm", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "IsTextInputEnabled", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "OpenCustomMenu", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("UI", "CloseCustomMenu", VMClassRegistry::kFunctionFlag_NoWait);
}
