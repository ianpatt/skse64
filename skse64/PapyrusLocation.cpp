#include "PapyrusLocation.h"

#include "GameForms.h"

namespace papyrusLocation
{
	BGSLocation * GetParent(BGSLocation* thisLocation)
	{
		return thisLocation ? thisLocation->parentLoc : NULL;
	}	
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusLocation::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0<BGSLocation, BGSLocation*>("GetParent", "Location", papyrusLocation::GetParent, registry));
}