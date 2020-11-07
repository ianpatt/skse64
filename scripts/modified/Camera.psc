Scriptname Camera Hidden


; Returns the character's current camera state
; 0 - first person
; 1 - auto vanity
; 2 - VATS
; 3 - free
; 4 - iron sights
; 5 - furniture
; 6 - transition
; 7 - tweenmenu
; 8 - third person 1
; 9 - third person 2
; 10 - horse
; 11 - bleedout
; 12 - dragon
int Function GetCameraState() global native

; Updates the camera when changing Shoulder positions
Function UpdateThirdPerson() global native

; Returns the player's camera FOV
float Function GetWorldFieldOfView() global native
float Function GetWorldFOV() global
	return GetWorldFieldOfView()
EndFunction

; Sets the player's camera FOV
Function SetWorldFieldOfView(float fov) global native
Function SetWorldFOV(float fov) global
	SetWorldFieldOfView(fov)
EndFunction

; Returns the player's camera FOV
float Function GetFirstPersonFieldOfView() global native
float Function GetFirstPersonFOV() global
	return GetFirstPersonFieldOfView()
EndFunction

; Sets the player's camera FOV
Function SetFirstPersonFieldOfView(float fov) global native
Function SetFirstPersonFOV(float fov) global
	SetFirstPersonFieldOfView(fov)
EndFunction