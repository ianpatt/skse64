
; Container-only functions
int Function GetNumItems() native
Form Function GetNthForm(int index) native
float Function GetTotalItemWeight() native
float Function GetTotalArmorWeight() native

; Tree and Flora only functions
bool Function IsHarvested() native
Function SetHarvested(bool harvested) native

; Tempering
Function SetItemHealthPercent(float health) native

; Charges

; Only works on ObjectReferences that have user-enchants
Function SetItemMaxCharge(float maxCharge) native
; Works on any enchanted item
float Function GetItemMaxCharge() native

float Function GetItemCharge() native
Function SetItemCharge(float charge) native

Function ResetInventory() native

bool Function IsOffLimits() native

; Returns the name of this reference
; this is the name that is displayed
string Function GetDisplayName() native

; Sets a reference's display name
; returns false if force is false and the reference
; is held by an alias using 'Stored Text' or 'Uses Stored Text'
; Text Replacement does not use this name and may be lost if forced
bool Function SetDisplayName(string name, bool force = false) native

; Returns the enable parent object
ObjectReference Function GetEnableParent() native

; Returns the player-made enchantment if there is one
Enchantment Function GetEnchantment() native

; Changes an item's player-made enchantment to something else
; None enchantment will remove the existing enchantment
; does not delete the custom enchantment, only removes it
Function SetEnchantment(Enchantment source, float maxCharge) native

; Creates a new enchantment on the item given the specified parameters
; all arrays must be the same size
; created enchantments are not purged from the save when removed or overwritten
; exact same enchantments are re-used by the game
Function CreateEnchantment(float maxCharge, MagicEffect[] effects, float[] magnitudes, int[] areas, int[] durations) native

; Returns the number of ref aliases holding this reference
int Function GetNumReferenceAliases() native

; Returns the nth ReferenceAlias holding this reference
ReferenceAlias Function GetNthReferenceAlias(int n) native

; Returns the poison applied to the weapon
Potion Function GetPoison() native

; Returns all base forms in the inventory/container into the specified FormList
Function GetAllForms(FormList toFill) native

; Returns all base forms from the container into a new array
Form[] Function GetContainerForms() native

; Returns all of the aliases holding this reference
ReferenceAlias[] Function GetReferenceAliases() native