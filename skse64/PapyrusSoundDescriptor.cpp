#include "PapyrusSoundDescriptor.h"

#include "GameFormComponents.h"
#include "GameForms.h"
#include "GameObjects.h"

namespace papyrusSoundDescriptor
{
	float GetDecibelAttenuation(BGSSoundDescriptorForm* thisDescriptor)
	{
		return (thisDescriptor && thisDescriptor->standardSoundDef) ? (thisDescriptor->standardSoundDef->soundCharacteristics.dbAttenuation / 100.0) : 0.0;
	}

	void SetDecibelAttenuation(BGSSoundDescriptorForm* thisDescriptor, float dbAttenuation)
	{
		if(thisDescriptor && thisDescriptor->standardSoundDef) {
			thisDescriptor->standardSoundDef->soundCharacteristics.dbAttenuation = (UInt16)(dbAttenuation * 100);
		}
	}

	UInt32 GetDecibelVariance(BGSSoundDescriptorForm* thisDescriptor)
	{
		return (thisDescriptor && thisDescriptor->standardSoundDef) ? thisDescriptor->standardSoundDef->soundCharacteristics.dbVariance : 0;
	}

	void SetDecibelVariance(BGSSoundDescriptorForm* thisDescriptor, UInt32 dbVariance)
	{
		if(thisDescriptor && thisDescriptor->standardSoundDef && dbVariance < 255) {
			thisDescriptor->standardSoundDef->soundCharacteristics.dbVariance = dbVariance;
		}
	}

	UInt32 GetFrequencyVariance(BGSSoundDescriptorForm* thisDescriptor)
	{
		return (thisDescriptor && thisDescriptor->standardSoundDef) ? thisDescriptor->standardSoundDef->soundCharacteristics.frequencyVariance : 0;
	}

	void SetFrequencyVariance(BGSSoundDescriptorForm* thisDescriptor, UInt32 freqVariance)
	{
		if(thisDescriptor && thisDescriptor->standardSoundDef && freqVariance < 255) {
			thisDescriptor->standardSoundDef->soundCharacteristics.frequencyVariance = freqVariance;
		}
	}

	UInt32 GetFrequencyShift(BGSSoundDescriptorForm* thisDescriptor)
	{
		return (thisDescriptor && thisDescriptor->standardSoundDef) ? thisDescriptor->standardSoundDef->soundCharacteristics.frequencyShift : 0;
	}

	void SetFrequencyShift(BGSSoundDescriptorForm* thisDescriptor, UInt32 freqShift)
	{
		if(thisDescriptor && thisDescriptor->standardSoundDef && freqShift < 255) {
			thisDescriptor->standardSoundDef->soundCharacteristics.frequencyShift = freqShift;
		}
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusSoundDescriptor::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterForm(BGSSoundDescriptorForm::kTypeID, "SoundDescriptor");

	// Decibel Attenuation
	registry->RegisterFunction(
		new NativeFunction0 <BGSSoundDescriptorForm, float> ("GetDecibelAttenuation", "SoundDescriptor", papyrusSoundDescriptor::GetDecibelAttenuation, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSSoundDescriptorForm, void, float> ("SetDecibelAttenuation", "SoundDescriptor", papyrusSoundDescriptor::SetDecibelAttenuation, registry));

	// Decibel Variance
	registry->RegisterFunction(
		new NativeFunction0 <BGSSoundDescriptorForm, UInt32> ("GetDecibelVariance", "SoundDescriptor", papyrusSoundDescriptor::GetDecibelVariance, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSSoundDescriptorForm, void, UInt32> ("SetDecibelVariance", "SoundDescriptor", papyrusSoundDescriptor::SetDecibelVariance, registry));

	// Frequency Variance
	registry->RegisterFunction(
		new NativeFunction0 <BGSSoundDescriptorForm, UInt32> ("GetFrequencyVariance", "SoundDescriptor", papyrusSoundDescriptor::GetFrequencyVariance, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSSoundDescriptorForm, void, UInt32> ("SetFrequencyVariance", "SoundDescriptor", papyrusSoundDescriptor::SetFrequencyVariance, registry));

	// Frequency Shift
	registry->RegisterFunction(
		new NativeFunction0 <BGSSoundDescriptorForm, UInt32> ("GetFrequencyShift", "SoundDescriptor", papyrusSoundDescriptor::GetFrequencyShift, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSSoundDescriptorForm, void, UInt32> ("SetFrequencyShift", "SoundDescriptor", papyrusSoundDescriptor::SetFrequencyShift, registry));
}
