#include "GlobalLocks.h"
#include "common/ICriticalSection.h"

// Cache-line aligned to prevent false sharing during save/load
alignas(64) ICriticalSection	g_loadGameLock;
