#pragma once

class VMClassRegistry;
class BGSLocation;

namespace papyrusLocation
{
	BGSLocation * GetParent(BGSLocation* thisLocation);
	void RegisterFuncs(VMClassRegistry* registry);
};