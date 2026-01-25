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
	DEFINE_MEMBER_FN(Construct, NiMemStream *, 0x013120B0);
	DEFINE_MEMBER_FN(Destroy, void, 0x01312120);
	DEFINE_MEMBER_FN(Seek, void, 0x013121B0, SInt32);
	// ??_7NiMemStream@@6B@ + 18
	DEFINE_MEMBER_FN(GetOffset, UInt32, 0x00C25660, void);
	DEFINE_MEMBER_FN(CopyFrom, UInt32, 0x01312180, void *);
	// ??_7NiMemStream@@6B@ + 28
	DEFINE_MEMBER_FN(FunctionSwap, UInt32, 0x013121F0, bool);

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
	DEFINE_MEMBER_FN(Construct, BSResourceNiBinaryStream *, 0x00C76060, const char * filePath, UInt32 unk1, UInt32 unk2);	// unk1 = 0, unk2 = 0
	DEFINE_MEMBER_FN(ReadLine, UInt32, 0x00C765C0, char * dst, UInt32 dstLen, UInt32 terminator);
	
	// SE note: it looks like the Read and Write function got inlined? 
	// Anyway, the readFn and writeFn assigned in ctor are equivalent (in Skyrim32 one just calls the other)
	// readFn(object, dst, len) -> object.Read(dst, len), in x64, the parameters match.
	// E93F9EA10EEEC2FC86D868E75F28A023D7453255+51
	DEFINE_MEMBER_FN(Read, UInt32, 0x00C767B0, void * dst, UInt32 len);
	// E93F9EA10EEEC2FC86D868E75F28A023D7453255+5C
	DEFINE_MEMBER_FN(Write, UInt32, 0x00C76820, void * src, UInt32 len);
	DEFINE_MEMBER_FN(Destroy, void, 0x00C76340);

	DEFINE_MEMBER_FN(IsValid, bool, 0x00C76480, void);
	DEFINE_MEMBER_FN(Seek, void, 0x00C76490, SInt32);
	// ??_7BSResourceNiBinaryStream@@6B@ + 18
	DEFINE_MEMBER_FN(GetOffset, UInt32, 0x00C764E0, void);	// return offset;
	// ??_7BSResourceNiBinaryStream@@6B@ + 28
	DEFINE_MEMBER_FN(EnableEndianSwap, void, 0x00C76580, bool);

	UInt32 ReadLine(char * dst, UInt32 dstLen, UInt32 terminator);
	UInt32 ReadLine_w(wchar_t * dst, UInt32 dstLen, UInt32 terminator);	// length in characters
	UInt32 Read(void * dst, UInt32 len);
	UInt32 Write(void * src, UInt32 len);
};

STATIC_ASSERT(offsetof(BSResourceNiBinaryStream, readFn) == 0x10);
STATIC_ASSERT(offsetof(BSResourceNiBinaryStream, offset) == 0x30);
STATIC_ASSERT(sizeof(BSResourceNiBinaryStream) == 0x38);
