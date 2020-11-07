#include "GameStreams.h"

BSResourceNiBinaryStream::BSResourceNiBinaryStream()
{
	//
}

BSResourceNiBinaryStream::BSResourceNiBinaryStream(const char * path)
{
	CALL_MEMBER_FN(this, Construct)(path, 0, 0);
}

BSResourceNiBinaryStream::~BSResourceNiBinaryStream()
{
	CALL_MEMBER_FN(this, Destroy)();
}

bool BSResourceNiBinaryStream::IsValid(void)
{
	return CALL_MEMBER_FN(this, IsValid)();
}

void BSResourceNiBinaryStream::Seek(SInt32 delta)
{
	CALL_MEMBER_FN(this, Seek)(delta);
}

UInt32 BSResourceNiBinaryStream::GetOffset(void)
{
	return CALL_MEMBER_FN(this, GetOffset)();
}

void BSResourceNiBinaryStream::EnableEndianSwap(bool swap)
{
	CALL_MEMBER_FN(this, EnableEndianSwap)(swap);
}

UInt32 BSResourceNiBinaryStream::ReadLine(char * dst, UInt32 dstLen, UInt32 terminator)
{
	return CALL_MEMBER_FN(this, ReadLine)(dst, dstLen, terminator);
}

UInt32 BSResourceNiBinaryStream::ReadLine_w(wchar_t * dst, UInt32 dstLen, UInt32 terminator)
{
	wchar_t	* iter = dst;

	if(dstLen == 0)
		return 0;

	for(UInt32 i = 0; i < dstLen - 1; i++)
	{
		wchar_t	data;

		if(Read(&data, sizeof(data)) != sizeof(data))
			break;

		if(data == terminator)
			break;

		*iter++ = data;
	}

	// null terminate
	*iter = 0;

	return iter - dst;
}

UInt32 BSResourceNiBinaryStream::Read(void * dst, UInt32 len)
{
	return CALL_MEMBER_FN(this, Read)(dst, len);
}

UInt32 BSResourceNiBinaryStream::Write(void * src, UInt32 len)
{
	return CALL_MEMBER_FN(this, Write)(src, len);
}

NiMemStream::NiMemStream()
{
	CALL_MEMBER_FN(this, Construct)();
}

NiMemStream::~NiMemStream()
{
	CALL_MEMBER_FN(this, Destroy)();
}

void NiMemStream::Seek(SInt32 delta)
{
	CALL_MEMBER_FN(this, Seek)(delta);
}
UInt32 NiMemStream::GetOffset(void)
{
	return CALL_MEMBER_FN(this, GetOffset)();
}
void NiMemStream::CopyFrom(void * other)
{
	CALL_MEMBER_FN(this, CopyFrom)(other);
}
void NiMemStream::FunctionSwap(bool swap)
{
	CALL_MEMBER_FN(this, FunctionSwap)(swap);
}
