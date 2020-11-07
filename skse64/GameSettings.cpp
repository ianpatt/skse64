#include "GameSettings.h"
#include "GameAPI.h"

// 298D0E3E3DDD7D2333475E0B20F931E2ED6A0355+5A
RelocPtr<SettingCollectionList*> g_iniSettingCollection(0x0301D758);
// 5C8EA66C14C47204F60C72DE345DC6B22BB929B4+52
RelocPtr<SettingCollectionList*> g_iniPrefSettingCollection(0x02F6BA48);
// FE086A335E2EB844894F7D900623A149B381723F+6B
RelocPtr<SettingCollectionMap*>  g_gameSettingCollection(0x02EC58B0);

// ### we can just look these up
// D1E741E48E0181F5A2371C8E8BD02C1F0810BADF+0
RelocPtr<float> g_mapLocalHeight(0x01DB3ED8);	// fMapLocalHeight:MapMenu
// C7928A7F7ACB94729DA4D4011E03D5ADC79160F3+186
RelocPtr<float> g_localMapMinFrustumWidth(0x01DF5430);

UInt32 Setting::GetType(void) const
{
	if(!name || !name[0]) return kType_Unknown;

	switch(name[0])
	{
		case 'b':	return kType_Bool;
		case 'c':	return kType_Unknown;
		case 'h':	return kType_Unknown;
		case 'i':	return kType_Integer;
		case 'u':	return kType_Unknown;
		case 'f':	return kType_Float;
		case 'S':	return kType_String;	// dynamically allocated string
		case 's':	return kType_String;	// statically allocated string
		case 'r':	return kType_ID6;
		case 'a':	return kType_ID;
	}

	return kType_Unknown;
}

bool Setting::GetDouble(double * out) const
{
	switch(GetType())
	{
		case kType_Integer:	*out = data.s32; break;
		case kType_Float:	*out = data.f32; break;
		case kType_String:	return false;
		case kType_Bool:	*out = data.u8 ? 1 : 0; break;
		case kType_ID6:		*out = data.u32 >> 8; break;
		case kType_ID:		*out = data.u32; break;
		default:			return false;
		case kType_Unknown:	return false;
	}

	return true;
}

bool Setting::SetDouble(double value)
{
	switch(GetType())
	{
		case kType_Integer:	data.s32 = value; break;
		case kType_Float:	data.f32 = value; break;
		case kType_String:	return false;
		case kType_Bool:	data.u8 = value ? 1 : 0; break;
		case kType_ID6:		data.u32 = ((UInt32)value) << 8; break;
		case kType_ID:		data.u32 = value; break;
		default:			return false;
		case kType_Unknown:	return false;
	}

	return true;
}

char * FormHeap_Strdup(const char * src)
{
	UInt32	len = strlen(src) + 1;
	char	* result = (char *)Heap_Allocate(len);
	memcpy(result, src, len);

	return result;
}

bool Setting::SetString(const char * value)
{
	if(GetType() == kType_String)
	{
		if(name[0] == 'S')
			Heap_Free(data.s);

		data.s = FormHeap_Strdup(value);

		// mark string as dynamically allocated
		if(name[0] != 'S')
		{
			name = FormHeap_Strdup(name);
			name[0] = 'S';
		}

		return true;
	}
	else
	{
		return false;
	}
}

Setting	* SettingCollectionList::Get(const char * name)
{
	return CALL_MEMBER_FN(this, Get_Internal)(name);
}

Setting * SettingCollectionMap::Get(const char * name)
{
	return items.Get(name);
}

Setting * GetINISetting(const char * name)
{
	Setting	* setting = (*g_iniSettingCollection)->Get(name);
	if(!setting)
		setting = (*g_iniPrefSettingCollection)->Get(name);

	return setting;
}
