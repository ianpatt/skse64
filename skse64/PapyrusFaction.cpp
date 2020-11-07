#include "PapyrusFaction.h"

#include "GameObjects.h"
#include "GameData.h"

namespace papyrusFaction
{
	bool IsFactionFlagSet(TESFaction* thisFaction, UInt32 flag)
	{
		return (thisFaction) ? (thisFaction->factionFlags & flag) == flag : false;
	}

	void SetFactionFlag(TESFaction* thisFaction, UInt32 flag)
	{
		if (thisFaction) {
			thisFaction->factionFlags |= flag;
		}
	}

	void ClearFactionFlag(TESFaction* thisFaction, UInt32 flag)
	{
		if (thisFaction) {
			thisFaction->factionFlags &= ~flag;
		}
	}

	bool OnlyBuysStolenItems(TESFaction* thisFaction)
	{
		return (thisFaction) ? (thisFaction->vendorData.onlyBuysStolenItems == 1) : false;
	}

	void SetOnlyBuysStolenItems(TESFaction* thisFaction, bool buystolen)
	{
		if(thisFaction) {
			thisFaction->vendorData.onlyBuysStolenItems = (buystolen ? 1 : 0);
		}
	}

	UInt32 GetVendorStartHour(TESFaction* thisFaction)
	{
		return (thisFaction) ? thisFaction->vendorData.startHour : 0;
	}

	void SetVendorStartHour(TESFaction* thisFaction, UInt32 hour)
	{
		if(thisFaction) {
			if(hour > USHRT_MAX)
				hour = USHRT_MAX;
			thisFaction->vendorData.startHour = hour;
		}
	}

	UInt32 GetVendorEndHour(TESFaction* thisFaction)
	{
		return (thisFaction) ? thisFaction->vendorData.endHour : 0;
	}

	void SetVendorEndHour(TESFaction* thisFaction, UInt32 hour)
	{
		if(thisFaction) {
			if(hour > USHRT_MAX)
				hour = USHRT_MAX;
			thisFaction->vendorData.endHour = hour;
		}
	}

	UInt32 GetVendorRadius(TESFaction* thisFaction)
	{
		return (thisFaction) ? thisFaction->vendorData.radius : 0;
	}

	void SetVendorRadius(TESFaction* thisFaction, UInt32 radius)
	{
		if(thisFaction) {
			if(radius > USHRT_MAX)
				radius = USHRT_MAX;
			thisFaction->vendorData.radius = radius;
		}
	}

	TESObjectREFR * GetMerchantContainer(TESFaction* thisFaction)
	{
		return (thisFaction) ? thisFaction->vendorData.merchantContainer : NULL;
	}

	void SetMerchantContainer(TESFaction* thisFaction, TESObjectREFR * reference)
	{
		if(thisFaction && reference) {
			thisFaction->vendorData.merchantContainer = reference;
		}
	}

	bool IsNotSellBuy(TESFaction* thisFaction)
	{
		return (thisFaction) ? (thisFaction->vendorData.notSellBuy == 1) : false;
	}

	void SetNotSellBuy(TESFaction* thisFaction, bool notSellBuy)
	{
		if(thisFaction) {
			thisFaction->vendorData.notSellBuy = (notSellBuy ? 1 : 0);
		}
	}

	BGSListForm * GetBuySellList(TESFaction* thisFaction)
	{
		return (thisFaction) ? thisFaction->vendorData.buySellList : NULL;
	}

	void SetBuySellList(TESFaction* thisFaction, BGSListForm * vendorList)
	{
		if(thisFaction && vendorList) {
			thisFaction->vendorData.buySellList = vendorList;
		}
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusFaction::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction1 <TESFaction, bool, UInt32>("IsFactionFlagSet", "Faction", papyrusFaction::IsFactionFlagSet, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESFaction, void, UInt32>("SetFactionFlag", "Faction", papyrusFaction::SetFactionFlag, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESFaction, void, UInt32>("ClearFactionFlag", "Faction", papyrusFaction::ClearFactionFlag, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESFaction, bool>("OnlyBuysStolenItems", "Faction", papyrusFaction::OnlyBuysStolenItems, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESFaction, void, bool>("SetOnlyBuysStolenItems", "Faction", papyrusFaction::SetOnlyBuysStolenItems, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESFaction, UInt32>("GetVendorStartHour", "Faction", papyrusFaction::GetVendorStartHour, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESFaction, void, UInt32>("SetVendorStartHour", "Faction", papyrusFaction::SetVendorStartHour, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESFaction, UInt32>("GetVendorEndHour", "Faction", papyrusFaction::GetVendorEndHour, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESFaction, void, UInt32>("SetVendorEndHour", "Faction", papyrusFaction::SetVendorEndHour, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESFaction, UInt32>("GetVendorRadius", "Faction", papyrusFaction::GetVendorRadius, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESFaction, void, UInt32>("SetVendorRadius", "Faction", papyrusFaction::SetVendorRadius, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESFaction, TESObjectREFR*>("GetMerchantContainer", "Faction", papyrusFaction::GetMerchantContainer, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESFaction, void, TESObjectREFR*>("SetMerchantContainer", "Faction", papyrusFaction::SetMerchantContainer, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESFaction, bool>("IsNotSellBuy", "Faction", papyrusFaction::IsNotSellBuy, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESFaction, void, bool>("SetNotSellBuy", "Faction", papyrusFaction::SetNotSellBuy, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESFaction, BGSListForm*>("GetBuySellList", "Faction", papyrusFaction::GetBuySellList, registry));

	registry->RegisterFunction(
		new NativeFunction1 <TESFaction, void, BGSListForm*>("SetBuySellList", "Faction", papyrusFaction::SetBuySellList, registry));
}