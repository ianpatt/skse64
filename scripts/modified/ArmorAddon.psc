Scriptname ArmorAddon extends Form Hidden

; returns the model path of the particular model
string Function GetModelPath(bool firstPerson, bool female) native

; sets the model path of the particular model
Function SetModelPath(string path, bool firstPerson, bool female) native

; returns the number of texturesets for the particular model
int Function GetModelNumTextureSets(bool first, bool female) native

; returns the nth textureset for the particular model
TextureSet Function GetModelNthTextureSet(int n, bool first, bool female) native

; sets the nth textureset for the particular model
Function SetModelNthTextureSet(TextureSet texture, int n, bool first, bool female) native

; returns the number of races this armor addon applies to
int Function GetNumAdditionalRaces() native

; returns the nth race this armor addon applies to
Race Function GetNthAdditionalRace(int n) native

; Functions and Flags dealing the BipedObject slot values from the CK
; These are the equivalent of 1 << (SlotMask-30).  Basically
; these are a flags where 30 is the first bit, and 61 is the 31st bit.

; returns the slot mask for the armor addon.
int Function GetSlotMask() native
; sets the slot mask for the armor addon
Function SetSlotMask(int slotMask) native
; adds the specified slotMask to the armor addon
int Function AddSlotToMask(int slotMask) native
; removes the specified slot masks from the armor addon
int Function RemoveSlotFromMask(int slotMask) native

; calculates the equivalent mask value for the slot
; This is a global function, use it directly from Armor as it is faster
int Function GetMaskForSlot(int slot) global
	return Armor.GetMaskForSlot(slot)
EndFunction