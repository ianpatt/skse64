#include "ScaleformLoader.h"

#include "skse64_common/Relocation.h"

GFxLoader * GFxLoader::GetSingleton()
{
	// D73FE972523296E269125B9E6F076AD967D7970C+F9
	RelocPtr<GFxLoader*> g_GFxLoader(0x02FB92B0);
	return *g_GFxLoader;
}
