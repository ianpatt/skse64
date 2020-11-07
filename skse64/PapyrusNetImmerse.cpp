#include "PapyrusNetImmerse.h"

#include "GameAPI.h"
#include "GameForms.h"
#include "GameRTTI.h"
#include "GameReferences.h"
#include "GameObjects.h"
#include "GameThreads.h"

#include "NiNodes.h"
#include "NiGeometry.h"

#include "InternalTasks.h"

#include "PapyrusArgs.h"

namespace papyrusNetImmerse
{
	NiAVObject * ResolveNode(TESObjectREFR * obj, BSFixedString nodeName, bool firstPerson)
	{
		if(!obj) return NULL;

		NiAVObject	* result = obj->GetNiNode();

		// special-case for the player, switch between first/third-person
		PlayerCharacter * player = DYNAMIC_CAST(obj, TESObjectREFR, PlayerCharacter);
		if(player && player->loadedState)
			result = firstPerson ? player->firstPersonSkeleton : player->loadedState->node;

		// name lookup
		if(obj && nodeName.data[0] && result)
			result = result->GetObjectByName(&nodeName.data);

		return result;
	}

	bool HasNode(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		return object != NULL;
	}

	float GetNodeWorldPositionX(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		return object ? object->m_worldTransform.pos.x : 0;
	}

	float GetNodeWorldPositionY(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		return object ? object->m_worldTransform.pos.y : 0;
	}

	float GetNodeWorldPositionZ(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		return object ? object->m_worldTransform.pos.z : 0;
	}

	bool GetNodeWorldPosition(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, VMArray<float> inArray, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);
		if(object && inArray.Length() == 3) {
			inArray.Set(&object->m_worldTransform.pos.x, 0);
			inArray.Set(&object->m_worldTransform.pos.y, 1);
			inArray.Set(&object->m_worldTransform.pos.z, 2);
			return true;
		}

		return false;
	}

	float GetNodeLocalPositionX(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		return object ? object->m_localTransform.pos.x : 0;
	}

	float GetNodeLocalPositionY(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		return object ? object->m_localTransform.pos.y : 0;
	}

	float GetNodeLocalPositionZ(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		return object ? object->m_localTransform.pos.z : 0;
	}

	bool GetNodeLocalPosition(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, VMArray<float> inArray, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);
		if(object && inArray.Length() == 3) {
			inArray.Set(&object->m_localTransform.pos.x, 0);
			inArray.Set(&object->m_localTransform.pos.y, 1);
			inArray.Set(&object->m_localTransform.pos.z, 2);
			return true;
		}

		return false;
	}

	float GetRelativeNodePositionX(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeNameA, BSFixedString nodeNameB, bool firstPerson)
	{
		NiAVObject	* objectA = ResolveNode(obj, nodeNameA, firstPerson);
		NiAVObject	* objectB = ResolveNode(obj, nodeNameB, firstPerson);

		return (objectA && objectB) ? objectB->m_worldTransform.pos.x - objectA->m_worldTransform.pos.x : 0;
	}

	float GetRelativeNodePositionY(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeNameA, BSFixedString nodeNameB, bool firstPerson)
	{
		NiAVObject	* objectA = ResolveNode(obj, nodeNameA, firstPerson);
		NiAVObject	* objectB = ResolveNode(obj, nodeNameB, firstPerson);

		return (objectA && objectB) ? objectB->m_worldTransform.pos.y - objectA->m_worldTransform.pos.y : 0;
	}

	float GetRelativeNodePositionZ(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeNameA, BSFixedString nodeNameB, bool firstPerson)
	{
		NiAVObject	* objectA = ResolveNode(obj, nodeNameA, firstPerson);
		NiAVObject	* objectB = ResolveNode(obj, nodeNameB, firstPerson);

		return (objectA && objectB) ? objectB->m_worldTransform.pos.z - objectA->m_worldTransform.pos.z : 0;
	}

	bool GetRelativeNodePosition(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeNameA, BSFixedString nodeNameB, VMArray<float> inArray, bool firstPerson)
	{
		NiAVObject	* objectA = ResolveNode(obj, nodeNameA, firstPerson);
		NiAVObject	* objectB = ResolveNode(obj, nodeNameB, firstPerson);
		if(objectA && objectB && inArray.Length() == 3) {
			float x = objectB->m_worldTransform.pos.x - objectA->m_worldTransform.pos.x;
			float y = objectB->m_worldTransform.pos.y - objectA->m_worldTransform.pos.y;
			float z = objectB->m_worldTransform.pos.z - objectA->m_worldTransform.pos.z;
			inArray.Set(&x, 0);
			inArray.Set(&y, 1);
			inArray.Set(&z, 2);
			return true;
		}

		return false;
	}

	float GetNodeScale(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		return object ? object->m_localTransform.scale : 0;
	}

	void SetNodeLocalPositionX(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, float value, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		if(object)
		{
			object->m_localTransform.pos.x = value;
			TaskInterface::UpdateWorldData(object);
		}
	}

	void SetNodeLocalPositionY(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, float value, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		if(object)
		{
			object->m_localTransform.pos.y = value;
			TaskInterface::UpdateWorldData(object);
		}
	}

	void SetNodeLocalPositionZ(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, float value, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		if(object)
		{
			object->m_localTransform.pos.z = value;
			TaskInterface::UpdateWorldData(object);
		}
	}

	bool SetNodeLocalPosition(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, VMArray<float> inArray, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);
		if(object && inArray.Length() == 3) {
			inArray.Get(&object->m_localTransform.pos.x, 0);
			inArray.Get(&object->m_localTransform.pos.y, 1);
			inArray.Get(&object->m_localTransform.pos.z, 2);

			TaskInterface::UpdateWorldData(object);

			return true;
		}

		return false;
	}

	bool GetNodeLocalRotationEuler(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, VMArray<float> inArray, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);
		if(object && inArray.Length() == 3) {
			float heading, attitude, bank;
			
			object->m_localTransform.rot.GetEulerAngles(&heading, &attitude, &bank);

			// Radians to degrees
			heading *= 180 / MATH_PI;
			attitude *= 180 / MATH_PI;
			bank *= 180 / MATH_PI;

			inArray.Set(&heading, 0);
			inArray.Set(&attitude, 1);
			inArray.Set(&bank, 2);
			return true;
		}

		return false;
	}

	
	bool GetNodeWorldRotationEuler(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, VMArray<float> inArray, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);
		if(object && inArray.Length() == 3) {
			float heading, attitude, bank;
			object->m_worldTransform.rot.GetEulerAngles(&heading, &attitude, &bank);

			// Radians to degrees
			heading *= 180 / MATH_PI;
			attitude *= 180 / MATH_PI;
			bank *= 180 / MATH_PI;

			inArray.Set(&heading, 0);
			inArray.Set(&attitude, 1);
			inArray.Set(&bank, 2);
			return true;
		}

		return false;
	}

	
	bool SetNodeLocalRotationEuler(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, VMArray<float> inArray, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);
		if(object && inArray.Length() == 3) {
			float heading, attitude, bank;

			inArray.Get(&heading, 0);
			inArray.Get(&attitude, 1);
			inArray.Get(&bank, 2);

			// Degrees to radians
			heading *= MATH_PI / 180;
			attitude *= MATH_PI / 180;
			bank *= MATH_PI / 180;

			object->m_localTransform.rot.SetEulerAngles(heading, attitude, bank);

			TaskInterface::UpdateWorldData(object);

			return true;
		}

		return false;
	}

	bool SetNodeLocalRotationMatrix(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, VMArray<float> inArray, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);
		if(object && inArray.Length() == 9) {
			inArray.Get(&object->m_localTransform.rot.data[0][0], 0);
			inArray.Get(&object->m_localTransform.rot.data[0][1], 1);
			inArray.Get(&object->m_localTransform.rot.data[0][2], 2);

			inArray.Get(&object->m_localTransform.rot.data[1][0], 3);
			inArray.Get(&object->m_localTransform.rot.data[1][1], 4);
			inArray.Get(&object->m_localTransform.rot.data[1][2], 5);

			inArray.Get(&object->m_localTransform.rot.data[2][0], 6);
			inArray.Get(&object->m_localTransform.rot.data[2][1], 7);
			inArray.Get(&object->m_localTransform.rot.data[2][2], 8);

			TaskInterface::UpdateWorldData(object);

			return true;
		}

		return false;
	}

	bool GetNodeLocalRotationMatrix(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, VMArray<float> inArray, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);
		if(object && inArray.Length() == 9) {
			inArray.Set(&object->m_localTransform.rot.data[0][0], 0);
			inArray.Set(&object->m_localTransform.rot.data[0][1], 1);
			inArray.Set(&object->m_localTransform.rot.data[0][2], 2);

			inArray.Set(&object->m_localTransform.rot.data[1][0], 3);
			inArray.Set(&object->m_localTransform.rot.data[1][1], 4);
			inArray.Set(&object->m_localTransform.rot.data[1][2], 5);

			inArray.Set(&object->m_localTransform.rot.data[2][0], 6);
			inArray.Set(&object->m_localTransform.rot.data[2][1], 7);
			inArray.Set(&object->m_localTransform.rot.data[2][2], 8);
			return true;
		}

		return false;
	}

	bool GetNodeWorldRotationMatrix(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, VMArray<float> inArray, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);
		if(object && inArray.Length() == 9) {
			inArray.Set(&object->m_worldTransform.rot.data[0][0], 0);
			inArray.Set(&object->m_worldTransform.rot.data[0][1], 1);
			inArray.Set(&object->m_worldTransform.rot.data[0][2], 2);

			inArray.Set(&object->m_worldTransform.rot.data[1][0], 3);
			inArray.Set(&object->m_worldTransform.rot.data[1][1], 4);
			inArray.Set(&object->m_worldTransform.rot.data[1][2], 5);

			inArray.Set(&object->m_worldTransform.rot.data[2][0], 6);
			inArray.Set(&object->m_worldTransform.rot.data[2][1], 7);
			inArray.Set(&object->m_worldTransform.rot.data[2][2], 8);
			return true;
		}

		return false;
	}

	void SetNodeScale(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, float value, bool firstPerson)
	{
		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		if(object)
		{
			object->m_localTransform.scale = value;
			TaskInterface::UpdateWorldData(object);
		}
	}

	void SetNodeTextureSet(StaticFunctionTag* base, TESObjectREFR * obj, BSFixedString nodeName, BGSTextureSet * textureSet, bool firstPerson)
	{
		if(!textureSet) return;

		NiAVObject	* object = ResolveNode(obj, nodeName, firstPerson);

		if(object)
		{
			BSGeometry * geometry = object->GetAsBSGeometry();
			if(geometry)
			{
				BSTaskPool * taskPool = BSTaskPool::GetSingleton();
				if(taskPool)
				{
					CALL_MEMBER_FN(taskPool, SetNiGeometryTexture)(geometry, textureSet);
				}
			}
		}
	}
};

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusNetImmerse::RegisterFuncs(VMClassRegistry* registry)
{
	// NiNode Manipulation
	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString, bool>("HasNode", "NetImmerse", papyrusNetImmerse::HasNode, registry));

	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, bool>("GetNodeWorldPositionX", "NetImmerse", papyrusNetImmerse::GetNodeWorldPositionX, registry));

	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, bool>("GetNodeWorldPositionY", "NetImmerse", papyrusNetImmerse::GetNodeWorldPositionY, registry));

	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, bool>("GetNodeWorldPositionZ", "NetImmerse", papyrusNetImmerse::GetNodeWorldPositionZ, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString, VMArray<float>, bool>("GetNodeWorldPosition", "NetImmerse", papyrusNetImmerse::GetNodeWorldPosition, registry));

	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, bool>("GetNodeLocalPositionX", "NetImmerse", papyrusNetImmerse::GetNodeLocalPositionX, registry));

	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, bool>("GetNodeLocalPositionY", "NetImmerse", papyrusNetImmerse::GetNodeLocalPositionY, registry));

	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, bool>("GetNodeLocalPositionZ", "NetImmerse", papyrusNetImmerse::GetNodeLocalPositionZ, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString, VMArray<float>, bool>("GetNodeLocalPosition", "NetImmerse", papyrusNetImmerse::GetNodeLocalPosition, registry));

	registry->RegisterFunction(
		new NativeFunction3<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, bool>("GetNodeScale", "NetImmerse", papyrusNetImmerse::GetNodeScale, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, void, TESObjectREFR*, BSFixedString, float, bool>("SetNodeScale", "NetImmerse", papyrusNetImmerse::SetNodeScale, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, void, TESObjectREFR*, BSFixedString, float, bool>("SetNodeLocalPositionX", "NetImmerse", papyrusNetImmerse::SetNodeLocalPositionX, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, void, TESObjectREFR*, BSFixedString, float, bool>("SetNodeLocalPositionY", "NetImmerse", papyrusNetImmerse::SetNodeLocalPositionY, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, void, TESObjectREFR*, BSFixedString, float, bool>("SetNodeLocalPositionZ", "NetImmerse", papyrusNetImmerse::SetNodeLocalPositionZ, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString, VMArray<float>, bool>("SetNodeLocalPosition", "NetImmerse", papyrusNetImmerse::SetNodeLocalPosition, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString, VMArray<float>, bool>("GetNodeLocalRotationEuler", "NetImmerse", papyrusNetImmerse::GetNodeLocalRotationEuler, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString, VMArray<float>, bool>("SetNodeLocalRotationEuler", "NetImmerse", papyrusNetImmerse::SetNodeLocalRotationEuler, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString, VMArray<float>, bool>("GetNodeLocalRotationMatrix", "NetImmerse", papyrusNetImmerse::GetNodeLocalRotationMatrix, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString, VMArray<float>, bool>("SetNodeLocalRotationMatrix", "NetImmerse", papyrusNetImmerse::SetNodeLocalRotationMatrix, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString, VMArray<float>, bool>("GetNodeWorldRotationEuler", "NetImmerse", papyrusNetImmerse::GetNodeWorldRotationEuler, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString, VMArray<float>, bool>("GetNodeWorldRotationMatrix", "NetImmerse", papyrusNetImmerse::GetNodeWorldRotationMatrix, registry));


	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, void, TESObjectREFR*, BSFixedString, BGSTextureSet*, bool>("SetNodeTextureSet", "NetImmerse", papyrusNetImmerse::SetNodeTextureSet, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, BSFixedString, bool>("GetRelativeNodePositionX", "NetImmerse", papyrusNetImmerse::GetRelativeNodePositionX, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, BSFixedString, bool>("GetRelativeNodePositionY", "NetImmerse", papyrusNetImmerse::GetRelativeNodePositionY, registry));

	registry->RegisterFunction(
		new NativeFunction4<StaticFunctionTag, float, TESObjectREFR*, BSFixedString, BSFixedString, bool>("GetRelativeNodePositionZ", "NetImmerse", papyrusNetImmerse::GetRelativeNodePositionZ, registry));

	registry->RegisterFunction(
		new NativeFunction5<StaticFunctionTag, bool, TESObjectREFR*, BSFixedString, BSFixedString, VMArray<float>, bool>("GetRelativeNodePosition", "NetImmerse", papyrusNetImmerse::GetRelativeNodePosition, registry));

	/*registry->SetFunctionFlags("NetImmerse", "HasNode", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("NetImmerse", "GetNodePositionX", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("NetImmerse", "GetNodePositionY", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("NetImmerse", "GetNodePositionZ", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("NetImmerse", "GetNodeScale", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("NetImmerse", "SetNodeScale", VMClassRegistry::kFunctionFlag_NoWait);*/
}
