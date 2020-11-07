#include "ScaleformLoader.h"

#include "skse64_common/Relocation.h"

GFxLoader * GFxLoader::GetSingleton()
{
	// 4E9F39D1066653EF254B38406212E476F80A6C9B+AE
	RelocPtr<GFxLoader*> g_GFxLoader(0x02F257A0);
	return *g_GFxLoader;
}
