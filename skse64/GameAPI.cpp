#include "skse64/GameAPI.h"

// 4371A3D33EB5D62C7C548EBD5F760AF15A9B0B96+41
RelocPtr <Heap> g_mainHeap(0x01EBD280);

// 75643FD50A96D1F83B1AC4797EC84EF1C53039AC+68
RelocPtr <ConsoleManager *> g_console(0x02F000F0);

// E1E59B64FDA5B8A9085AE9314353ABEEA0DB2823+C4
RelocPtr <UInt32> g_consoleHandle(0x02F4C31C);

// 52DD97B7C619EA732D3CD95637F449FC7A23DD12+24
RelocPtr<UInt32> g_TlsIndexPtr(0x03497408);

// BC8BF08A45C960EB35F2BAFEC9432C80365A1473+14A
RelocPtr<PlayerCharacter*> g_thePlayer(0x02F26EF8);

void * Heap_Allocate(size_t size)
{
	return CALL_MEMBER_FN(g_mainHeap, Allocate)(size, 0, false);
}

void Heap_Free(void * ptr)
{
	CALL_MEMBER_FN(g_mainHeap, Free)(ptr, false);
}

void Console_Print(const char * fmt, ...)
{
	ConsoleManager * mgr = *g_console;
	if(mgr)
	{
		va_list args;
		va_start(args, fmt);

		CALL_MEMBER_FN(mgr, VPrint)(fmt, args);

		va_end(args);
	}
}

struct TLSData
{
	// thread local storage

	UInt8	unk000[0x600];	// 000
	UInt8	consoleMode;	// 600
	UInt8	pad601[7];		// 601
};

static TLSData * GetTLSData()
{
	UInt32 TlsIndex = *g_TlsIndexPtr;
	TLSData ** data = (TLSData **)__readgsqword(0x58);

	return data[TlsIndex];
}


bool IsConsoleMode(void)
{
	return GetTLSData()->consoleMode != 0;
}

__int64 GetPerfCounter(void)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return li.QuadPart;
}
