#pragma once

#include "GameTypes.h"

class TESFaction;
class TESObjectREFR;
class BGSListForm;

class VMClassRegistry;

namespace papyrusFaction
{
	void RegisterFuncs(VMClassRegistry* registry);

	bool OnlyBuysStolenItems(TESFaction* thisFaction);
	void SetOnlyBuysStolenItems(TESFaction* thisFaction, bool buystolen);

	UInt32 GetVendorStartHour(TESFaction* thisFaction);
	void SetVendorStartHour(TESFaction* thisFaction, UInt32 hour);

	UInt32 GetVendorEndHour(TESFaction* thisFaction);
	void SetVendorEndHour(TESFaction* thisFaction, UInt32 hour);

	UInt32 GetVendorRadius(TESFaction* thisFaction);
	void SetVendorRadius(TESFaction* thisFaction, UInt32 radius);

	TESObjectREFR * GetMerchantContainer(TESFaction* thisFaction);
	void SetMerchantContainer(TESFaction* thisFaction, TESObjectREFR * reference);


	bool IsNotSellBuy(TESFaction* thisFaction);
	void SetNotSellBuy(TESFaction* thisFaction, bool notSellBuy);

	BGSListForm * GetBuySellList(TESFaction* thisFaction);
	void SetBuySellList(TESFaction* thisFaction, BGSListForm * vendorList);
};
