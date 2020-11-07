#include "PapyrusSound.h"

#include "GameFormComponents.h"
#include "GameForms.h"
#include "GameObjects.h"

namespace papyrusSound
{
	BGSSoundDescriptorForm * GetDescriptor(TESSound* thisSound)
	{
		return (thisSound) ? thisSound->descriptor : NULL;
	}	
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusSound::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0<TESSound, BGSSoundDescriptorForm*>("GetDescriptor", "Sound", papyrusSound::GetDescriptor, registry));
}