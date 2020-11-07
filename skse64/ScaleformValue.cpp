#include "skse64/ScaleformValue.h"

GFxValue::~GFxValue()
{
	CleanManaged();
}

void GFxValue::AddManaged(void)
{
	if (IsManaged())
		objectInterface->AddManaged_Internal(this, data.obj);
}

void GFxValue::CleanManaged(void)
{
	if (IsManaged())
	{
		objectInterface->ReleaseManaged_Internal(this, data.obj);

		objectInterface = NULL;
		type = kType_Undefined;
	}
}

void GFxValue::SetUndefined(void)
{
	CleanManaged();

	type = kType_Undefined;
}

void GFxValue::SetNull(void)
{
	CleanManaged();

	type = kType_Null;
}

void GFxValue::SetBool(bool value)
{
	CleanManaged();

	type = kType_Bool;
	data.boolean = value;
}

void GFxValue::SetNumber(double value)
{
	CleanManaged();

	type = kType_Number;
	data.number = value;
}

void GFxValue::SetString(const char * value)
{
	CleanManaged();

	type = kType_String;
	data.string = value;
}

void GFxValue::SetWideString(const wchar_t * value)
{
	CleanManaged();

	type = kType_WideString;
	data.wideString = value;
}

bool GFxValue::GetBool(void) const
{
	switch (GetType())
	{
	case kType_Bool: return data.boolean;
	case kType_Number: return data.number != 0;
	default: HALT("GFxValue::GetBool: bad type"); return false;
	}
}

const char * GFxValue::GetString(void) const
{
	if (GetType() != kType_String)
		return NULL;

	if (IsManaged())
		return *data.managedString;
	else
		return data.string;
}

const wchar_t * GFxValue::GetWideString(void) const
{
	if (GetType() != kType_WideString)
		return NULL;

	if (IsManaged())
		return *data.managedWideString;
	else
		return data.wideString;
}

double GFxValue::GetNumber(void) const
{
	switch (GetType())
	{
	case kType_Number:	return data.number;
	case kType_Bool:	return data.boolean ? 1 : 0;
	default:			HALT("GFxValue::GetNumber: bad type"); return 0;
	}
}

bool GFxValue::HasMember(const char * name)
{
	return objectInterface->HasMember(data.obj, name, IsDisplayObject());
}

bool GFxValue::SetMember(const char * name, GFxValue * value)
{
	return objectInterface->SetMember(data.obj, name, value, IsDisplayObject());
}

bool GFxValue::GetMember(const char * name, GFxValue * value)
{
	return objectInterface->GetMember(data.obj, name, value, IsDisplayObject());
}

bool GFxValue::DeleteMember(const char * name)
{
	return objectInterface->DeleteMember(data.obj, name, IsDisplayObject());
}

bool GFxValue::Invoke(const char * name, GFxValue * result, GFxValue * args, UInt32 numArgs)
{
	return objectInterface->Invoke(data.obj, result, name, args, numArgs, IsDisplayObject());
}

bool GFxValue::PushBack(GFxValue * value)
{
	return objectInterface->PushBack(data.obj, value);
}

UInt32 GFxValue::GetArraySize()
{
	return objectInterface->GetArraySize(data.obj);
}

bool GFxValue::GetElement(UInt32 index, GFxValue * value)
{
	return objectInterface->GetElement(data.obj, index, value);
}

bool GFxValue::GetDisplayInfo(DisplayInfo * displayInfo)
{
	return objectInterface->GetDisplayInfo(data.obj, displayInfo);
}

bool GFxValue::SetDisplayInfo(DisplayInfo * displayInfo)
{
	return objectInterface->SetDisplayInfo(data.obj, displayInfo);
}

bool GFxValue::SetText(const char * text, bool html)
{
	return objectInterface->SetText(data.obj, text, html);
}
