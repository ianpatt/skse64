Scriptname LeveledActor extends Form Hidden

; Adds the given count of the given form to the under the given level in this leveled list
Function AddForm(Form apForm, int aiLevel) native

; Removes all script added forms from this leveled list
Function Revert() native