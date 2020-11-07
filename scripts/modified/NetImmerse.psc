Scriptname NetImmerse Hidden

; Note that only local transforms can be set as the world transform
; is computed based on the entire hierarchy rather than a single node

; Return whether the object has the particular node
bool Function HasNode(ObjectReference ref, string node, bool firstPerson) native global

; NiNode Manipulation
float Function GetNodeWorldPositionX(ObjectReference ref, string node, bool firstPerson) native global
float Function GetNodeWorldPositionY(ObjectReference ref, string node, bool firstPerson) native global
float Function GetNodeWorldPositionZ(ObjectReference ref, string node, bool firstPerson) native global

; Returns nodeB - nodeA
float Function GetRelativeNodePositionX(ObjectReference ref, string nodeA, string nodeB, bool firstPerson) native global
float Function GetRelativeNodePositionY(ObjectReference ref, string nodeA, string nodeB, bool firstPerson) native global
float Function GetRelativeNodePositionZ(ObjectReference ref, string nodeA, string nodeB, bool firstPerson) native global

float Function GetNodeLocalPositionX(ObjectReference ref, string node, bool firstPerson) native global
float Function GetNodeLocalPositionY(ObjectReference ref, string node, bool firstPerson) native global
float Function GetNodeLocalPositionZ(ObjectReference ref, string node, bool firstPerson) native global

Function SetNodeLocalPositionX(ObjectReference ref, string node, float x, bool firstPerson) native global
Function SetNodeLocalPositionY(ObjectReference ref, string node, float y, bool firstPerson) native global
Function SetNodeLocalPositionZ(ObjectReference ref, string node, float z, bool firstPerson) native global

; Sets the scale of a particular Nif node
float Function GetNodeScale(ObjectReference ref, string node, bool firstPerson) native global
Function SetNodeScale(ObjectReference ref, string node, float scale, bool firstPerson) native global

; Sets a NiTriShape's textures by name of the Nif node
Function SetNodeTextureSet(ObjectReference ref, string node, TextureSet tSet, bool firstPerson) native global


; Array based functions, return true when successful, false when unsuccessful (node did not exist, or array wrong size)

; returns the node's world position into the specify array, must be size of 3
bool Function GetNodeWorldPosition(ObjectReference ref, string node, float[] in, bool firstPerson) native global

; returns the node's relative world position of nodeB minus nodeA into the specify array, must be size of 3
bool Function GetRelativeNodePosition(ObjectReference ref, string nodeA, string nodeB, float[] in, bool firstPerson) native global

; returns the node's local position into the specify array, must be size of 3
bool Function GetNodeLocalPosition(ObjectReference ref, string node, float[] in, bool firstPerson) native global

; sets the node's local position of the specified array, must be size of 3
bool Function SetNodeLocalPosition(ObjectReference ref, string node, float[] in, bool firstPerson) native global

; Euler Rotation in DEGREES (heading, attitude, bank)
; returns the euler rotation of the node into the specified array, must be size of 3
bool Function GetNodeWorldRotationEuler(ObjectReference ref, string node, float[] in, bool firstPerson) native global

; returns the euler rotation of the node into the specified array, must be size of 3
bool Function GetNodeLocalRotationEuler(ObjectReference ref, string node, float[] in, bool firstPerson) native global

; sets the euler rotation for the node of the specified array, must be size of 3
bool Function SetNodeLocalRotationEuler(ObjectReference ref, string node, float[] in, bool firstPerson) native global

; Matrix Rotation in RADIANS
; returns the matrix rotation of the node into the specified array, must be size of 9
bool Function GetNodeWorldRotationMatrix(ObjectReference ref, string node, float[] in, bool firstPerson) native global

; returns the matrix rotation of the node into the specified array, must be size of 9
bool Function GetNodeLocalRotationMatrix(ObjectReference ref, string node, float[] in, bool firstPerson) native global

; sets the matrix rotation for the node of the specified array, must be size of 9
bool Function SetNodeLocalRotationMatrix(ObjectReference ref, string node, float[] in, bool firstPerson) native global


; DEPRECATED FUNCTIONS
Function SetNodePositionX(ObjectReference ref, string node, float x, bool firstPerson) global
	NetImmerse.SetNodeLocalPositionX(ref, node, x, firstPerson)
EndFunction
Function SetNodePositionY(ObjectReference ref, string node, float y, bool firstPerson) global
	NetImmerse.SetNodeLocalPositionY(ref, node, y, firstPerson)
EndFunction
Function SetNodePositionZ(ObjectReference ref, string node, float z, bool firstPerson) global
	NetImmerse.SetNodeLocalPositionZ(ref, node, z, firstPerson)
EndFunction

float Function GetNodePositionX(ObjectReference ref, string node, bool firstPerson) global
	return NetImmerse.GetNodeWorldPositionX(ref, node, firstPerson)
EndFunction
float Function GetNodePositionY(ObjectReference ref, string node, bool firstPerson) global
	return NetImmerse.GetNodeWorldPositionY(ref, node, firstPerson)
EndFunction
float Function GetNodePositionZ(ObjectReference ref, string node, bool firstPerson) global
	return NetImmerse.GetNodeWorldPositionZ(ref, node, firstPerson)
EndFunction