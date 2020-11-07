
int Function GetBaseDamage() native
Function SetBaseDamage(int damage) native

int Function GetCritDamage() native
Function SetCritDamage(int damage) native

float Function GetReach() native
Function SetReach(float reach) native

float Function GetMinRange() native
Function SetMinRange(float minRange) native

float Function GetMaxRange() native
Function SetMaxRange(float maxRange) native

float Function GetSpeed() native
Function SetSpeed(float speed) native

float Function GetStagger() native
Function SetStagger(float stagger) native

int Function GetWeaponType() native
Function SetWeaponType(int type) native

; works on the path to the nif file representing the in-game model of the weapon
string Function GetModelPath() native
Function SetModelPath(string path) native

; works on the path to the nif file representing the icon for the weapon in the inventory
string Function GetIconPath() native
Function SetIconPath(string path) native

; works on the path to the file representing the message icon for the weapon
string Function GetMessageIconPath() native
Function SetMessageIconPath(string path) native

; works on the enchantment associated with the weapon
Enchantment Function GetEnchantment() native
Function SetEnchantment(Enchantment e) native

; works on the enchantment value of the associated weapon
int Function GetEnchantmentValue() native
Function SetEnchantmentValue(int value) native

; works on the weapon model when equipped of the associated weapon
Static Function GetEquippedModel() native
Function SetEquippedModel(Static model) native

; Returns the particular equipslot type
EquipSlot Function GetEquipType() native
Function SetEquipType(EquipSlot type) native

string Function GetSkill() native
Function SetSkill(string skill) native

; DamageResist
; ElectricResist
; FireResist
; FrostResist
; MagicResist
; PoisonResist
string Function GetResist() native
Function SetResist(string resist) native

; works on the spell that applies when critting
Spell Function GetCritEffect() native
Function SetCritEffect(Spell ce) native

; Gets, sets or unsets whether the the crit effect should only occur on death
bool Function GetCritEffectOnDeath() native
Function SetCritEffectOnDeath(bool ceod) native

; Gets/sets the weapons crit multiplier
float Function GetCritMultiplier() native
Function SetCritMultiplier(float crit) native

; returns the weapon template of this weapon
Weapon Function GetTemplate() native

bool Function IsBattleaxe()
	return HasKeywordString("WeapTypeBattleaxe")
endFunction

bool Function IsBow()
	return HasKeywordString("WeapTypeBow")
endFunction

bool Function IsDagger()
	return HasKeywordString("WeapTypeDagger")
endFunction

bool Function IsGreatsword()
	return HasKeywordString("WeapTypeGreatsword")
endFunction

bool Function IsMace()
	return HasKeywordString("WeapTypeMace")
endFunction

bool Function IsStaff()
	return HasKeywordString("WeapTypeStaff")
endFunction

bool Function IsSword()
	return HasKeywordString("WeapTypeSword")
endFunction

bool Function IsWarhammer()
	return HasKeywordString("WeapTypeWarhammer")
endFunction

bool Function IsWarAxe()
	return HasKeywordString("WeapTypeWarAxe")
endFunction
