#pragma once

#include "GameTypes.h"

class VMClassRegistry;
struct StaticFunctionTag;
class TESObjectREFR;
class TESForm;
class BGSTextureSet;

namespace papyrusNetImmerse
{
	void RegisterFuncs(VMClassRegistry* registry);

	bool HasNode(StaticFunctionTag * base, TESObjectREFR * obj, BSFixedString nodeName, bool firstPerson);
	float GetNodePositionX(StaticFunctionTag * base, TESObjectREFR * obj, BSFixedString nodeName, bool firstPerson);
	float GetNodePositionY(StaticFunctionTag * base, TESObjectREFR * obj, BSFixedString nodeName, bool firstPerson);
	float GetNodePositionZ(StaticFunctionTag * base, TESObjectREFR * obj, BSFixedString nodeName, bool firstPerson);
	float GetNodeScale(StaticFunctionTag * base, TESObjectREFR * obj, BSFixedString nodeName, bool firstPerson);
	void SetNodeScale(StaticFunctionTag * base, TESObjectREFR * obj, BSFixedString nodeName, float value, bool firstPerson);
	void SetNodeTextureSet(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, BGSTextureSet * textureSet, bool firstPerson);
}
