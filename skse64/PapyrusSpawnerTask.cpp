#include "PapyrusSpawnerTask.h"

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

#include "GameForms.h"
#include "GameReferences.h"
#include "NiTypes.h"

#include "PapyrusObjects.h"
#include "PapyrusDelayFunctors.h"
#include "Serialization.h"

#include <vector>


namespace
{
///
/// SpawnerTaskFunctor
///

	class SpawnerTaskFunctor : public LatentSKSEDelayFunctor
	{
		struct SpawnData;

	public:
		explicit SpawnerTaskFunctor(UInt32 stackId) :
			LatentSKSEDelayFunctor( stackId )
		{}

		explicit SpawnerTaskFunctor(SerializationTag tag) :
			LatentSKSEDelayFunctor( tag )
		{}

		virtual const char* ClassName() const		{ return "SpawnerTaskFunctor"; }
		virtual UInt32		ClassVersion() const	{ return 1; }

		virtual bool Save(SKSESerializationInterface* intfc);
		virtual bool Load(SKSESerializationInterface* intfc, UInt32 version);

		virtual void Run(VMValue& resultValue)
		{
			VMClassRegistry* registry = (*g_skyrimVM)->GetClassRegistry();

			UInt32 nullHandle = *g_invalidRefHandle;

			VMResultArray<TESObjectREFR*> allSpawned;

			for (size_t i=0; i<data_.size(); i++)
			{
				SpawnData& e = data_[i];

				// Lookup form
				TESForm* spawnForm = LookupFormByID(e.spawnFormId);
				if (spawnForm == NULL)
					continue;

				// Lookup target
				NiPointer<TESObjectREFR> target;
				LookupREFRByHandle(e.targetRefrHandle, target);
				if (target == NULL)
					continue;

				// Spawn
				TESObjectREFR* spawned = PlaceAtMe_Native(registry, this->stackId_, target, spawnForm, e.count, e.bForcePersist, e.bInitiallyDisabled);
				if (spawned == NULL)
					continue;

				// Position
				TESObjectCELL* parentCell = spawned->parentCell;
				TESWorldSpace* worldspace = CALL_MEMBER_FN(spawned,GetWorldspace)();
				
				NiPoint3 finalPos( e.pos );
				finalPos += target->pos;

				MoveRefrToPosition(spawned, &nullHandle, parentCell, worldspace, &finalPos, &e.rot);

				// Save spawned for result
				allSpawned.push_back(spawned);
			}

			PackValue(&resultValue, &allSpawned, registry);
		}

		void AddSpawn(TESForm* form, TESObjectREFR* target, const NiPoint3& pos, const NiPoint3& rot,
					  UInt32 count, bool bForcePersist, bool bInitiallyDisabled)
		{
			UInt32 formId = form->formID;
			UInt32 targetRefrHandle = GetOrCreateRefrHandle(target);

			if (targetRefrHandle == *g_invalidRefHandle)
				return;

			SpawnData t = { formId, targetRefrHandle, pos, rot, count, bForcePersist, bInitiallyDisabled };
			data_.push_back(t);
		}

	private:
		struct SpawnData
		{
			UInt32			spawnFormId;
			UInt32			targetRefrHandle;
			NiPoint3		pos;
			NiPoint3		rot;
			UInt32			count;
			bool			bForcePersist;
			bool			bInitiallyDisabled;
		};

		std::vector<SpawnData>	data_;
	};

	bool SpawnerTaskFunctor::Save(SKSESerializationInterface* intfc)
	{
		using namespace Serialization;

		if (! LatentSKSEDelayFunctor::Save(intfc))
			return false;

		UInt32 dataSize = data_.size();

		if (! WriteData(intfc, &dataSize))
			return false;

		for (UInt32 i=0; i<dataSize; i++)
			if (! WriteData(intfc, &data_[i]))
				return false;

		return true;
	}

	bool SpawnerTaskFunctor::Load(SKSESerializationInterface* intfc, UInt32 version)
	{
		using namespace Serialization;

		if (! LatentSKSEDelayFunctor::Load(intfc, version))
			return false;

		UInt32 dataSize;;

		if (! ReadData(intfc, &dataSize))
			return false;

		// Note: Reserve, not resize.
		// If formIds fail to resolve, we may skip entries.
		data_.reserve(dataSize);

		for (UInt32 i=0; i<dataSize; i++)
		{
			SpawnData e;

			if (! ReadData(intfc, &e))
				return false;

			// Fixup formid of spawn. Might change if load order was changed.
			UInt32 fixedFormId;
			if (! intfc->ResolveFormId(e.spawnFormId, &fixedFormId))
			{
				_MESSAGE("SpawnerTaskFunctor::Load: FormID of spawn %i could not be resolved, skipping.");
				continue;
			}

			e.spawnFormId = fixedFormId; 

			data_.push_back(e);
		}

		return true;
	}
}

///
/// Native functions
///

namespace papyrusSpawnerTask
{
	SInt32 Create(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*)
	{
		SpawnerTaskFunctor* func = new SpawnerTaskFunctor(stackId);

		UInt32 handle = SKSEObjectStorageInstance().StoreObject(func, stackId);
		return handle;
	}

	void AddSpawn(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle, TESForm* form, TESObjectREFR* target,
				  VMArray<float> position,  VMArray<float> rotation, SInt32 count, bool bForcePersist, bool bInitiallyDisabled)
	{
		ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId)
		ERROR_AND_RETURN_IF(form == NULL, "FormToPlace parameter must not be none.", registry, stackId)
		ERROR_AND_RETURN_IF(target == NULL, "Target parameter must not be none.", registry, stackId)
		ERROR_AND_RETURN_IF(count <= 0, "Count parameter must be greater zero.", registry, stackId)
		ERROR_AND_RETURN_IF(position.Length() != 3, "Position parameter must be an array of length 3.", registry, stackId)
		ERROR_AND_RETURN_IF(rotation.Length() != 3, "Rotation parameter must be an array of length 3.", registry, stackId)

		SpawnerTaskFunctor* func = SKSEObjectStorageInstance().AccessObject<SpawnerTaskFunctor>(handle);

		ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId)

		NiPoint3 pos;
		position.Get(&pos.x, 0);
		position.Get(&pos.y, 1);
		position.Get(&pos.z, 2);
		
		NiPoint3 rot;
		rotation.Get(&rot.x, 0);
		rotation.Get(&rot.y, 1);
		rotation.Get(&rot.z, 2);

		func->AddSpawn(form, target, pos, rot, count, bForcePersist, bInitiallyDisabled);
	}

	bool Run(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle)
	{
		ERROR_AND_RETURN_0_IF(handle <= 0, "Invalid handle.", registry, stackId)

		SpawnerTaskFunctor* func = SKSEObjectStorageInstance().TakeObject<SpawnerTaskFunctor>(handle);

		ERROR_AND_RETURN_0_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId)

		if (stackId != func->StackId())
		{
			registry->LogError("Tried to run SpawnerTaskFunctor from non-owning stack.", stackId);
			
			delete func;
			return false;
		}

		SKSEDelayFunctorManagerInstance().Enqueue(func);

		return true;
	}

	void Cancel(VMClassRegistry* registry, UInt32 stackId, StaticFunctionTag*, SInt32 handle)
	{
		ERROR_AND_RETURN_IF(handle <= 0, "Invalid handle.", registry, stackId)

		SpawnerTaskFunctor* func = SKSEObjectStorageInstance().TakeObject<SpawnerTaskFunctor>(handle);

		ERROR_AND_RETURN_IF(func == NULL, "Failed to lookup object for given handle.", registry, stackId)

		delete func;
	}
}

void papyrusSpawnerTask::RegisterFuncs(VMClassRegistry* registry)
{
	SKSEObjectRegistry& skseObjRegistry = SKSEObjectRegistryInstance();
	skseObjRegistry.RegisterClass<SpawnerTaskFunctor>();

	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, SInt32>("Create", "SpawnerTask", papyrusSpawnerTask::Create, registry));

	registry->RegisterFunction(
		new NativeFunction8 <StaticFunctionTag, void, SInt32, TESForm*, TESObjectREFR*, VMArray<float>, VMArray<float>, SInt32, bool, bool>("AddSpawn", "SpawnerTask", papyrusSpawnerTask::AddSpawn, registry));

	registry->RegisterFunction(
		new LatentNativeFunction1 <StaticFunctionTag, VMResultArray<TESObjectREFR*>, SInt32>("Run", "SpawnerTask", papyrusSpawnerTask::Run, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, SInt32>("Cancel", "SpawnerTask", papyrusSpawnerTask::Cancel, registry));

	registry->SetFunctionFlags("SpawnerTask", "Create", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("SpawnerTask", "AddSpawn", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("SpawnerTask", "Cancel", VMClassRegistry::kFunctionFlag_NoWait);
	
}