#include "skse64/GameAPI.h"

// 4265A365BEE4B4196639CF9FCCF3C445CEC4DBFD+1B
RelocPtr <Main*> g_main(0x031886E8);

// B05BDE2B512930D17FC7064CCC201FEE897AA475+44
RelocPtr <Heap> g_mainHeap(0x035F23C0);

// 338B95DD01390309C6CF7F96A08D629A564D0A48+5D
RelocPtr <ConsoleManager *> g_console(0x031392E0);

// 4D7631B98BE6AABD42229719E0D837E677A24FB5+CF
RelocPtr <UInt32> g_consoleHandle(0x031AE194);

// 3DD6796A3BFE73B35604DD47E8677156AEBD4360+20
RelocPtr <UInt32> g_TlsIndexPtr(0x035F1A58);

// 14BE6944D5E0E54959DE86FE65045CCEBBC65E31+6
RelocPtr <PlayerCharacter*> g_thePlayer(0x03188918);

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
