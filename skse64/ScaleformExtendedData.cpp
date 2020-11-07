#include "ScaleformCallbacks.h"
#include "ScaleformMovie.h"
#include "ScaleformExtendedData.h"
#include "GameData.h"
#include "GameForms.h"
#include "GameObjects.h"
#include "GameReferences.h"
#include "GameExtraData.h"
#include "GameRTTI.h"
#include "GameMenus.h"

// Helpers
double round(double r)
{
	return (r >= 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

void RegisterUnmanagedString(GFxValue * dst, const char * name, const char * str)
{
	GFxValue	fxValue;

	fxValue.SetString(str);

	dst->SetMember(name, &fxValue);
}

void RegisterString(GFxValue * dst,  GFxMovieView * view, const char * name, const char * str)
{
	GFxValue	fxValue;

	view->CreateString(&fxValue, str);

	dst->SetMember(name, &fxValue);
}

void RegisterWideString(GFxValue * dst,  GFxMovieView * view, const char * name, const wchar_t * str)
{
	GFxValue	fxValue;

	view->CreateWideString(&fxValue, str);

	dst->SetMember(name, &fxValue);
}

void RegisterNumber(GFxValue * dst, const char * name, double value)
{
	GFxValue	fxValue;

	fxValue.SetNumber(value);

	dst->SetMember(name, &fxValue);
}

void RegisterBool(GFxValue * dst, const char * name, bool value)
{
	GFxValue fxValue;
	fxValue.SetBool(value);
	dst->SetMember(name, &fxValue);
}

void RegisterKeywords(GFxValue * pFxVal, GFxMovieView * view, BGSKeywordForm * keywordForm)
{
	GFxValue	keywordRoot;
	view->CreateObject(&keywordRoot);

	// Add all keywords as boolean properties with value true

	UInt32 count = keywordForm->numKeywords;
	BGSKeyword ** keywords = keywordForm->keywords;
	if(keywords)
	{
		for(int i = 0; i < count; i++)
		{
			BGSKeyword * pKey = keywords[i];
			if(pKey)
			{
				const char * keyString = pKey->keyword.Get();
				if(keyString)
				{
					RegisterBool(&keywordRoot, keyString, true);
				}
			}
		}
	}

	pFxVal->SetMember("keywords", &keywordRoot);
}

class FormListExtender : public BGSListForm::Visitor
{
	GFxValue		* m_array;
	GFxMovieView	* m_movie;
	bool	m_bExtra;
	bool	m_bRecursive;
	
public:
	FormListExtender::FormListExtender(GFxValue	* formArray, GFxMovieView * movie, bool bExtra, bool bRecursive) : m_array(formArray), m_movie(movie), m_bExtra(bExtra), m_bRecursive(bRecursive) { }
	virtual bool Accept(TESForm * form)
	{
		if(form) {
			GFxValue arrArg;
			m_movie->CreateObject(&arrArg);
			scaleformExtend::FormData(&arrArg, m_movie, form, m_bRecursive ? m_bExtra : false, m_bRecursive);
			m_array->PushBack(&arrArg);
		}
		return false;
	};
};

// Data extensions
namespace scaleformExtend
{
	void CommonItemData(GFxValue* pFxVal, TESForm * pForm)
	{
		if(!pFxVal || !pForm || !pFxVal->IsObject())
			return;

		RegisterNumber(pFxVal, "formType", (double)pForm->GetFormType());
		RegisterNumber(pFxVal, "formId", (double)pForm->formID);
	}

	void StandardItemData(GFxValue * pFxVal, TESForm * pForm, InventoryEntryData * pEntry)
	{
		if(!pForm || !pFxVal || !pFxVal->IsObject())
			return;

		switch(pForm->GetFormType())
		{
		case kFormType_Armor:
			{
				TESObjectARMO * pArmor = DYNAMIC_CAST(pForm, TESForm, TESObjectARMO);
				if(pArmor)
				{
					RegisterNumber(pFxVal, "partMask", pArmor->bipedObject.data.parts);
					RegisterNumber(pFxVal, "weightClass", pArmor->bipedObject.data.weightClass);
				}
			}
			break;

		case kFormType_Ammo:
			{
				TESAmmo * pAmmo = DYNAMIC_CAST(pForm, TESForm, TESAmmo);
				if(pAmmo)
				{
					RegisterNumber(pFxVal, "flags", pAmmo->settings.flags);
				}
			}
			break;

		case kFormType_Weapon:
			{
				TESObjectWEAP * pWeapon = DYNAMIC_CAST(pForm, TESForm, TESObjectWEAP);
				if(pWeapon)
				{
					RegisterNumber(pFxVal, "subType", pWeapon->type()); // DEPRECATED
					RegisterNumber(pFxVal, "weaponType", pWeapon->type());
					RegisterNumber(pFxVal, "speed", pWeapon->speed());
					RegisterNumber(pFxVal, "reach", pWeapon->reach());
					RegisterNumber(pFxVal, "stagger", pWeapon->stagger());
					RegisterNumber(pFxVal, "critDamage", pWeapon->critDamage());
					RegisterNumber(pFxVal, "minRange", pWeapon->minRange());
					RegisterNumber(pFxVal, "maxRange", pWeapon->maxRange());
					RegisterNumber(pFxVal, "baseDamage", pWeapon->damage.GetAttackDamage());

					BGSEquipSlot * equipSlot = pWeapon->equipType.GetEquipSlot();
					if (equipSlot)
						RegisterNumber(pFxVal, "equipSlot", equipSlot->formID);
				}
			}
			break;

		case kFormType_SoulGem:
			{
				TESSoulGem	* soulGem = DYNAMIC_CAST(pForm, TESForm, TESSoulGem);
				if(soulGem)
				{					
					RegisterNumber(pFxVal, "gemSize", soulGem->gemSize);
					RegisterNumber(pFxVal, "soulSize", pEntry ? CALL_MEMBER_FN(pEntry,GetSoulLevel)() : soulGem->soulSize);
				}
			}
			break;

		case kFormType_Potion:
			{
				AlchemyItem * pAlchemy = DYNAMIC_CAST(pForm, TESForm, AlchemyItem);
				if(pAlchemy)
				{
					RegisterNumber(pFxVal, "flags", pAlchemy->itemData.flags);
				}
			}
			break;

		case kFormType_Book:
			{
				TESObjectBOOK * pBook = DYNAMIC_CAST(pForm, TESForm, TESObjectBOOK);
				if(pBook)
				{
					RegisterNumber(pFxVal, "flags", pBook->data.flags);
					RegisterNumber(pFxVal, "bookType", pBook->data.type);
					switch(pBook->data.GetSanitizedType())
					{
					case TESObjectBOOK::Data::kType_Skill:
						RegisterNumber(pFxVal, "teachesSkill", pBook->data.teaches.skill);
						break;

					case TESObjectBOOK::Data::kType_Spell:
						{
							double formID = -1;

							if(pBook->data.teaches.spell)
								formID = pBook->data.teaches.spell->formID;

							RegisterNumber(pFxVal, "teachesSpell", formID);
						}
						break;
					}
				}
			}
			break;

		default:
			break;
		}
	}


	void MagicItemData(GFxValue * pFxVal, GFxMovieView * movieView, TESForm * pForm, bool bExtra, bool bRecursive)
	{
		if(!pFxVal || !pForm || !pFxVal->IsObject())
			return;

		switch(pForm->GetFormType())
		{
		case kFormType_Spell:
		case kFormType_ScrollItem:
		case kFormType_Ingredient:
		case kFormType_Potion:
		case kFormType_Enchantment:
			{
				MagicItem * pMagicItem = DYNAMIC_CAST(pForm, TESForm, MagicItem);
				if(pMagicItem)
				{
					if(pMagicItem->fullName.name.data)
						RegisterUnmanagedString(pFxVal, "spellName", pMagicItem->fullName.name.data);

					MagicItem::EffectItem * pEffect = CALL_MEMBER_FN(pMagicItem, GetCostliestEffectItem)(5, false);
					if(pEffect && pEffect->mgef)
					{
						RegisterNumber(pFxVal, "magnitude", pEffect->magnitude);
						RegisterNumber(pFxVal, "duration", pEffect->duration);
						RegisterNumber(pFxVal, "area", pEffect->area);

						// PLB: Normally I'd have this as a separate object but SkyUI is already using this
						scaleformExtend::MagicItemData(pFxVal, movieView, pEffect->mgef, bRecursive ? bExtra : false, bRecursive);
					}
				}

				SpellItem * pSpellItem = DYNAMIC_CAST(pMagicItem, MagicItem, SpellItem);
				if(pSpellItem)
				{
					RegisterNumber(pFxVal, "spellType", pSpellItem->data.type);
					RegisterNumber(pFxVal, "trueCost", pSpellItem->GetMagickaCost());

					BGSEquipSlot * equipSlot = pSpellItem->equipType.GetEquipSlot();
					if (equipSlot)
						RegisterNumber(pFxVal, "equipSlot", equipSlot->formID);
				}

				AlchemyItem * pAlchemyItem = DYNAMIC_CAST(pMagicItem, MagicItem, AlchemyItem);
				if(pAlchemyItem)
				{
					if(pAlchemyItem->itemData.useSound) {
						GFxValue useSound;
						movieView->CreateObject(&useSound);
						scaleformExtend::FormData(&useSound, movieView, pAlchemyItem->itemData.useSound, bRecursive ? bExtra : false, bRecursive);
						pFxVal->SetMember("useSound", &useSound);
					}
				}

				EnchantmentItem * pEnchantItem = DYNAMIC_CAST(pMagicItem, MagicItem, EnchantmentItem);
				if(pEnchantItem)
				{
					RegisterNumber(pFxVal, "flags", (double)pMagicItem->flags);

					GFxValue baseEnchant;
					movieView->CreateObject(&baseEnchant);
					scaleformExtend::FormData(&baseEnchant, movieView, pEnchantItem->data.baseEnchantment, bRecursive ? bExtra : false, bRecursive);
					pFxVal->SetMember("baseEnchant", &baseEnchant);

					GFxValue restrictions;
					movieView->CreateObject(&restrictions);
					scaleformExtend::FormData(&restrictions, movieView, pEnchantItem->data.restrictions, bRecursive ? bExtra : false, bRecursive);
					pFxVal->SetMember("restrictions", &restrictions);
				}
			}
			break;

		case kFormType_EffectSetting:
			{
				EffectSetting * pEffectSetting = DYNAMIC_CAST(pForm, TESForm, EffectSetting);
				if(pEffectSetting)
				{
					if(pEffectSetting->fullName.name.data)
						RegisterUnmanagedString(pFxVal, "effectName", pEffectSetting->fullName.name.data);

					RegisterNumber(pFxVal, "subType", pEffectSetting->school()); // DEPRECATED
					RegisterNumber(pFxVal, "effectFlags", pEffectSetting->properties.flags);
					RegisterNumber(pFxVal, "school", pEffectSetting->school());
					RegisterNumber(pFxVal, "skillLevel", pEffectSetting->level());
					RegisterNumber(pFxVal, "archetype", pEffectSetting->properties.archetype);
					RegisterNumber(pFxVal, "deliveryType", pEffectSetting->properties.deliveryType);
					RegisterNumber(pFxVal, "castTime", pEffectSetting->properties.castingTime);
					RegisterNumber(pFxVal, "delayTime", pEffectSetting->properties.delayTime);
					RegisterNumber(pFxVal, "actorValue", pEffectSetting->properties.primaryValue);
					RegisterNumber(pFxVal, "castType", pEffectSetting->properties.castType);
					RegisterNumber(pFxVal, "magicType", pEffectSetting->properties.resistance);
				}
			}
			break;
		case kFormType_Shout:
			{
				TESShout * pShout = DYNAMIC_CAST(pForm, TESForm, TESShout);
				if(pShout)
				{
					if(pShout->fullName.name.data)
						RegisterUnmanagedString(pFxVal, "fullName", pShout->fullName.name.data);

					// Words
					GFxValue words;
					movieView->CreateArray(&words);
					for(int i = 0; i < pShout->Words::kNumWords; i++)
					{
						GFxValue word;
						movieView->CreateObject(&word);

						if(pShout->words[i].word && pShout->words[i].word->fullName.name.data)
							RegisterUnmanagedString(&word, "word", pShout->words[i].word->fullName.name.data);

						if(pShout->words[i].word && pShout->words[i].word->word.data)
							RegisterUnmanagedString(&word, "fullName", pShout->words[i].word->word.data);

						RegisterNumber(&word, "recoveryTime", pShout->words[i].recoverytime);

						// Spell Object
						scaleformExtend::FormData(&word, movieView, pShout->words[i].spell, bRecursive ? bExtra : false, bRecursive);
						words.PushBack(&word);
					}
					pFxVal->SetMember("words", &words);
				}
			}
			break;

		default:
			break;
		}
	}

	void ActorData(GFxValue * pFxVal,  GFxMovieView * movieView, TESForm * pForm, bool bExtra, bool bRecursive)
	{
		if(!pForm || !pFxVal || !pFxVal->IsObject())
			return;

		switch(pForm->GetFormType())
		{
		case kFormType_Race:
			{
				TESRace * pRace = DYNAMIC_CAST(pForm, TESForm, TESRace);
				if(pRace)
				{
					if(pRace->fullName.name.data)
						RegisterUnmanagedString(pFxVal, "fullName", pRace->fullName.name.data);

					if(pRace->editorId.data)
						RegisterUnmanagedString(pFxVal, "editorId", pRace->editorId.data);

					// Spells
					GFxValue spells;
					movieView->CreateArray(&spells);
					for(int i = 0; i < pRace->spellList.GetSpellCount(); i++)
					{
						GFxValue spell;
						movieView->CreateObject(&spell);
						scaleformExtend::FormData(&spell, movieView, pRace->spellList.GetNthSpell(i), bRecursive ? bExtra : false, bRecursive);
						spells.PushBack(&spell);
					}
					pFxVal->SetMember("spells", &spells);

					// Shouts
					GFxValue shouts;
					movieView->CreateArray(&shouts);
					for(int i = 0; i < pRace->spellList.GetShoutCount(); i++)
					{
						GFxValue shout;
						movieView->CreateObject(&shout);
						scaleformExtend::FormData(&shout, movieView, pRace->spellList.GetNthShout(i), bRecursive ? bExtra : false, bRecursive);
						shouts.PushBack(&shout);
					}
					pFxVal->SetMember("shouts", &shouts);

					GFxValue bonuses;
					movieView->CreateArray(&bonuses);
					for(int i = 0; i < TESRace::kRace_NumSkillBonuses; i++)
					{
						GFxValue skillBonus;
						movieView->CreateObject(&skillBonus);
						RegisterNumber(&skillBonus, "skill", pRace->data.skillBonus[i].skill);
						RegisterNumber(&skillBonus, "bonus", pRace->data.skillBonus[i].bonus);
						bonuses.PushBack(&skillBonus);
					}
					pFxVal->SetMember("skillBonuses", &bonuses);
				}
			}
			break;
		case kFormType_NPC:
			{
				TESNPC * pNPC = DYNAMIC_CAST(pForm, TESForm, TESNPC);
				if(pNPC)
				{
					if(pNPC->fullName.name.data)
						RegisterUnmanagedString(pFxVal, "fullName", pNPC->fullName.name.data);
					if(pNPC->shortName.data)
						RegisterUnmanagedString(pFxVal, "shortName", pNPC->shortName.data);

					RegisterNumber(pFxVal, "weight", pNPC->weight);

					bool isLevelMult = (pNPC->actorData.flags & TESActorBaseData::kFlag_PCLevelMult) == TESActorBaseData::kFlag_PCLevelMult;
					if(isLevelMult)
						RegisterNumber(pFxVal, "levelMult", (double)pNPC->actorData.level / 1000.0);
					else
						RegisterNumber(pFxVal, "level", (double)pNPC->actorData.level);

					RegisterNumber(pFxVal, "minLevel", (double)pNPC->actorData.minLevel);
					RegisterNumber(pFxVal, "maxLevel", (double)pNPC->actorData.maxLevel);

					GFxValue race;
					movieView->CreateObject(&race);
					scaleformExtend::FormData(&race, movieView, pNPC->race.race, bRecursive ? bExtra : false, bRecursive);
					pFxVal->SetMember("race", &race);

					// Spells
					GFxValue spells;
					movieView->CreateArray(&spells);
					for(int i = 0; i < pNPC->spellList.GetSpellCount(); i++)
					{
						GFxValue spell;
						movieView->CreateObject(&spell);
						scaleformExtend::FormData(&spell, movieView, pNPC->spellList.GetNthSpell(i), bRecursive ? bExtra : false, bRecursive);
						spells.PushBack(&spell);
					}
					pFxVal->SetMember("spells", &spells);

					// Shouts
					GFxValue shouts;
					movieView->CreateArray(&shouts);
					for(int i = 0; i < pNPC->spellList.GetShoutCount(); i++)
					{
						GFxValue shout;
						movieView->CreateObject(&shout);
						scaleformExtend::FormData(&shout, movieView, pNPC->spellList.GetNthShout(i), bRecursive ? bExtra : false, bRecursive);
						shouts.PushBack(&shout);
					}
					pFxVal->SetMember("shouts", &shouts);
				}
			}
			break;

		case kFormType_Character:
			{
				Actor * pActor = DYNAMIC_CAST(pForm, TESForm, Actor);
				if(pActor)
				{
					// ActorBase as Object
					GFxValue actorBase;
					movieView->CreateObject(&actorBase);

					scaleformExtend::FormData(&actorBase, movieView, pActor->baseForm, bRecursive ? bExtra : false, bRecursive);
					pFxVal->SetMember("actorBase", &actorBase);

					RegisterUnmanagedString(pFxVal, "fullName", CALL_MEMBER_FN(pActor, GetReferenceName)());

					// Spells as Array
					GFxValue addedSpells;
					movieView->CreateArray(&addedSpells);

					for(int i = 0; i < pActor->addedSpells.Length(); i++)
					{
						GFxValue spell;
						movieView->CreateObject(&spell);
						scaleformExtend::FormData(&spell, movieView, pActor->addedSpells.Get(i), bRecursive ? bExtra : false, bRecursive);
						addedSpells.PushBack(&spell);
					}

					pFxVal->SetMember("spells", &addedSpells);

					// ActiveEffects as Array
					GFxValue activeEffects;
					movieView->CreateArray(&activeEffects);

					tList<ActiveEffect> * effects = pActor->magicTarget.GetActiveEffects();
					if(effects)
					{
						for(int i = 0; i < effects->Count(); i++)
						{
							GFxValue effect;
							movieView->CreateObject(&effect);

							ActiveEffect * pEffect = effects->GetNthItem(i);

							if(pEffect->item)
								scaleformExtend::MagicItemData(&effect, movieView, pEffect->item, bRecursive ? bExtra : false, bRecursive);

							RegisterNumber(&effect, "elapsed", pEffect->elapsed);
							RegisterNumber(&effect, "duration", pEffect->duration);
							RegisterNumber(&effect, "magnitude", pEffect->magnitude);
							RegisterBool(&effect, "inactive", (pEffect->flags & ActiveEffect::kFlag_Inactive) == ActiveEffect::kFlag_Inactive);
							
							// ActiveEffect
							if(pEffect->effect && pEffect->effect->mgef)
								scaleformExtend::MagicItemData(&effect, movieView, pEffect->effect->mgef, bRecursive ? bExtra : false, bRecursive);
							
							activeEffects.PushBack(&effect);
						}
					}
					pFxVal->SetMember("activeEffects", &activeEffects);

					GFxValue actorValues;
					movieView->CreateArray(&actorValues);

					for(int i = 0; i < ActorValueList::kNumActorValues; i++)
					{
						GFxValue actorValue;
						movieView->CreateObject(&actorValue);
						RegisterNumber(&actorValue, "current", pActor->actorValueOwner.GetCurrent(i));
						RegisterNumber(&actorValue, "maximum", pActor->actorValueOwner.GetMaximum(i));
						RegisterNumber(&actorValue, "base", pActor->actorValueOwner.GetBase(i));
						actorValues.PushBack(&actorValue);
					}

					pFxVal->SetMember("actorValues", &actorValues);
				}
				PlayerCharacter* pPC = DYNAMIC_CAST(pForm, TESForm, PlayerCharacter);
				if(pPC)
				{
					RegisterNumber(pFxVal, "perkPoints", (double)pPC->numPerkPoints);
				}
			}
			break;

		default:
			break;
		}
	}

	void FormListData(GFxValue * pFxVal, GFxMovieView * movieView, TESForm * pForm, bool bExtra, bool bRecursive)
	{
		if(!pForm || !pFxVal || !pFxVal->IsObject())
			return;

		switch(pForm->GetFormType())
		{
		case kFormType_List:
			{
				BGSListForm * formList = DYNAMIC_CAST(pForm, TESForm, BGSListForm);
				if(formList)
				{
					GFxValue formArray;
					movieView->CreateArray(&formArray);

					FormListExtender extender(&formArray, movieView, bExtra, bRecursive);
					formList->Visit(extender);

					pFxVal->SetMember("forms", &formArray);
				}
			}
			break;
		default:
			break;
		}
	}

	void MiscData(GFxValue * pFxVal, GFxMovieView * movieView, TESForm * pForm, bool bExtra, bool bRecursive)
	{
		if(!pForm || !pFxVal || !pFxVal->IsObject())
			return;

		switch(pForm->GetFormType())
		{
		case kFormType_Message:
			{
				BGSMessage * message = DYNAMIC_CAST(pForm, TESForm, BGSMessage);
				if(message)
				{
					GFxValue btnArray;
					movieView->CreateArray(&btnArray);
					BSFixedString * btnText;
					for(int i = 0; i < message->buttons.Count(); i++)
					{
						GFxValue fxValue;
						btnText = message->buttons.GetNthItem(i);
						if(btnText) {
							fxValue.SetString(btnText->data);
							btnArray.PushBack(&fxValue);
						}
					}

					pFxVal->SetMember("buttons", &btnArray);
					RegisterUnmanagedString(pFxVal, "message", message->GetDescription().c_str());
				}
			}
			break;
		case kFormType_Quest:
			{
				TESQuest * quest = DYNAMIC_CAST(pForm, TESForm, TESQuest);
				if(quest)
				{
					RegisterUnmanagedString(pFxVal, "fullName", quest->fullName.name.data);
					RegisterNumber(pFxVal, "flags", quest->unk0D8.flags);
					RegisterNumber(pFxVal, "priority", quest->unk0D8.priority);
					RegisterUnmanagedString(pFxVal, "editorId", quest->questID.Get());

					GFxValue aliasArray;

					movieView->CreateArray(&aliasArray);
					
					for(int i = 0; i < quest->aliases.count; i++)
					{
						BGSBaseAlias* alias;
						if(quest->aliases.GetNthItem(i, alias)) {
							GFxValue arrArg;
							movieView->CreateObject(&arrArg);
							RegisterUnmanagedString(&arrArg, "name", alias->name.data);
							RegisterNumber(&arrArg, "id", alias->aliasId);
							RegisterNumber(&arrArg, "flags", alias->flags);
							aliasArray.PushBack(&arrArg);
						}
					}

					pFxVal->SetMember("aliases", &aliasArray);
				}
			}
			break;
		case kFormType_HeadPart:
			{
				BGSHeadPart * headPart = DYNAMIC_CAST(pForm, TESForm, BGSHeadPart);
				if(headPart)
				{
					RegisterUnmanagedString(pFxVal, "fullName", headPart->fullName.name.data);
					RegisterUnmanagedString(pFxVal, "partName", headPart->partName.data);
					RegisterNumber(pFxVal, "partFlags", headPart->partFlags);
					RegisterNumber(pFxVal, "partType", headPart->type);

					RegisterUnmanagedString(pFxVal, "modelPath", headPart->model.GetModelName());
					RegisterUnmanagedString(pFxVal, "chargenMorphPath", headPart->chargenMorph.GetModelName());
					RegisterUnmanagedString(pFxVal, "raceMorphPath", headPart->raceMorph.GetModelName());

					GFxValue extraParts;
					movieView->CreateArray(&extraParts);
					for(int i = 0; i < headPart->extraParts.count; i++)
					{
						BGSHeadPart* extraPart;
						if(headPart->extraParts.GetNthItem(i, extraPart)) {
							GFxValue arrArg;
							movieView->CreateObject(&arrArg);
							scaleformExtend::FormData(&arrArg, movieView, extraPart, bRecursive ? bExtra : false, bRecursive);
							extraParts.PushBack(&arrArg);
						}
					}

					pFxVal->SetMember("extraParts", &extraParts);

					GFxValue validRaces;
					movieView->CreateObject(&validRaces);
					scaleformExtend::FormData(&validRaces, movieView, headPart->validRaces, bRecursive ? bExtra : false, bRecursive);
					pFxVal->SetMember("validRaces", &validRaces);

					GFxValue textureSet;
					movieView->CreateObject(&textureSet);
					scaleformExtend::FormData(&textureSet, movieView, headPart->textureSet, bRecursive ? bExtra : false, bRecursive);
					pFxVal->SetMember("textureSet", &textureSet);
				}
			}
			break;
		case kFormType_TextureSet:
			{
				BGSTextureSet * textureSet = DYNAMIC_CAST(pForm, TESForm, BGSTextureSet);
				if(textureSet)
				{
					GFxValue textureArray;
					movieView->CreateArray(&textureArray);
					for(int i = 0; i < BGSTextureSet::kNumTextures; i++)
					{
						GFxValue strArg;
						strArg.SetString(textureSet->texturePaths[i].str.data);
						textureArray.PushBack(&strArg);
					}

					pFxVal->SetMember("textures", &textureArray);
				}
			}
			break;
		default:
			break;
		}
	}

	// Convenience function, maybe combine all other functions into this one anyway?
	// bExtra - Adds all extra data other than FormId and FormType
	// bRecursive - Processes child forms
	void FormData(GFxValue * pFxVal, GFxMovieView * movieView, TESForm * pForm, bool bExtra, bool bRecursive)
	{
		if(!pForm || !pFxVal || !pFxVal->IsObject())
			return;

		scaleformExtend::CommonItemData(pFxVal, pForm);
		if(bExtra) {
			scaleformExtend::StandardItemData(pFxVal, pForm);
			scaleformExtend::MagicItemData(pFxVal, movieView, pForm, bExtra, bRecursive);
			scaleformExtend::ActorData(pFxVal, movieView, pForm, bExtra, bRecursive);
			scaleformExtend::MiscData(pFxVal, movieView, pForm, bExtra, bRecursive);
		}
		scaleformExtend::FormListData(pFxVal, movieView, pForm, bExtra, bRecursive);
	}

	void InventoryData(GFxValue * pFxVal, GFxMovieView * movieView, InventoryEntryData * objDesc)
	{
		TESForm	* pForm = objDesc->type;

		if(!pForm || !pFxVal || !pFxVal->IsObject())
			return;

		PlayerCharacter	* pPC = *g_thePlayer;

		BGSKeywordForm	* keywords = DYNAMIC_CAST(pForm, TESForm, BGSKeywordForm);
		if(keywords)
			RegisterKeywords(pFxVal, movieView, keywords);

		switch(pForm->GetFormType())
		{
		case kFormType_Armor:
			{
				TESObjectARMO * pArmor = DYNAMIC_CAST(pForm, TESForm, TESObjectARMO);
				if(pArmor)
				{
					double armorValue = CALL_MEMBER_FN(pPC, GetArmorValue)(objDesc);
					armorValue = round(armorValue);
					RegisterNumber(pFxVal, "armor", armorValue);
				}
			}
			break;

		case kFormType_Weapon:
			{
				TESObjectWEAP * pWeapon = DYNAMIC_CAST(pForm, TESForm, TESObjectWEAP);
				if(pWeapon)
				{
					double damage = CALL_MEMBER_FN(pPC, GetDamage)(objDesc);
					damage = round(damage);
					RegisterNumber(pFxVal, "damage", damage);
				}
			}
			break;

		case kFormType_Ammo:
			{
				TESAmmo * pAmmo = DYNAMIC_CAST(pForm, TESForm, TESAmmo);
				if(pAmmo)
				{
					double damage = CALL_MEMBER_FN(pPC, GetDamage)(objDesc);
					damage = round(damage);
					RegisterNumber(pFxVal, "damage", damage);
				}
			}
			break;

		default:
			break;
		}
	}

	// SkyUI currently uses the itemcard info in ItemMenus to retrieve this data.
	// This should replace the hack in future menus.
	void ItemInfoData(GFxValue* pFxVal, InventoryEntryData * pEntry)
	{
		if(!pFxVal || !pEntry || !pFxVal->IsObject())
			return;

		RegisterNumber(pFxVal, "value", (double)CALL_MEMBER_FN(pEntry,GetValue)());
		RegisterNumber(pFxVal, "weight", GetFormWeight(pEntry->type));
		RegisterNumber(pFxVal, "isStolen", ! CALL_MEMBER_FN(pEntry,IsOwnedBy)(*g_thePlayer, 1));
	}

	void CraftDisenchantData(GFxValue * pFxVal, GFxMovieView * movieView, InventoryEntryData * pEntry)
	{
		if(!pEntry || !pEntry->type || !pFxVal || !pFxVal->IsObject())
			return;

		EnchantmentItem* enchantment = NULL;

		TESEnchantableForm * enchantable = DYNAMIC_CAST(pEntry->type, TESForm, TESEnchantableForm);
		if (enchantable && enchantable->enchantment)
		{
			enchantment = enchantable->enchantment;
		}
		else if (pEntry->extendDataList)
		{
			for (ExtendDataList::Iterator it = pEntry->extendDataList->Begin(); !it.End(); ++it)
			{
				BaseExtraList * pExtraDataList = it.Get();

				if (pExtraDataList)
				{
					ExtraEnchantment* extraEnchant = static_cast<ExtraEnchantment*>(pExtraDataList->GetByType(kExtraData_Enchantment));
					if (extraEnchant)
					{
						enchantment = extraEnchant->enchant;
						break;
					}
				}
			}
		}

		if (enchantment)
		{
			RegisterUnmanagedString(pFxVal, "effectName", enchantment->fullName.name.data);
		}
	}

	enum
	{
		kCustomAlchemyFlag_None = 0x0,
		kCustomAlchemyFlag_Any = 0x1,
		kCustomAlchemyFlag_Beneficial = 0x2,
		kCustomAlchemyFlag_Harmful = 0x4,
		kCustomAlchemyFlag_Other = 0x8
	};

	void AlchemyCategoryArgs(AlchemyEffectCategory* effectArray, GFxValue * args, UInt32 numArgs)
	{
		AlchemyEffectCategory* curEffect = effectArray;

		// Replace the "flag" value (at offset 1) with a new value.
		// Previously, flag was just an incrementing number 1 to N+1, no special cases.
		// We can easily calculate this number in AS, so flag can be used for other things.

		const UInt32 kStride = 3;
		const UInt32 kFlagOffset = 1;

		// Start at kStride, because [0 .. kStride-1] is the "INGREDIENTS" entry
		for (UInt32 i=kStride + kFlagOffset; i<numArgs; i+=kStride)
		{
			TESForm* form = LookupFormByID(curEffect->formId);

			UInt32 newFlag = kCustomAlchemyFlag_None;

			EffectSetting * effect = DYNAMIC_CAST(form, TESForm, EffectSetting);
			if (effect)
			{
				UInt32 archetype = effect->properties.archetype;
				UInt32 bDetrimental = (effect->properties.flags & EffectSetting::Properties::kEffectType_Detrimental) != 0;

				switch (archetype)
				{
				case EffectSetting::Properties::kArchetype_ValueMod:
				case EffectSetting::Properties::kArchetype_DualValueMod:
				case EffectSetting::Properties::kArchetype_PeakValueMod:
				{
					newFlag = bDetrimental ? kCustomAlchemyFlag_Harmful : kCustomAlchemyFlag_Beneficial;
					break;
				}
				case EffectSetting::Properties::kArchetype_Absorb:
				case EffectSetting::Properties::kArchetype_CureDisease:
				case EffectSetting::Properties::kArchetype_Invisibility:
				case EffectSetting::Properties::kArchetype_CureParalysis:
				case EffectSetting::Properties::kArchetype_CureAddiction:
				case EffectSetting::Properties::kArchetype_CurePoison:
				case EffectSetting::Properties::kArchetype_Dispel:
				{
					newFlag = kCustomAlchemyFlag_Beneficial;
					break;
				}
				case EffectSetting::Properties::kArchetype_Frenzy:
				case EffectSetting::Properties::kArchetype_Calm:
				case EffectSetting::Properties::kArchetype_Demoralize:
				case EffectSetting::Properties::kArchetype_Paralysis:
				{
					newFlag = kCustomAlchemyFlag_Harmful;
					break;
				}
				default:
				{
					newFlag = kCustomAlchemyFlag_Other;
					break;
				}
				}
			}
			
			args[i].SetNumber(newFlag);

			curEffect++;
		}
	}
}
