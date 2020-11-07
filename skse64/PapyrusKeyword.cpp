#include "PapyrusKeyword.h"

#include "GameForms.h"
#include "GameObjects.h"
#include "GameData.h"

#include "common/ICriticalSection.h"

//extern UInt32 g_invalidateKeywordCache;

namespace papyrusKeyword
{
	typedef std::map <BSFixedString, BGSKeyword *> KeywordCache;

	static ICriticalSection	s_keywordCacheLock;
	static KeywordCache s_keywordCache;

	BGSKeyword* GetKeyword(StaticFunctionTag*, BSFixedString keyword)
	{
		s_keywordCacheLock.Enter();
		
		//if(g_invalidateKeywordCache == 1) {
			//s_keywordCache.clear();
			//g_invalidateKeywordCache = 0;
		//}

		if (s_keywordCache.empty()) {
			DataHandler* pDataHandler = DataHandler::GetSingleton();
			tArray<BGSKeyword*>& keywords = pDataHandler->keywords;
			for (UInt32 n = 0; n < 	keywords.count; n++) {
				BGSKeyword* pKeyword = NULL;
				keywords.GetNthItem(n, pKeyword);
				if (pKeyword) {
					s_keywordCache.insert(KeywordCache::value_type(BSFixedString(pKeyword->keyword.Get()), pKeyword));
				}
			}
		}

		s_keywordCacheLock.Leave();
		
		KeywordCache::iterator it = s_keywordCache.find(keyword);
		BGSKeyword* pKeyword = (it != s_keywordCache.end()) ? it->second : NULL;
		return pKeyword;
	}


	BSFixedString GetString(BGSKeyword* thisKeyword)
	{
		return (thisKeyword) ? thisKeyword->keyword.Get() : NULL;
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusKeyword::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0 <BGSKeyword, BSFixedString> ("GetString", "Keyword", papyrusKeyword::GetString, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, BGSKeyword*, BSFixedString> ("GetKeyword", "Keyword", papyrusKeyword::GetKeyword, registry));

}
