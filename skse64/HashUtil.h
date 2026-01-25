#pragma once

#include <functional>
#include "GameTypes.h"

namespace HashUtil
{
	// Calc CRC32 of null terminated string
	UInt32 CRC32(const char* str, UInt32 start = 0);
}

// Hash function for BSFixedString to enable std::unordered_map
// BSFixedString uses StringCache - identical strings share same pointer
// We hash the pointer address for O(1) lookup
namespace std {
	template<>
	struct hash<BSFixedString> {
		size_t operator()(const BSFixedString& str) const {
			return hash<const char*>()(str.data);
		}
	};
}
