#pragma once

class VMClassRegistry;
class BGSSoundDescriptorForm;

namespace papyrusSoundDescriptor
{
	float GetDecibelAttenuation(BGSSoundDescriptorForm* thisDescriptor);
	void SetDecibelAttenuation(BGSSoundDescriptorForm* thisDescriptor, float dbAttenuation);

	UInt32 GetDecibelVariance(BGSSoundDescriptorForm* thisDescriptor);
	void SetDecibelVariance(BGSSoundDescriptorForm* thisDescriptor, UInt32 dbVariance);

	UInt32 GetFrequencyVariance(BGSSoundDescriptorForm* thisDescriptor);
	void SetFrequencyVariance(BGSSoundDescriptorForm* thisDescriptor, UInt32 freqVariance);

	UInt32 GetFrequencyShift(BGSSoundDescriptorForm* thisDescriptor);
	void SetFrequencyShift(BGSSoundDescriptorForm* thisDescriptor, UInt32 freqShift);

	void RegisterFuncs(VMClassRegistry* registry);
};