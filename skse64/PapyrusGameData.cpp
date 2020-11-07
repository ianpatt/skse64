#include "PapyrusGameData.h"

#include "skse64/GameFormComponents.h"
#include "skse64/GameData.h"
#include "skse64/GameRTTI.h"
#include "skse64/GameForms.h"
#include "skse64/GameObjects.h"
#include "skse64/GameReferences.h"

#include "skse64/PapyrusArgs.h"

#include <set>

enum WeaponTypes
{
	kFilterType_HandToHandMelee = (1 << 0),
	kFilterType_OneHandSword = (1 << 1),
	kFilterType_OneHandDagger = (1 << 2),
	kFilterType_OneHandAxe = (1 << 3),
	kFilterType_OneHandMace = (1 << 4),
	kFilterType_TwoHandSword = (1 << 5),
	kFilterType_TwoHandAxe = (1 << 6),
	kFilterType_Bow = (1 << 7),
	kFilterType_Staff = (1 << 8),
	kFilterType_CrossBow = (1 << 9)
};

#define IS_TYPE(weaponTypes, weapon, t, h, s) ((weaponTypes & t) == t) && (weapon->gameData.type == h || weapon->gameData.type == s)

bool VerifyKeywords(TESForm * form, VMArray<BGSKeyword*> * keywords)
{
	if (keywords->Length() > 0) {
		BGSKeywordForm* pKeywords = DYNAMIC_CAST(form, TESForm, BGSKeywordForm);
		if (pKeywords) {
			bool failed = false;
			BGSKeyword * keyword = NULL;
			for (UInt32 k = 0; k < keywords->Length(); k++) {
				keywords->Get(&keyword, k);
				if (keyword && !pKeywords->HasKeyword(keyword)) {
					failed = true;
					break;
				}
			}

			if (failed)
				return false;
		}
	}

	return true;
}

namespace papyrusGameData
{
	VMResultArray<TESForm*> GetAllWeapons(StaticFunctionTag*, BSFixedString modName, VMArray<BGSKeyword*> keywords, bool playable, bool ignoreTemplates, bool ignoreEnchantments, bool onlyEnchanted, UInt32 weaponTypes)
	{
		VMResultArray<TESForm*> result;

		DataHandler * dataHandler = DataHandler::GetSingleton();
		const ModInfo * modInfo = dataHandler->LookupModByName(modName.data);
		if (!modInfo || !modInfo->IsActive())
			return result;

		TESObjectWEAP * weapon = NULL;
		for (UInt32 i = 0; i < dataHandler->weapons.count; i++)
		{
			dataHandler->weapons.GetNthItem(i, weapon);

			if (!modInfo->IsFormInMod(weapon->formID))
				continue;
			if (playable != weapon->IsPlayable())
				continue;
			if (ignoreTemplates && weapon->templateForm)
				continue;
			if (ignoreEnchantments && weapon->enchantable.enchantment)
				continue;
			if (onlyEnchanted && !weapon->enchantable.enchantment)
				continue;

			if (!VerifyKeywords(weapon, &keywords))
				continue;

			bool accept = false;
			if (IS_TYPE(weaponTypes, weapon, kFilterType_HandToHandMelee, TESObjectWEAP::GameData::kType_HandToHandMelee, TESObjectWEAP::GameData::kType_H2H))
				accept = true;
			if (IS_TYPE(weaponTypes, weapon, kFilterType_OneHandSword, TESObjectWEAP::GameData::kType_OneHandSword, TESObjectWEAP::GameData::kType_1HS))
				accept = true;
			if (IS_TYPE(weaponTypes, weapon, kFilterType_OneHandDagger, TESObjectWEAP::GameData::kType_OneHandDagger, TESObjectWEAP::GameData::kType_1HD))
				accept = true;
			if (IS_TYPE(weaponTypes, weapon, kFilterType_OneHandAxe, TESObjectWEAP::GameData::kType_OneHandAxe, TESObjectWEAP::GameData::kType_1HA))
				accept = true;
			if (IS_TYPE(weaponTypes, weapon, kFilterType_OneHandMace, TESObjectWEAP::GameData::kType_OneHandMace, TESObjectWEAP::GameData::kType_1HM))
				accept = true;
			if (IS_TYPE(weaponTypes, weapon, kFilterType_TwoHandSword, TESObjectWEAP::GameData::kType_TwoHandSword, TESObjectWEAP::GameData::kType_2HS))
				accept = true;
			if (IS_TYPE(weaponTypes, weapon, kFilterType_TwoHandAxe, TESObjectWEAP::GameData::kType_TwoHandAxe, TESObjectWEAP::GameData::kType_2HA))
				accept = true;
			if (IS_TYPE(weaponTypes, weapon, kFilterType_Bow, TESObjectWEAP::GameData::kType_Bow, TESObjectWEAP::GameData::kType_Bow2))
				accept = true;
			if (IS_TYPE(weaponTypes, weapon, kFilterType_Staff, TESObjectWEAP::GameData::kType_Staff, TESObjectWEAP::GameData::kType_Staff2))
				accept = true;
			if (IS_TYPE(weaponTypes, weapon, kFilterType_CrossBow, TESObjectWEAP::GameData::kType_CrossBow, TESObjectWEAP::GameData::kType_CBow))
				accept = true;
			if (!accept)
				continue;

			result.push_back(weapon);
		}

		return result;
	}

	VMResultArray<TESForm*> GetAllArmor(StaticFunctionTag*, BSFixedString modName, VMArray<BGSKeyword*> keywords, bool playable, bool ignoreTemplates, bool ignoreEnchantments, bool onlyEnchanted, bool ignoreSkin)
	{
		VMResultArray<TESForm*> result;

		DataHandler * dataHandler = DataHandler::GetSingleton();

		const ModInfo * modInfo = dataHandler->LookupModByName(modName.data);
		if (!modInfo || !modInfo->IsActive())
			return result;

		std::set<TESObjectARMO*> exclude;
		if (ignoreSkin) {
			TESRace * race = NULL;
			for (UInt32 i = 0; i < dataHandler->races.count; i++)
			{
				dataHandler->races.GetNthItem(i, race);
				if (race->skin.skin)
					exclude.insert(race->skin.skin);
			}

			TESNPC * npc = NULL;
			for (UInt32 i = 0; i < dataHandler->npcs.count; i++)
			{
				dataHandler->npcs.GetNthItem(i, npc);
				if (npc->skinForm.skin)
					exclude.insert(npc->skinForm.skin);
			}
		}

		TESObjectARMO * armor = NULL;
		for (UInt32 i = 0; i < dataHandler->armors.count; i++)
		{
			dataHandler->armors.GetNthItem(i, armor);

			if (!modInfo->IsFormInMod(armor->formID))
				continue;
			if (ignoreSkin && exclude.find(armor) != exclude.end())
				continue;
			if (playable != armor->IsPlayable())
				continue;
			if (ignoreTemplates && armor->templateArmor)
				continue;
			if (ignoreEnchantments && armor->enchantable.enchantment)
				continue;
			if (onlyEnchanted && !armor->enchantable.enchantment)
				continue;
			if (!VerifyKeywords(armor, &keywords))
				continue;

			result.push_back(armor);
		}


		return result;
	}

	VMResultArray<TESForm*> GetAllAmmo(StaticFunctionTag*, BSFixedString modName, VMArray<BGSKeyword*> keywords, bool playable)
	{
		VMResultArray<TESForm*> result;

		DataHandler * dataHandler = DataHandler::GetSingleton();
		const ModInfo * modInfo = dataHandler->LookupModByName(modName.data);
		if (!modInfo || !modInfo->IsActive())
			return result;


		TESAmmo * ammo = NULL;
		for (UInt32 i = 0; i < dataHandler->ammo.count; i++)
		{
			dataHandler->ammo.GetNthItem(i, ammo);

			if (!modInfo->IsFormInMod(ammo->formID))
				continue;
			if (playable != ammo->IsPlayable())
				continue;
			if (!VerifyKeywords(ammo, &keywords))
				continue;

			result.push_back(ammo);
		}

		return result;
	}

	VMResultArray<TESForm*> GetAllBooks(StaticFunctionTag*, BSFixedString modName, VMArray<BGSKeyword*> keywords, bool regular, bool spellBook, bool skillBook)
	{
		VMResultArray<TESForm*> result;

		DataHandler * dataHandler = DataHandler::GetSingleton();
		const ModInfo * modInfo = dataHandler->LookupModByName(modName.data);
		if (!modInfo || !modInfo->IsActive())
			return result;

		TESObjectBOOK * book = NULL;
		for (UInt32 i = 0; i < dataHandler->books.count; i++)
		{
			dataHandler->books.GetNthItem(i, book);

			if (!modInfo->IsFormInMod(book->formID))
				continue;

			if (!VerifyKeywords(book, &keywords))
				continue;

			bool isSkillBook = book->data.GetSanitizedType() == TESObjectBOOK::Data::kType_Skill;
			bool isSpellBook = book->data.GetSanitizedType() == TESObjectBOOK::Data::kType_Spell;
				
			bool accept = false;
			if (regular && !isSkillBook && !isSpellBook)
				accept = true;
			if (spellBook && isSpellBook)
				accept = true;
			if (skillBook && isSkillBook)
				accept = true;
			if (!accept)
				continue;


			result.push_back(book);
		}

		return result;
	}

	VMResultArray<TESForm*> GetAllPotions(StaticFunctionTag*, BSFixedString modName, VMArray<BGSKeyword*> keywords, bool potions, bool food, bool poison)
	{
		VMResultArray<TESForm*> result;

		DataHandler * dataHandler = DataHandler::GetSingleton();
		const ModInfo * modInfo = dataHandler->LookupModByName(modName.data);
		if (!modInfo || !modInfo->IsActive())
			return result;

		AlchemyItem * potion = NULL;
		for (UInt32 i = 0; i < dataHandler->potions.count; i++)
		{
			dataHandler->potions.GetNthItem(i, potion);

			if (!modInfo->IsFormInMod(potion->formID))
				continue;

			if (!VerifyKeywords(potion, &keywords))
				continue;
				
			bool isFood = potion->IsFood();
			bool isPoison = potion->IsPoison();

			bool accept = false;
			if (potions && !isFood && !isPoison)
				accept = true;
			if (food && isFood)
				accept = true;
			if (poison && isPoison)
				accept = true;
			if (!accept)
				continue;


			result.push_back(potion);
		}

		return result;
	}

	VMResultArray<TESForm*> GetAllIngredients(StaticFunctionTag*, BSFixedString modName, VMArray<BGSKeyword*> keywords)
	{
		VMResultArray<TESForm*> result;

		DataHandler * dataHandler = DataHandler::GetSingleton();
		const ModInfo * modInfo = dataHandler->LookupModByName(modName.data);
		if (!modInfo || !modInfo->IsActive())
			return result;

		IngredientItem * ingredient = NULL;
		for (UInt32 i = 0; i < dataHandler->ingredients.count; i++)
		{
			dataHandler->ingredients.GetNthItem(i, ingredient);

			if (!modInfo->IsFormInMod(ingredient->formID))
				continue;

			if (!VerifyKeywords(ingredient, &keywords))
				continue;

			result.push_back(ingredient);
		}

		return result;
	}

	VMResultArray<TESForm*> GetAllScrolls(StaticFunctionTag*, BSFixedString modName, VMArray<BGSKeyword*> keywords)
	{
		VMResultArray<TESForm*> result;

		DataHandler * dataHandler = DataHandler::GetSingleton();
		const ModInfo * modInfo = dataHandler->LookupModByName(modName.data);
		if (!modInfo || !modInfo->IsActive())
			return result;

		ScrollItem * scroll = NULL;
		for (UInt32 i = 0; i < dataHandler->scrolls.count; i++)
		{
			dataHandler->scrolls.GetNthItem(i, scroll);

			if (!modInfo->IsFormInMod(scroll->formID))
				continue;

			if (!VerifyKeywords(scroll, &keywords))
				continue;

			result.push_back(scroll);
		}

		return result;
	}

	VMResultArray<TESForm*> GetAllKeys(StaticFunctionTag*, BSFixedString modName, VMArray<BGSKeyword*> keywords)
	{
		VMResultArray<TESForm*> result;

		DataHandler * dataHandler = DataHandler::GetSingleton();
		const ModInfo * modInfo = dataHandler->LookupModByName(modName.data);
		if (!modInfo || !modInfo->IsActive())
			return result;

	
			TESKey * key = NULL;
			for (UInt32 i = 0; i < dataHandler->keys.count; i++)
			{
				dataHandler->keys.GetNthItem(i, key);

				if (!modInfo->IsFormInMod(key->formID))
					continue;

				if (!VerifyKeywords(key, &keywords))
					continue;

				result.push_back(key);
			}

		return result;
	}

	VMResultArray<TESForm*> GetAllMiscItems(StaticFunctionTag*, BSFixedString modName, VMArray<BGSKeyword*> keywords)
	{
		VMResultArray<TESForm*> result;

		DataHandler * dataHandler = DataHandler::GetSingleton();
		const ModInfo * modInfo = dataHandler->LookupModByName(modName.data);
		if (!modInfo || !modInfo->IsActive())
			return result;

		TESObjectMISC * misc = NULL;
		for (UInt32 i = 0; i < dataHandler->miscObjects.count; i++)
		{
			dataHandler->miscObjects.GetNthItem(i, misc);

			if (!modInfo->IsFormInMod(misc->formID))
				continue;

			if (!VerifyKeywords(misc, &keywords))
				continue;

			result.push_back(misc);
		}

		return result;
	}
}

#include "skse64/PapyrusVM.h"
#include "skse64/PapyrusNativeFunctions.h"

void papyrusGameData::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction7<StaticFunctionTag, VMResultArray<TESForm*>, BSFixedString, VMArray<BGSKeyword*>, bool, bool, bool, bool, UInt32>("GetAllWeapons", "GameData", papyrusGameData::GetAllWeapons, registry));

	registry->RegisterFunction(
		new NativeFunction7<StaticFunctionTag, VMResultArray<TESForm*>, BSFixedString, VMArray<BGSKeyword*>, bool, bool, bool, bool, bool>("GetAllArmor", "GameData", papyrusGameData::GetAllArmor, registry));

	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, VMResultArray<TESForm*>, BSFixedString, VMArray<BGSKeyword*>, bool>("GetAllAmmo", "GameData", papyrusGameData::GetAllAmmo, registry));

	registry->RegisterFunction(
		new NativeFunction5<StaticFunctionTag, VMResultArray<TESForm*>, BSFixedString, VMArray<BGSKeyword*>, bool, bool, bool>("GetAllBooks", "GameData", papyrusGameData::GetAllBooks, registry));

	registry->RegisterFunction(
		new NativeFunction5<StaticFunctionTag, VMResultArray<TESForm*>, BSFixedString, VMArray<BGSKeyword*>, bool, bool, bool>("GetAllPotions", "GameData", papyrusGameData::GetAllPotions, registry));

	registry->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, VMResultArray<TESForm*>, BSFixedString, VMArray<BGSKeyword*>>("GetAllIngredients", "GameData", papyrusGameData::GetAllIngredients, registry));

	registry->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, VMResultArray<TESForm*>, BSFixedString, VMArray<BGSKeyword*>>("GetAllScrolls", "GameData", papyrusGameData::GetAllScrolls, registry));

	registry->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, VMResultArray<TESForm*>, BSFixedString, VMArray<BGSKeyword*>>("GetAllKeys", "GameData", papyrusGameData::GetAllKeys, registry));

	registry->RegisterFunction(
		new NativeFunction2<StaticFunctionTag, VMResultArray<TESForm*>, BSFixedString, VMArray<BGSKeyword*>>("GetAllMiscItems", "GameData", papyrusGameData::GetAllMiscItems, registry));
}
