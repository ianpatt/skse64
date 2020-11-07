#include "skse64/PapyrusDefaultObjectManager.h"
#include "skse64/GameObjects.h"
#include "skse64/GameData.h"

namespace papyrusDefaultObjectManager
{
	void SetForm(BGSDefaultObjectManager * objectManager, BSFixedString keySearch, TESForm * form)
	{
		if(objectManager) {
			UInt32 size = strlen(keySearch.data);
			if(size == 4) {
				UInt32 key = toupper(keySearch.data[3]) << 24 | toupper(keySearch.data[2]) << 16 | toupper(keySearch.data[1]) << 8 | toupper(keySearch.data[0]);

				DefaultObjectList * defaultList = DefaultObjectList::GetSingleton();
				if(defaultList)
				{
					for(UInt32 i = 0; i < DefaultObjectList::kNumDefaultObjects; i++)
					{
						if(key == defaultList->objects[i].key) {
							objectManager->objects[i] = form;
						}
					}
				}
			}
		}
	}

	TESForm * GetForm(BGSDefaultObjectManager * objectManager, BSFixedString keySearch)
	{
		if(!objectManager)
			return NULL;

		UInt32 size = strlen(keySearch.data);
		if(size != 4) {
			return NULL;
		}

		UInt32 key = toupper(keySearch.data[3]) << 24 | toupper(keySearch.data[2]) << 16 | toupper(keySearch.data[1]) << 8 | toupper(keySearch.data[0]);

		DefaultObjectList * defaultList = DefaultObjectList::GetSingleton();
		if(defaultList)
		{
			for(UInt32 i = 0; i < DefaultObjectList::kNumDefaultObjects; i++)
			{
				if(key == defaultList->objects[i].key) {
					return objectManager->objects[i];
				}
			}
		}

		return NULL;
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusDefaultObjectManager::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterForm(BGSDefaultObjectManager::kTypeID, "DefaultObjectManager");

	registry->RegisterFunction(
		new NativeFunction1<BGSDefaultObjectManager, TESForm*, BSFixedString>("GetForm", "DefaultObjectManager", papyrusDefaultObjectManager::GetForm, registry));

	registry->RegisterFunction(
		new NativeFunction2<BGSDefaultObjectManager, void, BSFixedString, TESForm*>("SetForm", "DefaultObjectManager", papyrusDefaultObjectManager::SetForm, registry));
}
