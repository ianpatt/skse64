; Returns the number of refs in the cell
int Function GetNumRefs(int formTypeFilter = 0) native

; returns the ref at the specified index
ObjectReference Function GetNthRef(int n, int formTypeFilter = 0) native

; Returns the water level of the cell (-2147483648 if no water)
float Function GetWaterLevel() native

; Returns water level of the cell, if default returns water level from worldspace
float Function GetActualWaterLevel() native