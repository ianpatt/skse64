Scriptname Location extends Form Hidden

; Returns the float value attached to the specified keyword attached to this location
float Function GetKeywordData(Keyword akKeyword) native

; Returns the number of alive references matching the specified reference type
int Function GetRefTypeAliveCount(LocationRefType akRefType) native

; Returns the number of dead references matching the specified reference type
int Function GetRefTypeDeadCount(LocationRefType akRefType) native

; Returns if these two locations have a common parent - filtered with the keyword, if provided
bool Function HasCommonParent(Location akOther, Keyword akFilter = None) native

; Returns if this location has the specified reference type
bool Function HasRefType(LocationRefType akRefType) native

; Returns whether this location is flagged as "cleared" or not
bool Function IsCleared() native

; Returns whether the other location is a child of this one
bool Function IsChild(Location akOther) native

; Is this location loaded in game?
bool Function IsLoaded() native

bool Function IsSameLocation(Location akOtherLocation, Keyword akKeyword = None)
{Returns true if the calling location is the same as the supplied location - if an optional keyword is supplied, it also returns true if the locations share a parent with that keyword, or if either location is a child of the other and the other has that keyword.}
;jduvall 
	bool bmatching = self == akOtherLocation
	if !bmatching && akKeyword
		bmatching = HasCommonParent(akOtherLocation, akKeyword)
		
		if !bmatching && akOtherLocation.HasKeyword(akKeyword)
			bmatching = akOtherLocation.IsChild(self) 
		elseif !bmatching && self.HasKeyword(akKeyword)
			bmatching = self.IsChild(akOtherLocation) 
		endif
		
	endif
  return bmatching
endFunction


; Sets the specified keyword's data on the location
Function SetKeywordData(Keyword akKeyword, float afData) native

; Sets this location as cleared or not
Function SetCleared(bool abCleared = true) native