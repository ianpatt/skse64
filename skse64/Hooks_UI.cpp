#include "Hooks_UI.h"
#include "Hooks_Threads.h"
#include "skse64_common/SafeWrite.h"
#include "skse64_common/Utilities.h"
#include "skse64_common/BranchTrampoline.h"
#include "GameMenus.h"
#include "common/ICriticalSection.h"
#include <queue>

#include "PapyrusUI.h"

namespace
{
	struct QueueEntry
	{
		// Allow implicit construction
		QueueEntry(UIDelegate* cmd) : bIsV1(false)   { data.cmd    = cmd; }
		QueueEntry(UIDelegate_v1* cmd) : bIsV1(true) { data.cmd_v1 = cmd; }

		bool	bIsV1;

		union {
			UIDelegate*		cmd;
			UIDelegate_v1*	cmd_v1;
		} data;
	};
}

ICriticalSection			g_uiQueueLock;
std::queue<QueueEntry>		g_uiQueue;

void UIManager::QueueCommand(UIDelegate* cmd)
{
	g_uiQueueLock.Enter();
	g_uiQueue.push(cmd);
	g_uiQueueLock.Leave();
}

void UIManager::QueueCommand(UIDelegate_v1* cmd)
{
	g_uiQueueLock.Enter();
	g_uiQueue.push(cmd);
	g_uiQueueLock.Leave();
}

void UIManager::ProcessCommands(void)
{
	CALL_MEMBER_FN(this, ProcessEventQueue_HookTarget)();
	
	g_uiQueueLock.Enter();
	while (!g_uiQueue.empty())
	{
		QueueEntry e = g_uiQueue.front();
		g_uiQueue.pop();

		// Old interface
		if (e.bIsV1)
		{
			UIDelegate_v1* cmd = e.data.cmd_v1;
			cmd->Run();
			cmd->Dispose();
		}
		// New interface
		else
		{
			UIDelegate* cmd = e.data.cmd;
			cmd->Run();
			delete cmd;
		}
	}
	g_uiQueueLock.Leave();
}

void TaskInterface::AddUITask(UIDelegate_v1* task)
{
	UIManager * uiManager = UIManager::GetSingleton();
	if(uiManager) {
		uiManager->QueueCommand(task);
	}
}

RelocAddr <uintptr_t> ProcessEventQueue_HookTarget_Enter(0x00EBE630 + 0xA14);

void Hooks_UI_Commit(void)
{
	g_branchTrampoline.Write5Call(ProcessEventQueue_HookTarget_Enter, GetFnAddr(&UIManager::ProcessCommands));
}
