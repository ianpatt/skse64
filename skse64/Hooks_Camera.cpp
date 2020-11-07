#include "Hooks_Camera.h"
#include "GameCamera.h"
#include "PapyrusEvents.h"
#include "skse64_common/BranchTrampoline.h"
#include "skse64_common/SafeWrite.h"
#include "xbyak/xbyak.h"
#include <queue>

RelocAddr <uintptr_t> kInstallCameraHook_Base(0x004F5C80);

void InstallCameraHook(TESCameraState * oldState, TESCameraState * newState)
{
	SKSECameraEvent evn(oldState, newState);
	g_cameraEventDispatcher.SendEvent(&evn);
}

void Hooks_Camera_Commit(void)
{
	struct InstallHookCamera_Code : Xbyak::CodeGenerator {
		InstallHookCamera_Code(void * buf) : Xbyak::CodeGenerator(4096, buf)
		{
			Xbyak::Label origFunctionP5;

			push(r12);
			push(r13);
			sub(rsp, 0x28);	// Remainder that stack must keep 16 bytes alignment or bad things will happen

			mov(r12, rcx);
			mov(r13, ptr[rcx + 0x28]);

			// Original code
			mov(ptr[rsp], rbx); // Using rsp+0 instead of rsp+8 of original instruction, because call will decrement rsp
			call(ptr[rip + origFunctionP5]);

			mov(rcx, r13); // oldState
			mov(rdx, ptr[r12 + 0x28]); // newState
			mov(rax, (uintptr_t)InstallCameraHook);
			call(rax);

			add(rsp, 0x28);
			pop(r13);
			pop(r12);
			ret();

			L(origFunctionP5);
			dq(kInstallCameraHook_Base.GetUIntPtr() + 0x5);
		}
	};

	void * codeBuf = g_localTrampoline.StartAlloc();
	InstallHookCamera_Code code(codeBuf);
	g_localTrampoline.EndAlloc(code.getCurr());

	g_branchTrampoline.Write5Branch(kInstallCameraHook_Base.GetUIntPtr(), uintptr_t(code.getCode()));	
}
