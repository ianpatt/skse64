Scriptname FormList extends Form

; Adds the given form to this form list
Function AddForm(Form apForm) native

; Finds the specified form in the form list and returns its index.
; If not found, returns a negative number
int Function Find(Form apForm) native

; Returns the number of forms in the list
int Function GetSize() native

; Returns the form at index 'aiIndex' in the list
Form Function GetAt(int aiIndex) native

; Queries the form list to see if it contains the passed in form
bool Function HasForm(Form akForm) native

; Removes the given added form from this form list
Function RemoveAddedForm(Form apForm) native

; Removes all script added forms from this form list
Function Revert() native
