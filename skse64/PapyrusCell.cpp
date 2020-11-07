#include "PapyrusCell.h"

#include "GameForms.h"
#include "GameObjects.h"
#include "GameReferences.h"

namespace papyrusCell
{
	UInt32 GetNumRefs(TESObjectCELL* thisCell, UInt32 formType)
	{
		// SE: new implementation. objectList is not longer reliable and it is NULL in most cells.

		if (!thisCell)
			return 0;

		if (formType == 0)
			return thisCell->refData.maxSize - thisCell->refData.freeEntries;

		UInt32	numRefs = 0;
	
		for (UInt32 i = 0; i < thisCell->refData.maxSize; i++)
		{
			if (thisCell->refData.refArray[i].unk08 && thisCell->refData.refArray[i].ref)
			{
				if (thisCell->refData.refArray[i].ref->baseForm->formType == formType)
				{
					numRefs++;
				}
			}
		}	
		
		return numRefs;
	}

	TESObjectREFR * GetNthRef(TESObjectCELL * thisCell, UInt32 index, UInt32 formType)
	{
		// SE: new implementation. objectList is not longer reliable and it is NULL in most cells.
		
		if (!thisCell)
			return NULL;

		for (UInt32 i = 0, n = 0; i < thisCell->refData.maxSize; i++)
		{
			if (thisCell->refData.refArray[i].unk08 && thisCell->refData.refArray[i].ref)
			{
				if (formType == 0 || thisCell->refData.refArray[i].ref->baseForm->formType == formType)
				{
					if (n++ == index)
					{
						return thisCell->refData.refArray[i].ref;
					}
				}
			}
		}
		return NULL;
	}

	float GetWaterLevel(TESObjectCELL * thisCell)
	{
		if (!thisCell)
			return LONG_MIN;

		return thisCell->waterLevel;
	}

	float GetActualWaterLevel(TESObjectCELL * thisCell)
	{
		if (!thisCell)
			return LONG_MIN;

		if (thisCell->waterLevel == FLT_MAX && thisCell->worldSpace)
			return thisCell->worldSpace->landData.defaultWaterHeight;

		return thisCell->waterLevel;
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusCell::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction1 <TESObjectCELL, UInt32, UInt32>("GetNumRefs", "Cell", papyrusCell::GetNumRefs, registry));

	registry->RegisterFunction(
		new NativeFunction2 <TESObjectCELL, TESObjectREFR*, UInt32, UInt32>("GetNthRef", "Cell", papyrusCell::GetNthRef, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectCELL, float>("GetWaterLevel", "Cell", papyrusCell::GetWaterLevel, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESObjectCELL, float>("GetActualWaterLevel", "Cell", papyrusCell::GetActualWaterLevel, registry));
}
