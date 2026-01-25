#include "Hooks_NetImmerse.h"
#include "skse64_common/SafeWrite.h"
#include "skse64_common/Utilities.h"
#include "skse64_common/BranchTrampoline.h"

#include "skse64/GameReferences.h"
#include "skse64/GameFormComponents.h"
#include "skse64/PapyrusEvents.h"

#include "common/ICriticalSection.h"
#include "xbyak/xbyak.h"

// Used by UpdatePlayerTints to create a render target of a particular size
// 799F89E4A13D41B1F88644E1F6AFA5A633684C51+345
RelocPtr<UInt32> g_tintTextureWidth(0x0302BCC4);
// 799F89E4A13D41B1F88644E1F6AFA5A633684C51+33E
RelocPtr<UInt32> g_tintTextureHeight(0x0302BCC8);

UInt32 g_tintTextureResolution = 512;

struct RenderTargetInfo
{
	UInt32	width;
	UInt32	height;
	UInt32	unk08;
	UInt32	unk0C;
	SInt32	unk10;
	UInt32	unk14;
	UInt16	unk18;
	UInt16	unk1A;
	UInt32	unk1C;
};

typedef void ( * _CreateRenderTarget)(void * renderTargetManager, UInt64 type, RenderTargetInfo * info);
RelocAddr<_CreateRenderTarget> CreateRenderTarget(0x00D74B60);
_CreateRenderTarget CreateRenderTarget_Original = nullptr;

void CreateRenderTarget_Hook(BSRenderManager ** targetManager, UInt64 type, RenderTargetInfo * info)
{
	if (type == 0xF)
	{
		info->width = g_tintTextureResolution;
		info->height = g_tintTextureResolution;

		*g_tintTextureWidth = g_tintTextureResolution;
		*g_tintTextureHeight = g_tintTextureResolution;
	}
	CreateRenderTarget_Original(targetManager, type, info);
}

void ActorProcessManager::UpdateEquipment_Hooked(Actor * actor)
{
	CALL_MEMBER_FN(this, UpdateEquipment)(actor);

	SKSENiNodeUpdateEvent evn(actor);
	g_ninodeUpdateEventDispatcher.SendEvent(&evn);
}

void Hooks_NetImmerse_Init(void)
{
	UInt32	tintTextureResolution = 0;
	if(GetConfigOption_UInt32("Display", "iTintTextureResolution", &tintTextureResolution))
	{
		g_tintTextureResolution = tintTextureResolution;
	}
}

RelocAddr <uintptr_t> UpdateEquipment_Enter(0x00693110 + 0x189);

void Hooks_NetImmerse_Commit(void)
{
	{
		struct CreateRenderTarget_Code : Xbyak::CodeGenerator {
			CreateRenderTarget_Code(void * buf) : Xbyak::CodeGenerator(4096, buf)
			{
				Xbyak::Label retnLabel;

				movups(xmm0, ptr[r8]);
				movsxd(rax, edx);

				jmp(ptr[rip + retnLabel]);

				L(retnLabel);
				dq(CreateRenderTarget.GetUIntPtr() + 7);
			}
		};

		void * codeBuf = g_localTrampoline.StartAlloc();
		CreateRenderTarget_Code code(codeBuf);
		g_localTrampoline.EndAlloc(code.getCurr());

		CreateRenderTarget_Original = (_CreateRenderTarget)codeBuf;

		g_branchTrampoline.Write6Branch(CreateRenderTarget.GetUIntPtr(), (uintptr_t)CreateRenderTarget_Hook);
	}

	g_branchTrampoline.Write5Call(UpdateEquipment_Enter, GetFnAddr(&ActorProcessManager::UpdateEquipment_Hooked));
}
