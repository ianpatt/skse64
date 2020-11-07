#pragma once

class TaskDelegate;
class UIDelegate_v1;

void Hooks_Threads_Init(void);
void Hooks_Threads_Commit(void);

namespace TaskInterface
{
	void AddTask(TaskDelegate * task);
	void AddUITask(UIDelegate_v1 * task);
}
