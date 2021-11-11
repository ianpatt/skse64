#include "ObScript.h"
#include "skse64_common\Relocation.h"

// B0061DDA35158B8956EF9CFBDD3D63C5B4C96A8D+33
RelocPtr <ObScriptCommand>	g_firstObScriptCommand(0x01E4C9B0);
// B0061DDA35158B8956EF9CFBDD3D63C5B4C96A8D+F
RelocPtr <ObScriptCommand>	g_firstConsoleCommand(0x01E5B000);

RelocAddr <_ObScript_ExtractArgs> ObScript_ExtractArgs(0x002FC890);
