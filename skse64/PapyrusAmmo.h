#pragma once

class VMClassRegistry;
class BGSSoundDescriptorForm;
class TESAmmo;
class BGSProjectile;

namespace papyrusAmmo
{
	void RegisterFuncs(VMClassRegistry* registry);

	bool IsPlayable(TESAmmo * thisAmmo);
	bool IsBolt(TESAmmo * thisAmmo);
	BGSProjectile * GetProjectile(TESAmmo * thisAmmo);
	float GetDamage(TESAmmo * thisAmmo);
};
