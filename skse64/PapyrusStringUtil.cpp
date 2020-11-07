#include "PapyrusStringUtil.h"
#include "PapyrusArgs.h"

#include <algorithm>
#include <locale>

namespace papyrusStringUtil
{

	UInt32 GetLength(StaticFunctionTag* thisInput, BSFixedString theString) 
	{
		return strlen(theString.data);	
	}

	struct SingleCharStar
	{
		char m_c[2];
		SingleCharStar(char c) { m_c[0] = c; m_c[1] = '\0';}
		SingleCharStar& operator=(char c) {
			m_c[0] = c;
			return *this;
		}
		const char* c_str() { return m_c; }
	};

	BSFixedString GetNthChar(StaticFunctionTag* thisInput, BSFixedString theString, UInt32 index)
	{
		if (index < 0 || index >= strlen(theString.data))
			return NULL;
		return SingleCharStar(theString.data[index]).c_str();
	}

	bool IsUppercase(StaticFunctionTag* thisInput, BSFixedString s) 
	{
		if (!s.data)
			return false;
		return (isupper(s.data[0])) ? true : false; 
	}

	bool IsLowercase(StaticFunctionTag* thisInput, BSFixedString s) 
	{
		if (!s.data)
			return false;
		return islower(s.data[0]) ? true : false; 
	}

	bool IsLetter(StaticFunctionTag* thisInput, BSFixedString s) 
	{
		if (!s.data)
			return false;
		return isalpha(s.data[0]) ? true : false; 
	}
	bool IsDigit(StaticFunctionTag* thisInput, BSFixedString s) 
	{
		if (!s.data)
			return false;
		return isdigit(s.data[0]) ? true : false; 
	}
	bool IsPunctuation(StaticFunctionTag* thisInput, BSFixedString s) 
	{
		if (!s.data)
			return false;
		return ispunct(s.data[0]) ? true : false; 
	}
	bool IsPrintable(StaticFunctionTag* thisInput, BSFixedString s) 
	{
		if (!s.data)
			return false;
		return isprint(s.data[0]) ? true : false; 
	}
	BSFixedString ToUpper(StaticFunctionTag* thisInput, BSFixedString s)
	{
		if (!s.data)
			return NULL;

		return SingleCharStar(toupper(s.data[0])).c_str();
	}

	BSFixedString ToLower(StaticFunctionTag* thisInput, BSFixedString s) 
	{
		if (!s.data)
			return NULL;
		return SingleCharStar(tolower(s.data[0])).c_str();
	}

	UInt32 AsOrd(StaticFunctionTag* thisInput, BSFixedString s)
	{
		return s.data[0];
	}

	BSFixedString AsChar(StaticFunctionTag* thisInput, UInt32 c)
	{
		if (c > 255)
			return NULL;
		return SingleCharStar(c).c_str();
	}

	

	SInt32 Find(StaticFunctionTag* thisInput, BSFixedString theString, BSFixedString toFind, UInt32 startIndex)
	{
		if (startIndex < 0)
			startIndex = 0;

		std::string s(theString.data);
		if (startIndex >= s.length())
			return -1;
		
		std::transform(s.begin(), s.end(), s.begin(), toupper);
		
		std::string f(toFind.data);
		std::transform(f.begin(), f.end(), f.begin(), toupper);

		std::string::size_type pos = s.find(f, startIndex);
		return (pos == std::string::npos) ? -1 : pos;
	}

	BSFixedString Substring(StaticFunctionTag* thisInput, BSFixedString theString, UInt32 startIndex, UInt32 len)
	{
		if (startIndex < 0)
			return NULL;

		std::string tmpStr(theString.data);
		if (startIndex >= tmpStr.length())
			return NULL;

		std::string sub = (len) ? tmpStr.substr(startIndex, len) : tmpStr.substr(startIndex);
		return sub.c_str();
	}

	VMResultArray<BSFixedString> Split(StaticFunctionTag* thisInput, BSFixedString theString, BSFixedString theDelimiter)
	{
		VMResultArray<BSFixedString> result;

		std::string str(theString.data);
		std::string delimiters(theDelimiter.data);

		std::string f(str); // Eliminate case sensitivity during find
		std::transform(f.begin(), f.end(), f.begin(), toupper);
		std::transform(delimiters.begin(), delimiters.end(), delimiters.begin(), toupper);

		std::string::size_type lastPos = f.find_first_not_of(delimiters, 0);
		std::string::size_type pos     = f.find_first_of(delimiters, lastPos);

		while (std::string::npos != pos || std::string::npos != lastPos)
		{
			std::string token = str.substr(lastPos, pos - lastPos); // Pull from original string
			result.push_back(BSFixedString(token.c_str()));
			lastPos = f.find_first_not_of(delimiters, pos);
			pos = f.find_first_of(delimiters, lastPos);
		}

		return result;
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusStringUtil::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, BSFixedString> ("GetLength", "StringUtil", papyrusStringUtil::GetLength, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, BSFixedString, BSFixedString, UInt32> ("GetNthChar", "StringUtil", papyrusStringUtil::GetNthChar, registry));

	//registry->RegisterFunction(
	//	new NativeFunction1 <StaticFunctionTag, bool, BSFixedString> ("IsUppercase", "StringUtil", papyrusStringUtil::IsUppercase, registry));

	//registry->RegisterFunction(
	//	new NativeFunction1 <StaticFunctionTag, bool, BSFixedString> ("IsLowercase", "StringUtil", papyrusStringUtil::IsLowercase, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, bool, BSFixedString> ("IsLetter", "StringUtil", papyrusStringUtil::IsLetter, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, bool, BSFixedString> ("IsDigit", "StringUtil", papyrusStringUtil::IsDigit, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, bool, BSFixedString> ("IsPunctuation", "StringUtil", papyrusStringUtil::IsPunctuation, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, bool, BSFixedString> ("IsPrintable", "StringUtil", papyrusStringUtil::IsPrintable, registry));

	//registry->RegisterFunction(
	//	new NativeFunction1 <StaticFunctionTag, BSFixedString, BSFixedString> ("ToUpper", "StringUtil", papyrusStringUtil::ToUpper, registry));

	//registry->RegisterFunction(
	//	new NativeFunction1 <StaticFunctionTag, BSFixedString, BSFixedString> ("ToLower", "StringUtil", papyrusStringUtil::ToLower, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, SInt32, BSFixedString, BSFixedString, UInt32> ("Find", "StringUtil", papyrusStringUtil::Find, registry));

	registry->RegisterFunction(
		new NativeFunction3 <StaticFunctionTag, BSFixedString, BSFixedString, UInt32, UInt32> ("Substring", "StringUtil", papyrusStringUtil::Substring, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, BSFixedString> ("AsOrd", "StringUtil", papyrusStringUtil::AsOrd, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, BSFixedString, UInt32> ("AsChar", "StringUtil", papyrusStringUtil::AsChar, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, VMResultArray<BSFixedString>, BSFixedString, BSFixedString> ("Split", "StringUtil", papyrusStringUtil::Split, registry));

	registry->SetFunctionFlags("StringUtil", "GetLength", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("StringUtil", "GetNthChar", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("StringUtil", "IsLetter", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("StringUtil", "IsDigit", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("StringUtil", "IsPunctuation", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("StringUtil", "IsPrintable", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("StringUtil", "Find", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("StringUtil", "Substring", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("StringUtil", "AsOrd", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("StringUtil", "AsChar", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("StringUtil", "Split", VMClassRegistry::kFunctionFlag_NoWait);
}