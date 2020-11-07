#pragma once

struct StaticFunctionTag;
class VMClassRegistry;
class Setting;
class Actor;
class TESForm;
class TESObjectREFR;

#include "GameTypes.h"

namespace papyrusGame
{
	void RegisterFuncs(VMClassRegistry* registry);

	UInt32 GetPerkPoints(StaticFunctionTag*);
	void SetPerkPoints(StaticFunctionTag*, UInt32 perkPoints);
	void ModPerkPoints(StaticFunctionTag*, SInt32 modPerkPointsBy);

	void SetPlayerLevelPoints(StaticFunctionTag*, float points);
	float GetPlayerLevelPoints(StaticFunctionTag*);
	float GetPointsForLevel(StaticFunctionTag*, UInt32 level);

	UInt32 GetModCount(StaticFunctionTag*);
	UInt32 GetModByName(StaticFunctionTag*, BSFixedString name);
	BSFixedString GetModName(StaticFunctionTag*, UInt32 index);
	BSFixedString GetModAuthor(StaticFunctionTag*, UInt32 index);
	BSFixedString GetModDescription(StaticFunctionTag*, UInt32 index);
	UInt32 GetModDependencyCount(StaticFunctionTag*, UInt32 index);
	UInt32 GetNthModDependency(StaticFunctionTag*, UInt32 index, UInt32 dep_index);

	void SetGameSettingFloat(StaticFunctionTag * base, BSFixedString name, float value);
	void SetGameSettingInt(StaticFunctionTag * base, BSFixedString name, UInt32 value);
	void SetGameSettingBool(StaticFunctionTag * base, BSFixedString name, bool value);
	void SetGameSettingString(StaticFunctionTag * base, BSFixedString name, BSFixedString value);

	UInt32 GetNumTintMasks(StaticFunctionTag * base);
	UInt32 GetNthTintMaskColor(StaticFunctionTag * base, UInt32 n);
	void SetNthTintMaskColor(StaticFunctionTag * base, UInt32 n, UInt32 color);
	UInt32 GetNthTintMaskType(StaticFunctionTag * base, UInt32 n);
	BSFixedString GetNthTintMaskTexturePath(StaticFunctionTag * base, UInt32 n);
	void SetNthTintMaskTexturePath(StaticFunctionTag * base, BSFixedString path, UInt32 n);

	UInt32 GetNumTintMasksByType(StaticFunctionTag * base, UInt32 tintType);
	UInt32 GetTintMaskColor(StaticFunctionTag * base, UInt32 tintType, UInt32 index);
	void SetTintMaskColor(StaticFunctionTag * base, UInt32 color, UInt32 tintType, UInt32 index);
	BSFixedString GetTintMaskTexturePath(StaticFunctionTag * base, UInt32 tintType, UInt32 index);
	void SetTintMaskTexturePath(StaticFunctionTag * base, BSFixedString path, UInt32 tintType, UInt32 index);

	void UpdateTintMaskColors(StaticFunctionTag * base);

	void SetMiscStat(StaticFunctionTag * base, BSFixedString name, UInt32 value);
	void SetPlayersLastRiddenHorse(StaticFunctionTag * base, Actor* actor);
	bool GetPlayerMovementMode(StaticFunctionTag * base);

	void UnbindObjectHotkey(StaticFunctionTag * base, SInt32 hotkey);
	bool IsObjectHotkeyBound(StaticFunctionTag * base, SInt32 hotkey);
	TESForm * GetHotkeyBoundObject(StaticFunctionTag * base, SInt32 hotkey);

	SInt32 GetSkillLegendaryLevel(StaticFunctionTag * base, BSFixedString actorValue);
	void SetSkillLegendaryLevel(StaticFunctionTag * base, BSFixedString actorValue, UInt32 level);

	bool IsObjectFavorited(StaticFunctionTag * base, TESForm * form);

	TESForm* GetFormEx(StaticFunctionTag * base, UInt32 formId);

	TESObjectREFR* GetCurrentCrosshairRef(StaticFunctionTag * base);
};
