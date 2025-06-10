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

	// create temporary vector array to copy queue contents into
	std::vector<TaskDelegate*> tasks_vector;

	// copy queue contents until queue is empty (this is to prevent an infinite loop in some cases)
	while (!IsTaskQueueEmpty())
	{
		s_taskQueueLock.Enter();
		TaskDelegate* cmd = s_tasks.front();
		tasks_vector.push_back(cmd);
		s_tasks.pop();
		s_taskQueueLock.Leave();
	}

	// run copied delegates
	for (TaskDelegate* cmd : tasks_vector) {
		cmd->Run();
		cmd->Dispose();
	}
}

void TaskInterface::AddTask(TaskDelegate* cmd)
{
	s_taskQueueLock.Enter();
	s_tasks.push(cmd);
	s_taskQueueLock.Leave();
}

void Hooks_Threads_Init(void)
{

}

// 691ACD40B8430FBDA081477DEB2A9948ACC235F1+11F
RelocAddr <uintptr_t> ProcessTasks_HookTarget_Enter(0x00645EA0 + 0xB9C);
RelocAddr <uintptr_t> ProcessTasks_HookTarget_Enter2(0x006479E0 + 0x1C);

void Hooks_Threads_Commit(void)
{
	g_branchTrampoline.Write5Call(ProcessTasks_HookTarget_Enter, GetFnAddr(&BSTaskPool::ProcessTasks));
	g_branchTrampoline.Write5Call(ProcessTasks_HookTarget_Enter2, GetFnAddr(&BSTaskPool::ProcessTasks));
}
