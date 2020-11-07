#include "PapyrusCamera.h"
#include "GameReferences.h"
#include "GameCamera.h"

namespace papyrusCamera
{
	SInt32 GetCameraState(StaticFunctionTag * base)
	{
		PlayerCamera * playerCamera = PlayerCamera::GetSingleton();
		if(playerCamera) {
			for(int i = 0; i < PlayerCamera::kNumCameraStates; i++) {
				if(playerCamera->cameraState == playerCamera->cameraStates[i])
					return i;
			}
		}

		return -1;
	}

	void UpdateThirdPerson(StaticFunctionTag * base)
	{
		PlayerCharacter* pPC = (*g_thePlayer);
		PlayerCamera * camera = PlayerCamera::GetSingleton();
		if(pPC && camera) {
			CALL_MEMBER_FN(camera, UpdateThirdPerson)(pPC->actorState.IsWeaponDrawn());
		}
	}

	float GetWorldFieldOfView(StaticFunctionTag * base)
	{
		PlayerCamera * playerCamera = PlayerCamera::GetSingleton();
		if(playerCamera) {
			return playerCamera->worldFOV;
		}

		return 0;
	}

	void SetWorldFieldOfView(StaticFunctionTag * base, float fov)
	{
		PlayerCamera * playerCamera = PlayerCamera::GetSingleton();
		if(playerCamera) {
			playerCamera->worldFOV = fov;
		}
	}

	float GetFirstPersonFieldOfView(StaticFunctionTag * base)
	{
		PlayerCamera * playerCamera = PlayerCamera::GetSingleton();
		if(playerCamera) {
			return playerCamera->firstPersonFOV;
		}

		return 0;
	}

	void SetFirstPersonFieldOfView(StaticFunctionTag * base, float fov)
	{
		PlayerCamera * playerCamera = PlayerCamera::GetSingleton();
		if(playerCamera) {
			playerCamera->firstPersonFOV = fov;
		}
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusCamera::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, SInt32>("GetCameraState", "Camera", papyrusCamera::GetCameraState, registry));

	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, void>("UpdateThirdPerson", "Camera", papyrusCamera::UpdateThirdPerson, registry));

	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, float>("GetWorldFieldOfView", "Camera", papyrusCamera::GetWorldFieldOfView, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, float>("SetWorldFieldOfView", "Camera", papyrusCamera::SetWorldFieldOfView, registry));

	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, float>("GetFirstPersonFieldOfView", "Camera", papyrusCamera::GetFirstPersonFieldOfView, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, void, float>("SetFirstPersonFieldOfView", "Camera", papyrusCamera::SetFirstPersonFieldOfView, registry));

	registry->SetFunctionFlags("Camera", "GetCameraState", VMClassRegistry::kFunctionFlag_NoWait);
}
