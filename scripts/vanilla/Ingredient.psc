Scriptname Ingredient extends Form

; Is this ingredient classified as hostile?
bool Function IsHostile() native

; Flags the effect with the given 0 based index as known by the player
Function LearnEffect(int aiIndex) native

; Flags the next unknown effect as known by the player, returning index of effect learned
int Function LearnNextEffect() native

; Flags the all effects as known by the player
Function LearnAllEffects() native