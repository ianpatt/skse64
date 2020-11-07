Scriptname WornObject Hidden

; These functions operate directly on
; worn items within the inventory
; Valid Hand Slot:
; 0 - Left
; 1 - Right
; Valid Slot Masks:
; See Armor.psc
; Use zero when using hand slot

; Tempering
float Function GetItemHealthPercent(Actor akActor, int handSlot, int slotMask) global native
Function SetItemHealthPercent(Actor akActor, int handSlot, int slotMask, float health) global native

; Charges
; Only works on items that have user-enchants
Function SetItemMaxCharge(Actor akActor, int handSlot, int slotMask, float maxCharge) global native

; Works on any enchanted item
float Function GetItemMaxCharge(Actor akActor, int handSlot, int slotMask) global native

float Function GetItemCharge(Actor akActor, int handSlot, int slotMask) global native

; Use LeftItemCharge/RightItemCharge ActorValues instead
;Function SetItemCharge(Actor akActor, int handSlot, int slotMask, float charge) global native

; Returns the name of this reference
; this is the name that is displayed
string Function GetDisplayName(Actor akActor, int handSlot, int slotMask) global native

; Sets a reference's display name
; returns false if force is false and the reference
; is held by an alias using 'Stored Text' or 'Uses Stored Text'
; Text Replacement does not use this name and may be lost if forced
bool Function SetDisplayName(Actor akActor, int handSlot, int slotMask, string name, bool force = false) global native

; Returns the player-made enchantment if there is one
Enchantment Function GetEnchantment(Actor akActor, int handSlot, int slotMask) global native

; Changes an item's player-made enchantment to something else
; None enchantment will remove the existing enchantment
; does not delete the custom enchantment, only removes it
Function SetEnchantment(Actor akActor, int handSlot, int slotMask, Enchantment source, float maxCharge) global native

; Creates a new enchantment on the item given the specified parameters
; all arrays must be the same size
; created enchantments are not purged from the save when removed or overwritten
; exact same enchantments are re-used by the game
Function CreateEnchantment(Actor akActor, int handSlot, int slotMask, float maxCharge, MagicEffect[] effects, float[] magnitudes, int[] areas, int[] durations) global native

; Returns the number of ref aliases holding this reference
int Function GetNumReferenceAliases(Actor akActor, int handSlot, int slotMask) global native

; Returns the nth ReferenceAlias holding this reference
ReferenceAlias Function GetNthReferenceAlias(Actor akActor, int handSlot, int slotMask, int n) global native

; Returns the poison on the specified item
Potion Function GetPoison(Actor akActor, int handSlot, int slotMask) global native

; Returns all of the ReferenceAlias holding this reference
ReferenceAlias[] Function GetReferenceAliases(Actor akActor, int handSlot, int slotMask) global native