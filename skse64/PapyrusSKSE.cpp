#include "PapyrusSKSE.h"
#include "PluginManager.h"

#include "skse64_common/skse_version.h"


namespace papyrusSKSE {

	UInt32 GetVersion(StaticFunctionTag* base)
	{
		return SKSE_VERSION_INTEGER;
	}
	UInt32 GetVersionMinor(StaticFunctionTag* base)
	{
		return SKSE_VERSION_INTEGER_MINOR;
	}

	UInt32 GetVersionBeta(StaticFunctionTag* base)
	{
		return SKSE_VERSION_INTEGER_BETA;
	}

	UInt32 GetVersionRelease(StaticFunctionTag* base)
	{
		return SKSE_VERSION_RELEASEIDX;
	}

	UInt32 GetPluginVersion(StaticFunctionTag* base, BSFixedString name)
	{
		PluginInfo * info = g_pluginManager.GetInfoByName(name.data);
		if (info) {
			return info->version;
		}

		return -1;
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusSKSE::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0<StaticFunctionTag, UInt32>("GetVersion", "SKSE", papyrusSKSE::GetVersion, registry));

	registry->RegisterFunction(
		new NativeFunction0<StaticFunctionTag, UInt32>("GetVersionMinor", "SKSE", papyrusSKSE::GetVersionMinor, registry));

	registry->RegisterFunction(
		new NativeFunction0<StaticFunctionTag, UInt32>("GetVersionBeta", "SKSE", papyrusSKSE::GetVersionBeta, registry));

	registry->RegisterFunction(
		new NativeFunction0<StaticFunctionTag, UInt32>("GetVersionRelease", "SKSE", papyrusSKSE::GetVersionRelease, registry));

	registry->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, UInt32, BSFixedString>("GetPluginVersion", "SKSE", papyrusSKSE::GetPluginVersion, registry));

	registry->SetFunctionFlags("SKSE", "GetVersion", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("SKSE", "GetVersionMinor", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("SKSE", "GetVersionBeta", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("SKSE", "GetVersionRelease", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("SKSE", "GetPluginVersion", VMClassRegistry::kFunctionFlag_NoWait);
}