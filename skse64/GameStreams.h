#pragma once

#include "skse64_common/Utilities.h"

// class NiBinaryStream
// class NiFile : public NiBinaryStream
// class NiMemStream : public NiBinaryStream
// class OutputInfoStream : public NiBinaryStream
// class BSResourceNiBinaryStream : public NiBinaryStream
// class BSResourceStreamParser : public BSResourceNiBinaryStream
// class BSStreamParserData

// 20
class NiBinaryStream
{
public:
	virtual ~NiBinaryStream() { };

	virtual bool	IsValid(void) = 0;
	virtual void	Seek(SInt32 delta) = 0;
	virtual UInt32	GetOffset(void) { return 0; }

	//	void	** _vtbl;	// 00
	UInt32	unk08;		// 08
	UInt32	pad0C;		// 0C
	void	* readFn;	// 10
	void	* writeFn;	// 18
};

class NiMemStream : public NiBinaryStream
{
public:
	NiMemStream();
	virtual ~NiMemStream();

	virtual bool	IsValid(void) { return true; };
	virtual void	Seek(SInt32 delta);
	virtual UInt32	GetOffset(void);
	virtual void	CopyFrom(void * other);
	virtual void	FunctionSwap(bool swap);

	MEMBER_FN_PREFIX(NiMemStream);
	DEFINE_MEMBER_FN(Construct, NiMemStream *, 0x014F5980);
	DEFINE_MEMBER_FN(Destroy, void, 0x014F59F0);
	DEFINE_MEMBER_FN(Seek, void, 0x014F5A80, SInt32);
	// ??_7NiMemStream@@6B@ + 18
	DEFINE_MEMBER_FN(GetOffset, UInt32, 0x00CE4C60, void);
	DEFINE_MEMBER_FN(CopyFrom, UInt32, 0x014F5A50, void *);
	// ??_7NiMemStream@@6B@ + 28
	DEFINE_MEMBER_FN(FunctionSwap, UInt32, 0x014F5AC0, bool);

	void	* data;	// 20
	UInt32	offset;	// 28
	UInt32	unk2C;	// 2C
	UInt32	unk30;	// 30 - Inited to 0x400
	UInt8	flag1;	// 34
	UInt8	flag2;	// 35
	UInt16	pad36;	// 36
};

class BSResourceNiBinaryStream : public NiBinaryStream
{
public:
	BSResourceNiBinaryStream();
	BSResourceNiBinaryStream(const char * path);
	virtual ~BSResourceNiBinaryStream();

	// stub implementations
	virtual bool	IsValid(void);
	virtual void	Seek(SInt32 delta);
	virtual UInt32	GetOffset(void);
	virtual void	Unk_04(void) { }
	virtual void	EnableEndianSwap(bool swap);

	void	* unk10;	// 20
	void	* unk14;	// 28
	UInt64	offset;		// 30

	MEMBER_FN_PREFIX(BSResourceNiBinaryStream);
	DEFINE_MEMBER_FN(Construct, BSResourceNiBinaryStream *, 0x00D37C30, const char * filePath, UInt32 unk1, UInt32 unk2);	// unk1 = 0, unk2 = 0
	DEFINE_MEMBER_FN(ReadLine, UInt32, 0x00D38170, char * dst, UInt32 dstLen, UInt32 terminator);
	
	// see ??_7BSResourceNiBinaryStream@@6B@ ctor
	DEFINE_MEMBER_FN(Read, UInt32, 0x00D38360, void * dst, UInt32 len);
	DEFINE_MEMBER_FN(Write, UInt32, 0x00D383D0, void * src, UInt32 len);
	DEFINE_MEMBER_FN(Destroy, void, 0x00D37F10);

	DEFINE_MEMBER_FN(IsValid, bool, 0x00D38050, void);
	DEFINE_MEMBER_FN(Seek, void, 0x00D38060, SInt32);
	// ??_7BSResourceNiBinaryStream@@6B@ + 18
	DEFINE_MEMBER_FN(GetOffset, UInt32, 0x00D380A0, void);	// return offset;
	// ??_7BSResourceNiBinaryStream@@6B@ + 28
	DEFINE_MEMBER_FN(EnableEndianSwap, void, 0x00D38130, bool);

	UInt32 ReadLine(char * dst, UInt32 dstLen, UInt32 terminator);
	UInt32 ReadLine_w(wchar_t * dst, UInt32 dstLen, UInt32 terminator);	// length in characters
	UInt32 Read(void * dst, UInt32 len);
	UInt32 Write(void * src, UInt32 len);
};

STATIC_ASSERT(offsetof(BSResourceNiBinaryStream, readFn) == 0x10);
STATIC_ASSERT(offsetof(BSResourceNiBinaryStream, offset) == 0x30);
STATIC_ASSERT(sizeof(BSResourceNiBinaryStream) == 0x38);
