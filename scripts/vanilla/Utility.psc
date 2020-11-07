Scriptname Utility Hidden

; Converts a float game time (in terms of game days passed) to a string detailing the date
; and time it represents in "MM/DD/YYYY HH:MM" format. A 24-hour clock is used, and the function
; is latent (due to issues in the current architecture with returning strings from code)
string Function GameTimeToString(float afGameTime) native global

; Obtains the current game time in terms of game days passed (same as the global variable)
float Function GetCurrentGameTime() native global

; Obtains the number of seconds since the application started (the same timer that WaitMenuMode uses)
; Does not take into account menu-mode, or VM frozen time
; Most useful for determining how long something took to run
float Function GetCurrentRealTime() native global

; Returns whether the game is currently in menu mode or not
bool Function IsInMenuMode() native global

; Generates a random integer between aiMin and aiMax (inclusive)
int Function RandomInt(int aiMin = 0, int aiMax = 100) native global

; Generates a random floating point number between afMin and afMax (inclusive)
float Function RandomFloat(float afMin = 0.0, float afMax = 1.0) native global

; Set the given INI by type
function SetINIFloat(string ini, float value) native global
function SetINIInt(string ini, int value) native global
function SetINIBool(string ini, bool value) native global 
function SetINIString(string ini, string value) native global

; Waits for the specified amount of time (latent). Timer will not run during menu mode
Function Wait(float afSeconds) native global

; Waits for the specified amount of game time (latent)
Function WaitGameTime(float afHours) native global

; Waits for the specified amount of time (latent) - Timer WILL run during menu mode
Function WaitMenuMode(float afSeconds) native global

; Frame rate capture functions only available in beta version

; Gets you a string describing the frame rate for a certain number of frames
; (String will be no longer than 1K characters long, separated by commas)
string Function CaptureFrameRate(int numFrames) native global

; Starts or ends a frame rate capture -- then you can get the min or max since 
; frame capture started at any time
Function StartFrameRateCapture() native global
Function EndFrameRateCapture() native global
float Function GetAverageFrameRate() native global
float Function GetMinFrameRate() native global
float Function GetMaxFrameRate() native global

; Memory tracking functions - only available if memory tracking is turned on
int Function GetCurrentMemory() native global ; Must be called first, it sets up the memory stats used by the other functions
int Function GetBudgetCount() native global
int Function GetCurrentBudget(int aiBudgetNumber) native global
bool Function OverBudget(int aiBudgetNumber) native global
string Function GetBudgetName(int aiBudgetNumber) native global