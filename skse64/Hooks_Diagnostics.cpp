#include "Hooks_Diagnostics.h"
#include "skse64_common/SafeWrite.h"
#include "skse64_common/Utilities.h"
#include <vector>
#include <string>

// not really needed, bethesda does this now

void Hooks_Diagnostics_Commit(void)
{
	UInt32	enableDiagnostics = 0;
	if (GetConfigOption_UInt32("General", "EnableDiagnostics", &enableDiagnostics))
	{
		if (enableDiagnostics)
		{
			// 
		}
	}
}
