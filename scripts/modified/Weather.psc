
; Returns the sun glare percentage
float Function GetSunGlare() native

; Returns the sun damage percentage
float Function GetSunDamage() native

; Returns the wind direction in degrees (0-360)
float Function GetWindDirection() native

; Returns the wind direction range in degrees (0-180)
float Function GetWindDirectionRange() native

; 0 - Near
; 1 - Far
; 2 - Power
; 3 - Max
float Function GetFogDistance(bool day, int type) native
