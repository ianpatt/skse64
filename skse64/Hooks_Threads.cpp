#include "Hooks_Threads.h"
#include "skse64_common/SafeWrite.h"
#include "skse64_common/Utilities.h"
#include "skse64_common/BranchTrampoline.h"
#include "GameThreads.h"

#include "common/ICriticalSection.h"
#include <queue>

ICriticalSection			s_taskQueueLock;
std::queue<TaskDelegate*>	s_tasks;

static bool IsTaskQueueEmpty()
{
	IScopedCriticalSection scoped(&s_taskQueueLock);
	return s_tasks.empty();
}

void BSTaskPool::ProcessTasks()
{
	CALL_MEMBER_FN(this, ProcessTaskQueue_HookTarget)();

	while (!IsTaskQueueEmpty())
	{
		s_taskQueueLock.Enter();
		TaskDelegate * cmd = s_tasks.front();
		s_tasks.pop();
		s_taskQueueLock.Leave();

		cmd->Run();
		cmd->Dispose();
	}
}

void TaskInterface::AddTask(TaskDelegate * cmd)
{
	s_taskQueueLock.Enter();
	s_tasks.push(cmd);
	s_taskQueueLock.Leave();
}

void Hooks_Threads_Init(void)
{
	
}

RelocAddr <uintptr_t> ProcessTasks_HookTarget_Enter(0x005B2FF0 + 0x6B8);
RelocAddr <uintptr_t> ProcessTasks_HookTarget_Enter2(0x005B44C0 + 0x1C);

void Hooks_Threads_Commit(void)
{
	g_branchTrampoline.Write5Call(ProcessTasks_HookTarget_Enter, GetFnAddr(&BSTaskPool::ProcessTasks));
	g_branchTrampoline.Write5Call(ProcessTasks_HookTarget_Enter2, GetFnAddr(&BSTaskPool::ProcessTasks));
}
