; get/set the CombatStyle of the actor
CombatStyle Function GetCombatStyle() native
Function SetCombatStyle(CombatStyle cs) native

; Get the Outfit of the actor
Outfit Function GetOutfit(bool bSleepOutfit = false) native

; set the Class of the actor
Function SetClass(Class c) native

; Get/Set the actors body height
float Function GetHeight() native
Function SetHeight(float height) native

; Get/Set the actors body weight
float Function GetWeight() native
Function SetWeight(float weight) native

; Get/Set actors HeadPart by index
int Function GetNumHeadParts() native
HeadPart Function GetNthHeadPart(int slotPart) native
Function SetNthHeadPart(HeadPart headPart, int slotPart) native
int Function GetIndexOfHeadPartByType(int type) native

; These functions are READ-ONLY they are for accessing the
; HeadPart list when the ActorBase's Race has been overlayed
; with another race (e.g. Vampires)
int Function GetNumOverlayHeadParts() native
HeadPart Function GetNthOverlayHeadPart(int slotPart) native
int Function GetIndexOfOverlayHeadPartByType(int type) native

; Get/Set actors face morph value by index
float Function GetFaceMorph(int index) native
Function SetFaceMorph(float value, int index) native

; Get/Set actors facemorph preset by index
; 0 - Nose
; 1 - ??
; 2 - Mouth
; 3 - Eyes
int Function GetFacePreset(int index) native
Function SetFacePreset(int value, int index) native

ColorForm Function GetHairColor() native
Function SetHairColor(ColorForm color) native

; returns the number of spells defined in the base actor form
int Function GetSpellCount() native

; returns the specified spell defined in the base actor  form
Spell Function GetNthSpell(int n) native

; returns the face textureset of the actor (Player Only?)
TextureSet Function GetFaceTextureSet() native
Function SetFaceTextureSet(TextureSet textures) native

; Gets/sets the Actor's voicetype
VoiceType Function GetVoiceType() native
Function SetVoiceType(VoiceType nVoice) native

; Gets/sets the skin of the actorbase
Armor Function GetSkin() native
Function SetSkin(Armor skin) native

; Gets/sets the far away skin of the actorbase
Armor Function GetSkinFar() native
Function SetSkinFar(Armor skin) native

; Gets the root template of the ActorBase
ActorBase Function GetTemplate() native