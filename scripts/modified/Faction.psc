
int property kFaction_HiddenFromNPC				= 0x00000001 AutoReadOnly
int property kFaction_SpecialCombat				= 0x00000002 AutoReadOnly
int property kFaction_TrackCrime				= 0x00000010 AutoReadOnly
int property kFaction_IgnoreMurder				= 0x00000020 AutoReadOnly
int property kFaction_IgnoreAssault				= 0x00000040 AutoReadOnly
int property kFaction_IgnoreStealing			= 0x00000080 AutoReadOnly
int property kFaction_IgnoreTrespass			= 0x00000100 AutoReadOnly
int property kFaction_NoReportCrime				= 0x00000200 AutoReadOnly
int property kFaction_CrimeGoldDefaults			= 0x00000400 AutoReadOnly
int property kFaction_IgnorePickpocket			= 0x00000800 AutoReadOnly
int property kFaction_Vendor					= 0x00001000 AutoReadOnly
int property kFaction_CanBeOwner				= 0x00002000 AutoReadOnly
int property kFaction_IgnoreWerewolf			= 0x00004000 AutoReadOnly

; Not recommended unless the faction was previously a vendor
; due to the faction not having a package location the vendor
; may not be able to set up shop anywhere at all
Function MakeVendor()
	SetFactionFlag(self.kFaction_Vendor)
EndFunction

bool Function IsVendor()
	return IsFactionFlagSet(self.kFaction_Vendor)
EndFunction

Function ClearVendor()
	ClearFactionFlag(self.kFaction_Vendor)
EndFunction

bool Function IsFactionFlagSet(int flag) native
Function SetFactionFlag(int flag) native
Function ClearFactionFlag(int flag) native

bool Function OnlyBuysStolenItems() native
Function SetOnlyBuysStolenItems(bool onlyStolen) native

int Function GetVendorStartHour() native
Function SetVendorStartHour(int hour) native

int Function GetVendorEndHour() native
Function SetVendorEndHour(int hour) native

int Function GetVendorRadius() native
Function SetVendorRadius(int radius) native

ObjectReference Function GetMerchantContainer() native
Function SetMerchantContainer(ObjectReference akContainer) native

bool Function IsNotSellBuy() native
Function SetNotSellBuy(bool notSellBuy) native

FormList Function GetBuySellList() native
Function SetBuySellList(FormList akList) native