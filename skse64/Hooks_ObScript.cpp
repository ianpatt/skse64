#include "Hooks_ObScript.h"
#include "ObScript.h"
#include "GameAPI.h"
//#include "GameReferences.h"
#include "skse64_common/SafeWrite.h"
#include "skse64_common/skse_version.h"
#include "GameData.h"
#include "GameObjects.h"
#include "GameReferences.h"
#include "Hooks_Gameplay.h"
#include "GameInput.h"

// currently ForceRSXCrash, could also use ToggleESRAM and several others
static ObScriptCommand * s_hijackedCommand = nullptr;

void Hooks_ObScript_Init()
{
	// instead of hooking the entire classic scripting system, we're just hijacking some unused commands

	for(ObScriptCommand * iter = g_firstConsoleCommand; iter->opcode < kObScript_NumConsoleCommands + kObScript_ConsoleOpBase; ++iter)
	{
		if(!strcmp(iter->longName, "ForceRSXCrash"))
		{
			s_hijackedCommand = iter;
			break;
		}
	}
}

bool GetSKSEVersion_Execute(const ObScriptParam * paramInfo, ScriptData * scriptData, TESObjectREFR * thisObj, TESObjectREFR* containingObj, Script* scriptObj, ScriptLocals* locals, double& result, UInt32& opcodeOffsetPtr)
{
	_MESSAGE("GetSKSEVersion_Execute");

	Console_Print("SKSE64 version: %d.%d.%d, release idx %d, runtime %08X",
		SKSE_VERSION_INTEGER, SKSE_VERSION_INTEGER_MINOR, SKSE_VERSION_INTEGER_BETA,
		SKSE_VERSION_RELEASEIDX, RUNTIME_VERSION);

	return true;
}

void Hooks_ObScript_Commit()
{
	ObScriptCommand cmd = *s_hijackedCommand;

	cmd.longName = "GetSKSEVersion";
	cmd.shortName = "skse";
	cmd.helpText = "";
	cmd.needsParent = 0;
	cmd.numParams = 0;
	cmd.execute = GetSKSEVersion_Execute;
	cmd.flags = 0;

	SafeWriteBuf((uintptr_t)s_hijackedCommand, &cmd, sizeof(cmd));
}
