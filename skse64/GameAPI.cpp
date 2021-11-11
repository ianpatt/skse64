#include "skse64/GameAPI.h"

// CFBAE5ABDAD102C91A5CD32DE6F45A70A6EC7561+80
RelocPtr <Heap> g_mainHeap(0x01F57A80);

// 338B95DD01390309C6CF7F96A08D629A564D0A48+5D
RelocPtr <ConsoleManager *> g_console(0x02F9A800);

// D00D45B942A5CA8F9A6461459B627FB58388D055+44
RelocPtr <UInt32> g_consoleHandle(0x01F57A80);

// 3DD6796A3BFE73B35604DD47E8677156AEBD4360+20
RelocPtr <UInt32> g_TlsIndexPtr(0x03531398);

// 14BE6944D5E0E54959DE86FE65045CCEBBC65E31+6
RelocPtr <PlayerCharacter*> g_thePlayer(0x02FC19C8);

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
