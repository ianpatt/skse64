#include "ObScript.h"
#include "skse64_common\Relocation.h"

// 698845F80EE915EE803E515A37C1269ED6A2FE43+47
RelocPtr <ObScriptCommand>	g_firstObScriptCommand(0x01DB8910);
// 698845F80EE915EE803E515A37C1269ED6A2FE43+1E
RelocPtr <ObScriptCommand>	g_firstConsoleCommand(0x01DC6F60);

RelocAddr<_ObScript_ExtractArgs> ObjScript_ExtractArgs(0x002E8400);