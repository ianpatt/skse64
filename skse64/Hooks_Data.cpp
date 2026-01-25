#include "Hooks_Data.h"
#include "GameData.h"
#include "PluginManager.h"
#include "skse64_common/BranchTrampoline.h"

UInt32 DataHandler::LoadScripts_Hook()
{
	UInt32 ret = CALL_MEMBER_FN(this, LoadScripts)();
	PluginManager::Dispatch_Message(0, SKSEMessagingInterface::kMessage_DataLoaded, (void*)NULL, 0, NULL);
	return ret;
}

RelocAddr<uintptr_t>  LoadScripts_Enter(0x005B0120 + 0x3E8);

void Hooks_Data_Commit(void)
{
	g_branchTrampoline.Write5Call(LoadScripts_Enter.GetUIntPtr(), GetFnAddr(&DataHandler::LoadScripts_Hook));
}
