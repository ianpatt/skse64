#include "PapyrusWeather.h"

#include "GameForms.h"

namespace papyrusWeather
{
	float GetSunGlare(TESWeather* thisWeather)
	{
		return (thisWeather) ? (thisWeather->general.sunGlare / 256.0) : 0.0;
	}

	float GetSunDamage(TESWeather* thisWeather)
	{
		return (thisWeather) ? (thisWeather->general.sunDamage / 256.0) : 0.0;
	}

	float GetWindDirection(TESWeather* thisWeather)
	{
		return (thisWeather) ? (thisWeather->general.windDirection / (256.0 / 360.0)) : 0.0;
	}

	float GetWindDirectionRange(TESWeather* thisWeather)
	{
		return (thisWeather) ? (thisWeather->general.windDirRange / (256.0 / 180.0)) : 0.0;
	}

	float GetFogDistance(TESWeather* thisWeather, bool day, UInt32 fdType)
	{
		if(!thisWeather)
			return 0.0;

		switch(fdType)
		{
			case 0:
				return day ? thisWeather->fogDistance.nearDay : thisWeather->fogDistance.nearNight;
				break;
			case 1:
				return day ? thisWeather->fogDistance.farDay : thisWeather->fogDistance.farNight;
				break;
			case 2:
				return day ? thisWeather->fogDistance.powerDay : thisWeather->fogDistance.powerNight;
				break;
			case 3:
				return day ? thisWeather->fogDistance.maxDay : thisWeather->fogDistance.maxNight;
				break;
		}

		return 0.0;
	}
};

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusWeather::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0 <TESWeather, float> ("GetSunGlare", "Weather", papyrusWeather::GetSunGlare, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESWeather, float> ("GetSunDamage", "Weather", papyrusWeather::GetSunDamage, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESWeather, float> ("GetWindDirection", "Weather", papyrusWeather::GetWindDirection, registry));

	registry->RegisterFunction(
		new NativeFunction0 <TESWeather, float> ("GetWindDirectionRange", "Weather", papyrusWeather::GetWindDirectionRange, registry));

	registry->RegisterFunction(
		new NativeFunction2 <TESWeather, float, bool, UInt32> ("GetFogDistance", "Weather", papyrusWeather::GetFogDistance, registry));
}
