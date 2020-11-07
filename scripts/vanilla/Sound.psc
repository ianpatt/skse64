Scriptname Sound extends Form Hidden
import ObjectReference

; Play this sound base object from the specified source
int Function Play(ObjectReference akSource) native

; Play this sound from the specified source, and wait for it to finish
bool Function PlayAndWait(ObjectReference akSource) native

; Stops a given playback instance of a sound
Function StopInstance(int aiPlaybackInstance) native global

; Set the volume of a given playback instance of a sound. Clamped between 0 and 1.
Function SetInstanceVolume(int aiPlaybackInstance, float afVolume) native global

