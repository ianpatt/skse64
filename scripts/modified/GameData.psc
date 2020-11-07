Scriptname GameData Hidden
 
; Keywords are AND operations, must have all listed keywords
; IgnoreTemplates will exclude items that are inherited from other items with slightly altered stats
; IgnoreEnchantments will exclude any item with an enchantment
; WeaponTypes are a bitfield, will filter weapons by type
; Add together to filter by multiple types
int Property WeaponTypeHandToHand = 1 AutoReadOnly
int Property WeaponTypeOneHandSword = 2 AutoReadOnly
int Property WeaponTypeOneHandDagger = 4 AutoReadOnly
int Property WeaponTypeOneHandAxe = 8 AutoReadOnly
int Property WeaponTypeOneHandMace = 16 AutoReadOnly
int Property WeaponTypeTwoHandSword = 32 AutoReadOnly
int Property WeaponTypeTwoHandAxe = 64 AutoReadOnly
int Property WeaponTypeBow = 128 AutoReadOnly
int Property WeaponTypeStaff = 256 AutoReadOnly
int Property WeaponTypeCrossbow = 512 AutoReadOnly
 
Form[] Function GetAllWeapons(string modName, Keyword[] keywords = None, bool playable = true, bool ignoreTemplates = true, bool ignoreEnchantments = true, bool onlyEnchanted = false, int weaponTypes = 0xFFFFFFFF) global native
 
Form[] Function GetAllArmor(string modName, Keyword[] keywords = None, bool playable = true, bool ignoreTemplates = true, bool ignoreEnchantments = true, bool onlyEnchanted = false, bool ignoreSkin = true) global native
 
Form[] Function GetAllAmmo(string modName, Keyword[] keywords = None, bool playable = true) global native
 
Form[] Function GetAllBooks(string modName, Keyword[] keywords = None, bool regular = true, bool spell = false, bool skill = false) global native
 
Form[] Function GetAllPotions(string modName, Keyword[] keywords = None, bool potions = true, bool food = false, bool poison = false) global native
 
Form[] Function GetAllIngredients(string modName, Keyword[] keywords = None) global native
 
Form[] Function GetAllScrolls(string modName, Keyword[] keywords = None) global native
 
Form[] Function GetAllKeys(string modName, Keyword[] keywords = None) global native
 
Form[] Function GetAllMiscItems(string modName, Keyword[] keywords = None) global native