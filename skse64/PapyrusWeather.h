#pragma once

class VMClassRegistry;

class TESWeather;

namespace papyrusWeather
{
	float GetSunGlare(TESWeather* thisWeather);
	float GetSunDamage(TESWeather* thisWeather);
	float GetWindDirection(TESWeather* thisWeather);
	float GetWindDirectionRange(TESWeather* thisWeather);
	float GetFogDistance(TESWeather* thisWeather, bool day, UInt32 fdType);

	void RegisterFuncs(VMClassRegistry* registry);
}