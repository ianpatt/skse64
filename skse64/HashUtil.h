#pragma once

namespace HashUtil
{
	// Calc CRC32 of null terminated string
	UInt32 CRC32(const char* str, UInt32 start = 0);
}
