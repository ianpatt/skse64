Scriptname StringUtil Hidden

; Note about the internal Skyrim implementation of the string classes used for scripting:
; the strings are case-insensitive.  Each BSFixedString is managed in a cache and reused
; everywhere it is needed.  This means that strings like "O" and "o" are technically equivalent;
; Which string is used depends greatly on which version is found first.  We are investigating
; how to manage this, but for the time being be aware that the distinction between uppercase
; and lowercase may not exist.  It also means that functions below returning an integer
; for the character may not correspond exactly.  Also GetNthChar("Hello Skyrim!", 4) will
; return a string with either "O" or "o" depnding on which might be registered first.  All
; my tests so far have it return the uppercase, eventhough in the string it is lowercase.
; We may solve this problem by switching back to returning an integer rather than a string
; for GetNthChar, but this will still have problems.

; return the length of the string
int Function GetLength(string s) global native

; returns a single character string with the character at index
string Function GetNthChar(string s, int index) global native

; Functions to work on Chars
; returns information about a specific character
; assumes a single character string.  If a multicharacter string is passed
; the information about the first character is returned
bool Function IsLetter(string c) global native
bool Function IsDigit(string c) global native
bool Function IsPunctuation(string c) global native
bool Function IsPrintable(string c) global native

; returns the index of the first character of toFind inside string s
; returns -1 if toFind is not part of the string or if startIndex is invalid
int Function Find(string s, string toFind, int startIndex = 0) global native

; returns a substring of the specified string starting at startIndex and going for len characters
; or until the end of the string.  Default len of 0 means for the entire string
string Function Substring(string s, int startIndex, int len = 0) global native

; returns the numeric value of the first character as an int
int Function AsOrd(string c) global native

; returns a single character string interpreting c as a character
string Function AsChar(int c) global native

; returns array of strings separated by the specified delimiter
string[] Function Split(string s, string delim) global native