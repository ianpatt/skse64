#pragma once

#include "skse64_common/Utilities.h"
#include "skse64/NiTypes.h"

// 18
class Setting
{
public:
	Setting();
	virtual ~Setting();

	enum
	{
		kType_Unknown = 0,
		kType_Integer,
		kType_Float,
		kType_String,
		kType_Bool,
		kType_ID6,	// need to find an example of this
		kType_ID,
	};

	union Data
	{
		UInt32	u32;
		SInt32	s32;
		float	f32;
		UInt8	u8;		// used for bool
		char	* s;
	};

//	void	** _vtbl;	// 00
	Data	data;		// 08
	char	* name;		// 10

	UInt32	GetType(void) const;

	bool	GetDouble(double * out) const;
	bool	SetDouble(double value);

	bool	SetString(const char * value);
};

// 128
class SettingCollectionList
{
public:
	virtual ~SettingCollectionList();

	struct Entry
	{
		Setting	* setting;
		Entry	* next;
	};

//	void	** _vtbl;	// 000
	UInt64	pad008[(0x118 - 0x008) >> 3];
	Entry	items;	// 118

	MEMBER_FN_PREFIX(SettingCollectionList);
	DEFINE_MEMBER_FN(Get_Internal, Setting *, 0x003271B0, const char * name);

	Setting	*	Get(const char * name);
};
STATIC_ASSERT(offsetof(SettingCollectionList, items) == 0x118);

// 138
class SettingCollectionMap
{
public:
	virtual ~SettingCollectionMap();

	Setting *	Get(const char * name);

//	void	** _vtbl;	// 000
	UInt64	pad008[(0x118 - 0x008) >> 3];
	NiTMap <char const *, Setting *>	items;	// 118 - actually BSTCaseInsensitiveStringMap but that only changes the virtual functions
};
STATIC_ASSERT(offsetof(SettingCollectionMap, items) == 0x118);

Setting * GetINISetting(const char * name);

extern RelocPtr<SettingCollectionList*> g_iniSettingCollection;
extern RelocPtr<SettingCollectionList*> g_iniPrefSettingCollection;
extern RelocPtr<SettingCollectionMap*>  g_gameSettingCollection;

extern RelocPtr<float> g_mapLocalHeight;
extern RelocPtr<float> g_localMapMinFrustumWidth;
