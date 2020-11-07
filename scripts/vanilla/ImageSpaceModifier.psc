Scriptname ImageSpaceModifier extends Form Hidden

; Applies this modifier with the specified strength (which is not clamped)
Function Apply(float afStrength = 1.0) native

; Adds this modifier to the cross-fade chain, removing the previous modifier, and fading over the specified duration (in seconds)
Function ApplyCrossFade(float afFadeDuration = 1.0) native

; Disables this modifier and enables the new one, "popping" between the two. Will not interrupt any running cross-fade (if the modifiers aren't the ones fading)
Function PopTo(ImageSpaceModifier akNewModifier, float afStrength = 1.0) native

; Removes this modifier
Function Remove() native

; Removes whatever modifier is on the cross-fade chain, fading it out
Function RemoveCrossFade(float afFadeDuration = 1.0) native global