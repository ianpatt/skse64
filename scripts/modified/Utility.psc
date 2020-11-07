
float Function GetINIFloat(string ini) global native
int Function GetINIInt(string ini) global native
bool Function GetINIBool(string ini) global native
string Function GetINIString(string ini) global native


; Size is treated as unsigned, negative numbers will result 
; extremely large positive numbers, USE WITH CARE
float[] Function CreateFloatArray(int size, float fill = 0.0) global native
int[] Function CreateIntArray(int size, int fill = 0) global native
bool[] Function CreateBoolArray(int size, bool fill = false) global native
string[] Function CreateStringArray(int size, string fill = "") global native
Form[] Function CreateFormArray(int size, Form fill = None) global native
Alias[] Function CreateAliasArray(int size, Alias fill = None) global native

float[] Function ResizeFloatArray(float[] source, int size, float fill = 0.0) global native
int[] Function ResizeIntArray(int[] source, int size, int fill = 0) global native
bool[] Function ResizeBoolArray(bool[] source, int size, bool fill = false) global native
string[] Function ResizeStringArray(string[] source, int size, string fill = "") global native
Form[] Function ResizeFormArray(Form[] source, int size, Form fill = None) global native
Alias[] Function ResizeAliasArray(Alias[] source, int size, Alias fill = None) global native