Scriptname Weather extends Form Hidden

; Tells the sky to release its overriding weather. 
function ReleaseOverride() native global

; Forces the active weather on the sky to be this weather.
function ForceActive( bool abOverride=false ) native

; Sets the active weather on the sky to be this weather.
function SetActive( bool abOverride=false, bool abAccelerate=false ) native

; Finds a weather from the current region/climate whose classification matches the given one.
; 0 - Pleasant
; 1 - Cloudy
; 2 - Rainy
; 3 - Snow
Weather function FindWeather( int auiType ) native global

; Gets this weather's classification
; -1 - No classification
;  0 - Pleasant
;  1 - Cloudy
;  2 - Rainy
;  3 - Snow
int function GetClassification() native

; Gets the sky's current weather
Weather function GetCurrentWeather() native global

; Gets the sky's outgoing weather
Weather function GetOutgoingWeather() native global

; Gets the transition percentage of the current weather
float function GetCurrentWeatherTransition() native global

; Gets the sky's current mode
; 0 - No sky (SM_NONE)
; 1 - Interior (SM_INTERIOR)
; 2 - Skydome only (SM_SKYDOME_ONLY)
; 3 - Full sky (SM_FULL)
int function GetSkyMode() native global