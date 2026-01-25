#include "skse64/NiRenderer.h"

// 4EE5DE7D7BF6D392967302448BCC9612EEE6E7F9+4
RelocPtr <BSRenderManager> g_renderManager(0x03028490);

BSRenderManager * BSRenderManager::GetSingleton()
{
	return g_renderManager;
}

// 57AB5FBCBB5E60836D42FAF78C4DFED6A7BACA41+28
RelocPtr <BSShaderResourceManager> g_shaderResourceManager(0x030136C0);
