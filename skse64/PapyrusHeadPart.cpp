#include "PapyrusHeadPart.h"

#include "GameObjects.h"
#include "GameData.h"

#include "common/ICriticalSection.h"
 
namespace papyrusHeadPart
{
	ICriticalSection	s_headPartCacheLock;
	typedef std::map<BSFixedString, BGSHeadPart*> HeadPartCache;
	static HeadPartCache s_headPartCache;

	BGSHeadPart* GetHeadPart(StaticFunctionTag*, BSFixedString editorID)
	{
		s_headPartCacheLock.Enter();

		if (s_headPartCache.empty()) {
			DataHandler* pDataHandler = DataHandler::GetSingleton();
			tArray<BGSHeadPart*>& headParts = pDataHandler->headParts;
			for (UInt32 n = 0; n < headParts.count; n++) {
				BGSHeadPart* pHeadPart = NULL;
				headParts.GetNthItem(n, pHeadPart);
				if (pHeadPart) {
					s_headPartCache.insert(HeadPartCache::value_type(BSFixedString(pHeadPart->partName), pHeadPart));
				}
			}
		}

		s_headPartCacheLock.Leave();

		HeadPartCache::iterator it = s_headPartCache.find(editorID);
		BGSHeadPart* pHeadPart = (it != s_headPartCache.end()) ? it->second : NULL;
		return pHeadPart;
	}
	
	UInt32 GetType(BGSHeadPart* thisPart)
	{
		if(!thisPart)
			return 0;
		return thisPart->type;
	}

	UInt32 GetNumExtraParts(BGSHeadPart* thisPart)
	{
		return (thisPart) ? thisPart->extraParts.count : 0;
	}
	
	BGSHeadPart* GetNthExtraPart(BGSHeadPart* thisPart, UInt32 n)
	{
		BGSHeadPart* headPart;
		if(!thisPart || !thisPart->extraParts.GetNthItem(n, headPart))
			return NULL;

		return headPart;
	}

	bool IsExtraPart(BGSHeadPart* thisPart)
	{
		return (thisPart && thisPart->IsExtraPart()) ? true : false;
	}

	bool HasExtraPart(BGSHeadPart* thisPart, BGSHeadPart* extraPart)
	{
		return (thisPart && thisPart->extraParts.GetItemIndex(extraPart) != -1) ? true : false;
	}
	
	UInt32 GetIndexOfExtraPart(BGSHeadPart* thisPart, BGSHeadPart* extraPart)
	{
		return (thisPart) ? thisPart->extraParts.GetItemIndex(extraPart) : 0;
	}
	
	BGSListForm* GetValidRaces(BGSHeadPart* thisPart)
	{
		return (thisPart) ? thisPart->validRaces : NULL;
	}
	
	void SetValidRaces(BGSHeadPart* thisPart, BGSListForm* raceList)
	{
		if(thisPart && raceList) {
			thisPart->validRaces = raceList;
		}
	}

	BSFixedString GetPartName(BGSHeadPart* thisPart)
	{
		return thisPart ? thisPart->partName : "";
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusHeadPart::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterForm(BGSHeadPart::kTypeID, "HeadPart");

	registry->RegisterFunction(
		new NativeFunction1<StaticFunctionTag, BGSHeadPart*, BSFixedString>("GetHeadPart", "HeadPart", papyrusHeadPart::GetHeadPart, registry));

	registry->RegisterFunction(
		new NativeFunction0<BGSHeadPart, UInt32>("GetType", "HeadPart", papyrusHeadPart::GetType, registry));

	registry->RegisterFunction(
		new NativeFunction0<BGSHeadPart, UInt32>("GetNumExtraParts", "HeadPart", papyrusHeadPart::GetNumExtraParts, registry));

	registry->RegisterFunction(
		new NativeFunction1<BGSHeadPart, BGSHeadPart*, UInt32>("GetNthExtraPart", "HeadPart", papyrusHeadPart::GetNthExtraPart, registry));

	registry->RegisterFunction(
		new NativeFunction1<BGSHeadPart, bool, BGSHeadPart*>("HasExtraPart", "HeadPart", papyrusHeadPart::HasExtraPart, registry));

	registry->RegisterFunction(
		new NativeFunction0<BGSHeadPart, bool>("IsExtraPart", "HeadPart", papyrusHeadPart::IsExtraPart, registry));

	registry->RegisterFunction(
		new NativeFunction1<BGSHeadPart, UInt32, BGSHeadPart*>("GetIndexOfExtraPart", "HeadPart", papyrusHeadPart::GetIndexOfExtraPart, registry));

	registry->RegisterFunction(
		new NativeFunction0<BGSHeadPart, BGSListForm*>("GetValidRaces", "HeadPart", papyrusHeadPart::GetValidRaces, registry));

	registry->RegisterFunction(
		new NativeFunction1<BGSHeadPart, void, BGSListForm*>("SetValidRaces", "HeadPart", papyrusHeadPart::SetValidRaces, registry));

	registry->RegisterFunction(
		new NativeFunction0<BGSHeadPart, BSFixedString>("GetPartName", "HeadPart", papyrusHeadPart::GetPartName, registry));
}