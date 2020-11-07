#pragma once

#include "GameMenus.h"
#include "ScaleformCallbacks.h"
#include "ScaleformMovie.h"

#include "PapyrusObjects.h"

#include <string>
#include <vector>

struct StaticFunctionTag;
class VMClassRegistry;
template <typename T> class VMArray;

struct SKSESerializationInterface;

///
/// Helpers
///

template <typename T> void SetGFxValue(GFxValue * val, T arg);
template <> void SetGFxValue<bool> (GFxValue * val, bool arg);
template <> void SetGFxValue<float> (GFxValue * val, float arg);
template <> void SetGFxValue<UInt32> (GFxValue * val, UInt32 arg);
template <> void SetGFxValue<SInt32> (GFxValue * val, SInt32 arg);
template <> void SetGFxValue<BSFixedString> (GFxValue * val, BSFixedString arg);

template <typename T> T GetGFxValue(GFxValue * val);
template <> bool GetGFxValue<bool> (GFxValue * val);
template <> float GetGFxValue<float> (GFxValue * val);
template <> UInt32 GetGFxValue<UInt32> (GFxValue * val);
template <> SInt32 GetGFxValue<SInt32> (GFxValue * val);
template <> BSFixedString GetGFxValue<BSFixedString> (GFxValue * val);

///
/// UIDelegate
///

class UIDelegate : public ISKSEObject
{
public:
	virtual ~UIDelegate() {}
	
	virtual void Run() = 0;
};

///
/// UIInvokeDelegate
///

class UIInvokeDelegate : public UIDelegate
{
public:
	UIInvokeDelegate(const char* nameBuf, const char* targetBuf);
	explicit UIInvokeDelegate(SerializationTag tag);

	virtual const char* ClassName() const		{ return "UIInvokeDelegate"; }
	virtual UInt32		ClassVersion() const	{ return 1; }

	virtual bool Save(SKSESerializationInterface* intfc);
	virtual bool Load(SKSESerializationInterface* intfc, UInt32 version);

	virtual void Run();	

	std::vector<GFxValue>	args;

private:
	std::string		menuName_;
	std::string		target_;
};

///
/// UIInvokeFormDelegate
///

class UIInvokeFormDelegate : public UIDelegate
{
public:
	UIInvokeFormDelegate(const char* nameBuf, const char* targetBuf, UInt32 type, UInt64 handle);
	explicit UIInvokeFormDelegate(SerializationTag tag);

	virtual const char* ClassName() const		{ return "UIInvokeFormDelegate"; }
	virtual UInt32		ClassVersion() const	{ return 1; }

	virtual bool Save(SKSESerializationInterface* intfc);
	virtual bool Load(SKSESerializationInterface* intfc, UInt32 version);

	virtual void	Run(void);

private:
	std::string		menuName_;
	std::string		target_;

	UInt32			type_;
	UInt64			handle_;
};

///
/// Native functions
///
namespace papyrusUI
{
	// Template functions declared here for reference only
	template <typename T>
	void SetT(StaticFunctionTag* thisInput, BSFixedString menuName, BSFixedString targetStr, T value);

	template <typename T>
	T GetT(StaticFunctionTag* thisInput, BSFixedString menuName, BSFixedString sourceStr);

	template <typename T>
	void InvokeArgT(StaticFunctionTag* thisInput, BSFixedString menuName, BSFixedString targetStr, T arg);

	template <typename T>
	void InvokeArrayT(StaticFunctionTag* thisInput, BSFixedString menuName, BSFixedString targetStr, VMArray<T> args);

	bool IsMenuOpen(StaticFunctionTag* thisInput, BSFixedString menuName);

	void InvokeForm(StaticFunctionTag* thisInput, BSFixedString menuName, BSFixedString targetStr, TESForm * form);

	void OpenCustomMenu(StaticFunctionTag* thisInput, BSFixedString swfPath, SInt32 flags);
	void CloseCustomMenu(StaticFunctionTag* thisInput);

	void RegisterFuncs(VMClassRegistry* registry);
}