#include "ScaleformState.h"
#include "skse64/GameAPI.h"

RelocAddr<BSScaleformTranslator::_GetCachedString> BSScaleformTranslator::GetCachedString(0x00C4F5D0);

void SKSEGFxLogger::LogMessageVarg(UInt32 messageType, const char* fmt, va_list args)
{
	gLog.Log(IDebugLog::kLevel_Message, fmt, args);
}
