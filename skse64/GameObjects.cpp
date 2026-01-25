#include "GameObjects.h"
#include "GameData.h"
#include "GameRTTI.h"

UInt32 TESObjectBOOK::Data::GetSanitizedType(void)
{
	if(flags & kType_Spell)
		return kType_Spell;
	if(flags & kType_Skill)
		return kType_Skill;

	return kType_None;
}

BGSHeadPart * TESNPC::GetCurrentHeadPartByType(UInt32 type)
{
	BGSHeadPart * facePart = NULL;
	if(CALL_MEMBER_FN(this, HasOverlays)()) {
		facePart = GetHeadPartOverlayByType(type);
	} else {
		facePart = GetHeadPartByType(type);
	}

	return facePart;
}

BGSHeadPart * TESNPC::GetHeadPartByType(UInt32 type)
{
	if(headparts) {
		for(UInt32 i = 0; i < numHeadParts; i++) {
			if(headparts[i] && headparts[i]->type == type)
				return headparts[i];
		}
	}

	return NULL;
}

BGSHeadPart * TESNPC::GetHeadPartOverlayByType(UInt32 type)
{
	UInt32 numOverlays = GetNumActorBaseOverlays(this);
	BGSHeadPart ** overlays = GetActorBaseOverlays(this);
	if(overlays) {
		for(UInt32 i = 0; i < numOverlays; i++) {
			if(overlays[i]->type == type) {
				return overlays[i];
			}
		}
	}

	return NULL;
}


TESNPC * TESNPC::GetRootTemplate()
{
	TESNPC * node = NULL;
	node = nextTemplate;
	if (node) {
		while (node->nextTemplate)
			node = node->nextTemplate;
	}

	return node;
}

void TESObjectARMA::GetNodeName(char * dstBuff, TESObjectREFR * refr, TESObjectARMO * armor, float weightOverride)
{
	float weight = 100.0;
	TESNPC * npc = DYNAMIC_CAST(refr->baseForm, TESForm, TESNPC);
	if(npc && npc->nextTemplate) {
		TESNPC * temp = npc->GetRootTemplate();
		if(temp) {
			weight = temp->weight;
		}
	}
	else
		weight = CALL_MEMBER_FN(refr, GetWeight)();

	weight /= 100.0;
	if(weightOverride >= 0.0) // Determines whether to factor weight into the name, -1 true, 1 false
		weight = weightOverride;

	weight *= 100.0;
	
	UInt32 sex = npc ? CALL_MEMBER_FN(npc, GetSex)() : 0;
	/*sprintf_s(dstBuff, MAX_PATH, "%s (%08X)[%d]/%s (%08X) [%2.0f%%]", 
		this->Unk_32(), // I really have no idea why they used a string here for a boolean value, probably legacy code
		this->formID,
		sex,
		armor->Unk_32(), 
		armor->formID, 
		weight);*/
	sprintf_s(dstBuff, MAX_PATH, " (%08X)[%d]/ (%08X) [%2.0f%%]", 
		this->formID,
		sex,
		armor->formID, 
		weight);
}

void TESObjectWEAP::GetNodeName(char * dstBuff)
{
	/*sprintf_s(dstBuff, MAX_PATH, "%s %s (%08X)",
		"Weapon",
		this->Unk_34(), // I really have no idea why they used a string here for a boolean value, probably legacy code
		this->formID);*/
	sprintf_s(dstBuff, MAX_PATH, "%s  (%08X)",
		"Weapon",
		this->formID);
}

bool HasArmorRace(TESRace * sourceRace, TESRace * targetRace)
{
	if(sourceRace == targetRace)
		return true;

	// Check if this race inherits the source race
	TESRace * armorRace = sourceRace->armorRace;
	while(armorRace) {
		if(armorRace == targetRace)
			return true;
		armorRace = armorRace->armorRace;
	}

	return false;
}

bool TESObjectARMA::isValidRace(TESRace * sourceRace) const
{
	// Found race is the race this armor is designed for, or inherits a race this armor is designed for
	if(HasArmorRace(sourceRace, race.race))
		return true;

	for(UInt32 i = 0; i < additionalRaces.count; i++)
	{
		TESRace * currentRace = NULL;
		additionalRaces.GetNthItem(i, currentRace);

		// Source race is a found race, or inherits one of the found races
		if(HasArmorRace(sourceRace, currentRace))
			return true;
	}

	return false;
}

BSShaderTextureSet * BSShaderTextureSet::Create()
{
	BSShaderTextureSet * textureSet = (BSShaderTextureSet*)Heap_Allocate(sizeof(BSShaderTextureSet));
	CALL_MEMBER_FN(textureSet, ctor)();
	return textureSet;
}

void TESNPC::SetFaceTexture(BGSTextureSet * textureSet)
{
	if(!headData)
	{
		if(textureSet)
		{
			HeadData * newHeadData = (HeadData *)Heap_Allocate(sizeof(HeadData));
			if(newHeadData) {
				newHeadData->hairColor = NULL;
				newHeadData->headTexture = NULL;
			} else {
				newHeadData = NULL;
			}
			headData = newHeadData;
		}
	}
	if(headData)
		headData->headTexture = textureSet;
}

void TESNPC::SetHairColor(BGSColorForm * hairColor)
{
	if(!headData)
	{
		if(hairColor)
		{
			HeadData * newHeadData = (HeadData *)Heap_Allocate(sizeof(HeadData));
			if(newHeadData) {
				newHeadData->hairColor = NULL;
				newHeadData->headTexture = NULL;
			} else {
				newHeadData = NULL;
			}
			headData = newHeadData;
		}
	}
	if(headData)
		headData->hairColor = hairColor;
}
