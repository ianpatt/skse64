#pragma once

#include "skse64_common/Utilities.h"
#include "skse64/NiTypes.h"

class NiBinaryStream;
class NiObject;

// 620+
class NiStream
{
public:
	NiStream();
	virtual ~NiStream();

	virtual bool	LoadStream(NiBinaryStream * stream);
	virtual bool	LoadBuf(char * buf, UInt32 len);
	virtual bool	LoadPath(const char * path);
	virtual bool	SaveStream(NiBinaryStream * stream);
	virtual bool	SaveBuf(char ** buf, UInt32 * len);
	virtual bool	SavePath(const char * path);

	MEMBER_FN_PREFIX(NiStream);
	DEFINE_MEMBER_FN(ctor, NiStream *, 0x00C59690);
	DEFINE_MEMBER_FN(dtor, void, 0x00C598F0);
	DEFINE_MEMBER_FN(AddObject, void, 0x00C59AE0, NiObject * object);

//	void	** _vtbl;								// 000
	UInt64	pad004[(0x218 - 0x008) >> 3];			// 008
	NiTLargeArray <NiObject *>	m_objects;			// 218
	NiTLargeArray <UInt32>		m_objectSizes;		// 238
	NiTLargeArray <NiObject *>	m_rootObjects;		// 258
	NiTLargeArray <char *>		m_stringTable;		// 278
	UInt64						unk298;				// 298
	UInt64						unk2A0;				// 2A0
	UInt64						unk2A8;				// 2A8
	NiTMap<NiObject*, UInt32>	m_pointerMap;		// 2B0
	UInt64						unk2D0[(0x300 - 0x2D0) >> 3]; // 2D0
	UInt32						unk300;				// 300
	SInt32						unk304;				// 304
	UInt32						unk308;				// 308
	char						unk30C[MAX_PATH];	// 30C
	UInt32						unk410;
	UInt32						unk414;
	UInt64						unk418[(0x518 - 0x418) >> 3]; // 418
	char						unk518[MAX_PATH];	// 518
};
STATIC_ASSERT(offsetof(NiStream, m_objects) == 0x218);
STATIC_ASSERT(offsetof(NiStream, m_objectSizes) == 0x238);
STATIC_ASSERT(offsetof(NiStream, m_rootObjects) == 0x258);
STATIC_ASSERT(offsetof(NiStream, m_stringTable) == 0x278);

// 638
class BSStream : public NiStream
{
public:
	MEMBER_FN_PREFIX(BSStream);
	DEFINE_MEMBER_FN(dtor, void, 0x0014CC20);

	UInt64	unk620;
	UInt64	unk628;
	UInt64	unk630;
};

class DeepCopyStream : public NiStream
{
public:
	DeepCopyStream();
	virtual ~DeepCopyStream();

	// vtbl 001117394

	MEMBER_FN_PREFIX(DeepCopyStream);
	DEFINE_MEMBER_FN(SaveStream, bool, 0x00C59F10, char ** buffer, UInt32 * length);
	DEFINE_MEMBER_FN(LoadStream, bool, 0x00C59EC0, char * buffer, UInt32 length);
};

