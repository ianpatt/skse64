#pragma once

#include "skse64_loader_common/IdentifyEXE.h"

bool InjectDLL(PROCESS_INFORMATION * info, const char * dllPath, ProcHookInfo * hookInfo);
bool InjectDLLThread(PROCESS_INFORMATION * info, const char * dllPath, bool sync, bool noTimeout);
