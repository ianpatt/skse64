Function SetAssociatedSkill(string skill) native

string Function GetResistance() native
Function SetResistance(string skill) native

; Hostile	 		0x00000001
; Recover  			0x00000002
; Detrimental  		0x00000004
; NoHitEvent  		0x00000010
; DispelKeywords  	0x00000100
; NoDuration  		0x00000200
; NoMagnitude  		0x00000400
; NoArea  			0x00000800
; FXPersist  		0x00001000
; GloryVisuals  	0x00004000
; HideInUI  		0x00008000
; NoRecast  		0x00020000
; Magnitude  		0x00200000
; Duration  		0x00400000
; Painless  		0x04000000
; NoHitEffect  		0x08000000
; NoDeathDispel  	0x10000000

bool Function IsEffectFlagSet(int flag) native
Function SetEffectFlag(int flag) native
Function ClearEffectFlag(int flag) native

float Function GetCastTime() native
Function SetCastTime(float castTime) native

int Function GetSkillLevel() native
Function SetSkillLevel(int level) native

int Function GetArea() native
Function SetArea(int area) native

float Function GetSkillUsageMult() native
Function SetSkillUsageMult(float usageMult) native

float Function GetBaseCost() native
Function SetBaseCost(float cost) native

Light Function GetLight() native
Function SetLight(Light obj) native

EffectShader Function GetHitShader() native
Function SetHitShader(EffectShader obj) native

EffectShader Function GetEnchantShader() native
Function SetEnchantShader(EffectShader obj) native

Projectile Function GetProjectile() native
Function SetProjectile(Projectile obj) native

Explosion Function GetExplosion() native
Function SetExplosion(Explosion obj) native

Art Function GetCastingArt() native
Function SetCastingArt(Art obj) native

Art Function GetHitEffectArt() native
Function SetHitEffectArt(Art obj) native

Art Function GetEnchantArt() native
Function SetEnchantArt(Art obj) native

ImpactDataSet Function GetImpactDataSet() native
Function SetImpactDataSet(ImpactDataSet obj) native

Spell Function GetEquipAbility() native
Function SetEquipAbility(Spell obj) native

ImageSpaceModifier Function GetImageSpaceMod() native
Function SetImageSpaceMod(ImageSpaceModifier obj) native

Perk Function GetPerk() native
Function SetPerk(Perk obj) native

int Function GetCastingType() native
; Constant Effect     0
; Fire And Forget     1
; Concentration       2

int Function GetDeliveryType() native
; Self                0
; Contact             1
; Aimed               2
; Target Actor        3
; Target Location     4

; Entries will be None if there is no sound
; will always return an array of size 6
Sound[] Function GetSounds() native
; Draw Sheathe        0
; Charge              1
; Ready               2
; Release             3
; Loop                4
; Hit                 5