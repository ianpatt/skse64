
; Container-only functions

;Adds or removes count[i] instances of items[i] to the container.
;Intended as a fast way to add large quantity of items to a container in a single operation.
;It has the added benefit of avoiding OnItemAdded/removed events which can cause stack dumps on an overwhelmed VM.
;Returns true if items were added, false otherwise.
;NOTES:
;1. Extra data will not be copied, which makes this unsuitable for adding tempered/enchanted equipment.
;2. Removing equipped items from an actor will result in undefined behavior. Un-equip them first.
;3. If items[] and count[] don't have the same size, the function will perform no task.
;	Usage: player.AddItemsBulk(item_list, items_count)
bool Function AddItemsBulk(Form[] items, int[] count, bool remove = false) native

;Returns how many instances of the passed form are present in the container (-1 if the container was not scanned).
;Container contents are cached in the first call and subsequent calls return the cached values instead of querying the
;container again. Intended as a faster alternative to GetItemCount, to be used when several calls are required in quick
;succession and the contents of the container are not expected to change.
;NOTES:
;If called on a different container from last, the cache is rebuilt from the new container before looking up the item.
;If refresh is true, the cache is rebuilt before looking up the item.
;If item is NONE, the cache is cleared and -1 is returned.
;The function is bottlenecked by frametime, which at a typical avg of 16ms, only allows ~60 calls per second.
;While faster than "GetItemCount", which only manages ~10-14 calls per second, it's still not be ideal if processing
;many items in a time sensitive manner. Consider using the new GetItemsCount in such circumstances.
;	Usage: container.GetItemCountCached(my_item)
int Function GetItemCountCached(Form item, bool rebuild = false) native

;Returns an array, each element containing the number of instances of items[x] present in the container.
;The array is filled in such a way that its first element will contain the count of items' first element, and so on.
;Intended to be used when large quantities of items need to be processed as fast as possible.
;	Usage: container.GetItemsCount(item_list)
Int[] Function GetItemsCount(Form[] items) native

int Function GetNumItems() native
Form Function GetNthForm(int index) native
float Function GetTotalItemWeight() native
float Function GetTotalArmorWeight() native

;Returns the final cost of the item as shown in the UI, after factors like enchantments and tempering are calculated.
;If there's more than one instance of the same form in the container, only the first one is returned.
;	Usage: container.GetDisplayValue(my_item)
int Function GetDisplayValue(Form item) native

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

;Restores the charge of the passed weapon if found in the container.
;If "all" is set to true, every enchanted instance found will be recharged, otherwise only the first one will.
;Returns how much charge was restored to the weapon(s).
;Works only on weapons inside a container. Accepts forms as long as the form can be cast to weapon.
;	Usage: container.RestoreItemCharge(myWeapon)
float Function RestoreItemCharge(Weapon wepn, bool all) native

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