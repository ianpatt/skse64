#include "skse64/NiRenderer.h"

// 984B8D46444288152A10744FB7226DC9943D175E+57
RelocPtr <BSRenderManager> g_renderManager(0x030C29C0);

BSRenderManager * BSRenderManager::GetSingleton()
{
	return g_renderManager;
}

// 56C98A6AC471728E145A8629B087CE2BE81252C1+CC
RelocPtr <BSShaderResourceManager> g_shaderResourceManager(0x030AE240);
