#include "PapyrusActorBase.h"

#include "GameForms.h"
#include "GameObjects.h"
#include "GameReferences.h"
#include "GameExtraData.h"
#include "GameData.h"
#include "GameRTTI.h"

namespace papyrusActorBase
{
	TESCombatStyle* GetCombatStyle(TESNPC* thisNPC)
	{
		return (thisNPC) ? thisNPC->combatStyle : NULL;
	}

	void SetCombatStyle(TESNPC* thisNPC, TESCombatStyle* cs)
	{
		if (thisNPC && cs) {
			thisNPC->combatStyle = cs;
		}
	}

	BGSOutfit* GetOutfit(TESNPC* thisNPC, bool bSleepOutfit)
	{
		if (!thisNPC)
			return NULL;
		return bSleepOutfit ? thisNPC->sleepOutfit : thisNPC->defaultOutfit;
	}

	void SetClass(TESNPC* thisNPC, TESClass* nuClass)
	{
		if (thisNPC && nuClass) {
			thisNPC->npcClass = nuClass;
		}
	}

	UInt32 GetSpellCount(TESNPC* thisNPC)
	{
		return (thisNPC) ? thisNPC->spellList.GetSpellCount() : 0;
	}

	SpellItem* GetNthSpell(TESNPC* thisNPC, UInt32 n)
	{
		return (thisNPC) ? thisNPC->spellList.GetNthSpell(n) : NULL;
	}

	BGSVoiceType* GetVoiceType(TESNPC* thisNPC)
	{
		return (thisNPC) ? thisNPC->actorData.voiceType : NULL;
	}

	void SetVoiceType(TESNPC* thisNPC, BGSVoiceType * newVoice)
	{
		if (thisNPC) {
			thisNPC->actorData.voiceType = newVoice;
		}
	}

	float GetWeight(TESNPC* thisNPC)
	{
		return (thisNPC) ? thisNPC->weight : 0.0;
	}

	void SetWeight(TESNPC* thisNPC, float weight)
	{
		if (thisNPC) {
			thisNPC->weight = weight;
		}
	}

	float GetHeight(TESNPC* thisNPC)
	{
		return (thisNPC) ? thisNPC->height : 0.0;
	}

	void SetHeight(TESNPC* thisNPC, float height)
	{
		if (thisNPC) {
			thisNPC->height = height;
		}
	}

	UInt32 GetNumHeadParts(TESNPC* thisNPC)
	{
		return (thisNPC) ? thisNPC->numHeadParts : 0;
	}

	BGSHeadPart* GetNthHeadPart(TESNPC* thisNPC, UInt32 n)
	{
		return (thisNPC && thisNPC->headparts && n < thisNPC->numHeadParts) ? thisNPC->headparts[n] : NULL;
	}

	void SetNthHeadPart(TESNPC* thisNPC, BGSHeadPart* headPart, UInt32 n )
	{
		if (thisNPC && n < thisNPC->numHeadParts) {
			if (headPart && thisNPC->headparts) {
				thisNPC->headparts[n] = headPart;
			}
		}
	}

	// Convenience function to return a headPart's index by type
	SInt32 GetIndexOfHeadPartByType(TESNPC* thisNPC, UInt32 type)
	{
		if (thisNPC && thisNPC->headparts) {
			for(int i = 0; i < thisNPC->numHeadParts; i++) {
				if(thisNPC->headparts[i] && thisNPC->headparts[i]->type == type && !thisNPC->headparts[i]->IsExtraPart()) {
					return i;
				}
			}
		}

		return -1;
	}

	UInt32 GetNumOverlayHeadParts(TESNPC* thisNPC)
	{
		return (thisNPC) ? GetNumActorBaseOverlays(thisNPC) : 0;
	}

	BGSHeadPart* GetNthOverlayHeadPart(TESNPC* thisNPC, UInt32 n)
	{
		if (!thisNPC)
			return NULL;

		UInt32 numOverlays = GetNumActorBaseOverlays(thisNPC);
		if(n >= numOverlays)
			return NULL;

		BGSHeadPart ** overlays = GetActorBaseOverlays(thisNPC);
		return overlays[n];
	}

	// Convenience function to return a headPart's index by type
	SInt32 GetIndexOfOverlayHeadPartByType(TESNPC* thisNPC, UInt32 type)
	{
		if (!thisNPC)
			return NULL;

		UInt32 numOverlays = GetNumActorBaseOverlays(thisNPC);
		BGSHeadPart ** overlays = GetActorBaseOverlays(thisNPC);
		for(UInt32 i = 0; i < numOverlays; i++)
		{
			if(overlays[i] && overlays[i]->type == type && !overlays[i]->IsExtraPart())
				return i;
		}

		return -1;
	}

	float GetFaceMorph(TESNPC* thisNPC, UInt32 index)
	{
		return (thisNPC && thisNPC->faceMorph && index < TESNPC::FaceMorphs::kNumOptions) ? thisNPC->faceMorph->option[index] : 0.0;
	}

	void SetFaceMorph(TESNPC* thisNPC, float value, UInt32 index)
	{
		if (thisNPC && thisNPC->faceMorph && index < TESNPC::FaceMorphs::kNumOptions) {
			thisNPC->faceMorph->option[index] = value;
		}
	}

	UInt32 GetFacePreset(TESNPC* thisNPC, UInt32 index)
	{
		return (thisNPC && thisNPC->faceMorph && index < TESNPC::FaceMorphs::kNumPresets) ? thisNPC->faceMorph->presets[index] : 0;
	}

	void SetFacePreset(TESNPC* thisNPC, UInt32 value, UInt32 index)
	{
		if (thisNPC && thisNPC->faceMorph && index < TESNPC::FaceMorphs::kNumPresets) {
			thisNPC->faceMorph->presets[index] = value;
		}
	}

	// Hair Color
	BGSColorForm* GetHairColor(TESNPC* thisNPC)
	{
		return (thisNPC && thisNPC->headData) ? thisNPC->headData->hairColor : NULL;
	}

	void SetHairColor(TESNPC* thisNPC, BGSColorForm* colorForm)
	{
		if (thisNPC && colorForm && thisNPC->headData) {
			thisNPC->headData->hairColor = colorForm;
		}
	}

	BGSTextureSet * GetFaceTextureSet(TESNPC* thisNPC)
	{
		return (thisNPC && thisNPC->headData) ? thisNPC->headData->headTexture : NULL;
	}

	void SetFaceTextureSet(TESNPC* thisNPC, BGSTextureSet * textureSet)
	{
		if (thisNPC) {
			thisNPC->SetFaceTexture(textureSet);
		}
	}

	TESObjectARMO * GetSkin(TESNPC* thisNPC)
	{
		return (thisNPC) ? thisNPC->skinForm.skin : NULL;
	}

	void SetSkin(TESNPC* thisNPC, TESObjectARMO * skin)
	{
		if (thisNPC) {
			thisNPC->skinForm.skin = skin;
		}
	}

	TESObjectARMO * GetSkinFar(TESNPC* thisNPC)
	{
		return (thisNPC) ? thisNPC->skinFar : NULL;
	}

	void SetSkinFar(TESNPC* thisNPC, TESObjectARMO * skin)
	{
		if (thisNPC) {
			thisNPC->skinFar = skin;
		}
	}

	TESNPC* GetTemplate(TESNPC* thisNPC)
	{
		return thisNPC ? thisNPC->GetRootTemplate() : NULL;
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusActorBase::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0 <TESNPC, TESCombatStyle*>("GetCombatStyle", "ActorBase", papyrusActorBase::GetCombatStyle, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESNPC, void, TESCombatStyle*>("SetCombatStyle", "ActorBase", papyrusActorBase::SetCombatStyle, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESNPC, BGSOutfit*, bool>("GetOutfit", "ActorBase", papyrusActorBase::GetOutfit, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESNPC, void, TESClass*>("SetClass", "ActorBase", papyrusActorBase::SetClass, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESNPC, UInt32>("GetSpellCount", "ActorBase", papyrusActorBase::GetSpellCount, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESNPC, SpellItem*, UInt32>("GetNthSpell", "ActorBase", papyrusActorBase::GetNthSpell, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESNPC, BGSVoiceType*>("GetVoiceType", "ActorBase", papyrusActorBase::GetVoiceType, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESNPC, void, BGSVoiceType*>("SetVoiceType", "ActorBase", papyrusActorBase::SetVoiceType, registry));

	// Character Creation Information

	registry->RegisterFunction(
		new NativeFunction0 <TESNPC, float>("GetHeight", "ActorBase", papyrusActorBase::GetHeight, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESNPC, void, float>("SetHeight", "ActorBase", papyrusActorBase::SetHeight, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESNPC, float>("GetWeight", "ActorBase", papyrusActorBase::GetWeight, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESNPC, void, float>("SetWeight", "ActorBase", papyrusActorBase::SetWeight, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESNPC, UInt32>("GetNumHeadParts", "ActorBase", papyrusActorBase::GetNumHeadParts, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESNPC, BGSHeadPart*, UInt32>("GetNthHeadPart", "ActorBase", papyrusActorBase::GetNthHeadPart, registry));

	registry->RegisterFunction(
		new NativeFunction2 <TESNPC, void, BGSHeadPart*, UInt32>("SetNthHeadPart", "ActorBase", papyrusActorBase::SetNthHeadPart, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESNPC, SInt32, UInt32>("GetIndexOfHeadPartByType", "ActorBase", papyrusActorBase::GetIndexOfHeadPartByType, registry));


	registry->RegisterFunction(
		new NativeFunction0 <TESNPC, UInt32>("GetNumOverlayHeadParts", "ActorBase", papyrusActorBase::GetNumOverlayHeadParts, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESNPC, BGSHeadPart*, UInt32>("GetNthOverlayHeadPart", "ActorBase", papyrusActorBase::GetNthOverlayHeadPart, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESNPC, SInt32, UInt32>("GetIndexOfOverlayHeadPartByType", "ActorBase", papyrusActorBase::GetIndexOfOverlayHeadPartByType, registry));


	registry->RegisterFunction(
		new NativeFunction1 <TESNPC, float, UInt32>("GetFaceMorph", "ActorBase", papyrusActorBase::GetFaceMorph, registry));

	registry->RegisterFunction(
		new NativeFunction2 <TESNPC, void, float, UInt32>("SetFaceMorph", "ActorBase", papyrusActorBase::SetFaceMorph, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESNPC, UInt32, UInt32>("GetFacePreset", "ActorBase", papyrusActorBase::GetFacePreset, registry));

	registry->RegisterFunction(
		new NativeFunction2 <TESNPC, void, UInt32, UInt32>("SetFacePreset", "ActorBase", papyrusActorBase::SetFacePreset, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESNPC, BGSColorForm*>("GetHairColor", "ActorBase", papyrusActorBase::GetHairColor, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESNPC, void, BGSColorForm*>("SetHairColor", "ActorBase", papyrusActorBase::SetHairColor, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESNPC, BGSTextureSet*>("GetFaceTextureSet", "ActorBase", papyrusActorBase::GetFaceTextureSet, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESNPC, void, BGSTextureSet*>("SetFaceTextureSet", "ActorBase", papyrusActorBase::SetFaceTextureSet, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESNPC, TESObjectARMO*>("GetSkin", "ActorBase", papyrusActorBase::GetSkin, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESNPC, void, TESObjectARMO*>("SetSkin", "ActorBase", papyrusActorBase::SetSkin, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESNPC, TESObjectARMO*>("GetSkinFar", "ActorBase", papyrusActorBase::GetSkinFar, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESNPC, void, TESObjectARMO*>("SetSkinFar", "ActorBase", papyrusActorBase::SetSkinFar, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESNPC, TESNPC*>("GetTemplate", "ActorBase", papyrusActorBase::GetTemplate, registry));

	registry->SetFunctionFlags("ActorBase", "GetCombatStyle", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "SetCombatStyle", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "GetOutfit", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "SetClass", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "GetSpellCount", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "GetNthSpell", VMClassRegistry::kFunctionFlag_NoWait);

	registry->SetFunctionFlags("ActorBase", "GetHeight", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "SetHeight", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "GetWeight", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "SetWeight", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "GetNumHeadParts", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "GetNthHeadPart", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "SetNthHeadPart", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "GetIndexOfHeadPartByType", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "GetNumOverlayHeadParts", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "GetNthOverlayHeadPart", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "GetIndexOfOverlayHeadPartByType", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "GetFaceMorph", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "SetFaceMorph", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "GetFacePreset", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "SetFacePreset", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "SetHairColor", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "GetHairColor", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "GetFaceTextureSet", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "SetFaceTextureSet", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "GetSkin", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "SetSkin", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ActorBase", "GetTemplate", VMClassRegistry::kFunctionFlag_NoWait);
}
