
; returns the quest with the specified editor id
Quest Function GetQuest(string editorId) global native

; returns the editor ID of the quest
string Function GetID() native

; returns the priority of the quest
int Function GetPriority() native

; returns the number of aliases associated with the quest
int Function GetNumAliases() native

; returns the specified alias associated with the queest
Alias Function GetNthAlias(int index) native

; returns the alias associated with the quest by name
Alias Function GetAliasByName(string name) native

; returns the alias by AlisID
Alias Function GetAliasById(int aliasId) native

; Returns all the aliases of this quest
Alias[] Function GetAliases() native