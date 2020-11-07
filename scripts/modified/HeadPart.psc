Scriptname HeadPart extends Form Hidden

int Property Type_Misc = 0 AutoReadOnly
int Property Type_Face = 1 AutoReadOnly
int Property Type_Eyes = 2 AutoReadOnly
int Property Type_Hair = 3 AutoReadOnly
int Property Type_FacialHair = 4 AutoReadOnly
int Property Type_Scar = 5 AutoReadOnly
int Property Type_Brows = 6 AutoReadOnly

HeadPart Function GetHeadPart(string name) native global

; Returns the head part type
int Function GetType() native

int Function GetNumExtraParts() native
HeadPart Function GetNthExtraPart(int n) native

bool Function HasExtraPart(HeadPart p) native
int Function GetIndexOfExtraPart(HeadPart p) native

; Returns a formlist of the valid races for this head part
FormList Function GetValidRaces() native
Function SetValidRaces(FormList vRaces) native

; Returns whether the head part is an extra part
bool Function IsExtraPart() native

; Returns the EditorID of the HeadPart
string Function GetPartName() native