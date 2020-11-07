#include "PapyrusArt.h"

#include "GameObjects.h"
#include "GameData.h"

namespace papyrusArt
{
	BSFixedString GetModelPath(BGSArtObject* thisArt)
	{
		return (thisArt) ? thisArt->texSwap.GetModelName() : NULL;
	}

	void SetModelPath(BGSArtObject* thisArt, BSFixedString nuPath)
	{
		if(thisArt) {
			thisArt->texSwap.SetModelName(nuPath.data);
		}
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusArt::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterForm(BGSArtObject::kTypeID, "Art");

	registry->RegisterFunction(
		new NativeFunction0 <BGSArtObject, BSFixedString>("GetModelPath", "Art", papyrusArt::GetModelPath, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSArtObject, void, BSFixedString>("SetModelPath", "Art", papyrusArt::SetModelPath, registry));
}