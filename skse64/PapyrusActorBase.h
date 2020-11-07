#pragma once

class VMClassRegistry;
class TESForm;
class TESNPC;
class TESCombatStyle;
class BGSOutfit;
class TESClass;
class SpellItem;
class BGSHeadPart;
class BGSTextureSet;
class TESObjectARMO;

namespace papyrusActorBase {
	void RegisterFuncs(VMClassRegistry* registry);

	TESCombatStyle* GetCombatStyle(TESNPC* thisNPC);
	void SetCombatStyle(TESNPC* thisNPC, TESCombatStyle* cs);

	BGSOutfit* GetOutfit(TESNPC* thisNPC, bool bSleepOutfit = false);

	void SetClass(TESNPC* thisNPC, TESClass* nuClass);

	float GetHeight(TESNPC* thisNPC);
	void SetHeight(TESNPC* thisNPC, float height);
	float GetWeight(TESNPC* thisNPC);
	void SetWeight(TESNPC* thisNPC, float weight);

	UInt32 GetNumHeadParts(TESNPC* thisNPC);
	BGSHeadPart* GetNthHeadPart(TESNPC* thisNPC, UInt32 n);
	void SetNthHeadPart(TESNPC* thisNPC, BGSHeadPart* headPart, UInt32 n );
	SInt32 GetIndexOfHeadPartByType(TESNPC* thisNPC, UInt32 type);

	UInt32 GetNumOverlayHeadParts(TESNPC* thisNPC);
	BGSHeadPart* GetNthOverlayHeadPart(TESNPC* thisNPC, UInt32 n);
	SInt32 GetIndexOfOverlayHeadPartByType(TESNPC* thisNPC, UInt32 type);

	float GetFaceMorph(TESNPC* thisNPC, UInt32 index);
	void SetFaceMorph(TESNPC* thisNPC, float value, UInt32 index);
	BGSTextureSet * GetFaceTextureSet(TESNPC* thisNPC);
	void SetFaceTextureSet(TESNPC* thisNPC, BGSTextureSet * textureSet);

	UInt32 GetSpellCount(TESNPC* thisNPC);
	SpellItem* GetNthSpell(TESNPC* thisNPC, UInt32 n);

	TESObjectARMO * GetSkin(TESNPC* thisNPC);
	void SetSkin(TESNPC* thisNPC, TESObjectARMO * skin);
	TESObjectARMO * GetSkinFar(TESNPC* thisNPC);
	void SetSkinFar(TESNPC* thisNPC, TESObjectARMO * skin);
	TESNPC* GetTemplate(TESNPC* thisNPC);
};
