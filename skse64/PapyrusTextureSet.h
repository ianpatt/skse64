#pragma once

#include "GameTypes.h"

class BGSTextureSet;
class VMClassRegistry;

namespace papyrusTextureSet
{
	void RegisterFuncs(VMClassRegistry * registry);

	UInt32 GetNumTexturePaths(BGSTextureSet * thisTextureSet);
	BSFixedString GetNthTexturePath(BGSTextureSet * thisTextureSet, UInt32 n);
	void SetNthTexturePath(BGSTextureSet * thisTextureSet, UInt32 n, BSFixedString texturePath);
};
