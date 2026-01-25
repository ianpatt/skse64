#include "skse64/NiMaterial.h"
#include "skse64/GameAPI.h"
#include "skse64/GameObjects.h"

// 96D2EFF152717F366F7EAAA62CAD7EB12855423D+47
RelocAddr<_CreateShaderMaterial> CreateShaderMaterial(0x012CFBC0);

RelocAddr<_CreateFacegenTintMaterial> CreateFacegenTintMaterial(0x012D26F0);

void BSLightingShaderMaterial::SetTextureSet(BSTextureSet * newSet)
{
	textureSet = newSet;
}
