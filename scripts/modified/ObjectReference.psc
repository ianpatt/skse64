
; Container-only functions

;Quietly adds or removes count[] instances of every form in items[] to the container.
;Intended as a fast way to add large quantity of items to a container without sending OnItemAdded event notifications
;and causing stack dumps from an overwhelmed VM. Returns true if items were added, false otherwise.
;Note that:
;1.Extra data will not be copied.
;2.If removing items from an actor, the caller must make sure no removed item is equipped or issues will ensue.
;3.Both items[] and count[] must have the exact same size.
bool Function AddItemsBulk(Form[] items, int[] count, bool remove = false) native

;Returns the number of instances of the passed form, present in the container.
;In the first call, container items are cached and subsequent calls return the cached value instead of querying the container again.
;Intended as a faster alternative to GetItemCount, to be used when several calls are required and the contents of the
;accessed container is not expected to change.
;If items is None, or if called on a different container, the cache is cleared and its contents refreshed.
int Function GetItemCountCached(Form item) native

int Function GetNumItems() native
Form Function GetNthForm(int index) native
float Function GetTotalItemWeight() native
float Function GetTotalArmorWeight() native

;Restores the charge of the passed weapon if found in the container.
;If "all" is set to true, every enchanted instance found will be recharged, otherwise only the first one will.
;Returns how much charge was restored to the weapon(s).
float Function RestoreItemCharge(Weapon wepn, bool all) native

;Returns the final cost of the item as shown in the UI, after all factors like enchantments and tempering are calculated.
;If there's more than one instance of the same form in the container, only the first one is returned
int Function GetDisplayValue(form item) native

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
