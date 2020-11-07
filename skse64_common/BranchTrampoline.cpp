#include "BranchTrampoline.h"
#include "SafeWrite.h"
#include <climits>

BranchTrampoline g_branchTrampoline;
BranchTrampoline g_localTrampoline;

BranchTrampoline::BranchTrampoline()
	:m_base(nullptr)
	, m_len(0)
	, m_allocated(0)
	, m_curAlloc(nullptr)
{
	//
}

BranchTrampoline::~BranchTrampoline()
{
	Destroy();
}

bool BranchTrampoline::Create(size_t len, void * module)
{
	if (!module) module = GetModuleHandle(NULL);

	// search backwards from module base
	uintptr_t moduleBase = uintptr_t(module);
	uintptr_t addr = moduleBase;
	uintptr_t lowestOKAddress = moduleBase - 0x80000000 + (1024 * 1024 * 128);	// largest 32-bit displacement with 128MB scratch space
	addr--;

	while (!m_base)
	{
		MEMORY_BASIC_INFORMATION info;

		if (!VirtualQuery((void *)addr, &info, sizeof(info)))
		{
			_ERROR("VirtualQuery failed: %08X", GetLastError());
			break;
		}

		if (info.State == MEM_FREE)
		{
			// free block, big enough?
			if (info.RegionSize >= len)
			{
				// try to allocate it
				addr = ((uintptr_t)info.BaseAddress) + info.RegionSize - len;

				m_base = (void *)VirtualAlloc((void *)addr, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
				if (m_base)
				{
					m_len = len;
					m_allocated = 0;
				}
				else
				{
					_WARNING("trampoline alloc %016I64Xx%016I64X failed (%08X)", addr, len, GetLastError());
				}
			}
		}

		// move back and try again
		if (!m_base)
		{
			addr = ((uintptr_t)info.BaseAddress) - 1;
		}

		if (addr < lowestOKAddress)
		{
			_ERROR("couldn't allocate trampoline, no free space before image");
			break;
		}
	}

	return m_base != nullptr;
}

void BranchTrampoline::Destroy()
{
	if (m_base)
	{
		VirtualFree(m_base, 0, MEM_RELEASE);
		m_base = nullptr;
	}
}

void BranchTrampoline::SetBase(size_t len, void * base)
{
	ASSERT(!m_base);
	m_base = base;
	m_len = len;
	m_allocated = 0;
	m_curAlloc = nullptr;
}

void * BranchTrampoline::StartAlloc()
{
	ASSERT(m_base);
	ASSERT(!m_curAlloc);

	m_curAlloc = ((UInt8 *)m_base) + m_allocated;

	return m_curAlloc;
}

void BranchTrampoline::EndAlloc(const void * end)
{
	ASSERT(m_base);
	ASSERT(m_curAlloc);

	size_t len = uintptr_t(end) - uintptr_t(m_curAlloc);
	ASSERT(len <= Remain());

	m_allocated += len;
	m_curAlloc = nullptr;
}

void * BranchTrampoline::Allocate(size_t size)
{
	ASSERT(m_base);

	void * result = nullptr;

	if (size <= Remain())
	{
		result = ((UInt8 *)m_base) + m_allocated;
		m_allocated += size;
	}

	return result;
}

bool BranchTrampoline::Write6Branch(uintptr_t src, uintptr_t dst)
{
	return Write6Branch_Internal(src, dst, 0x25);
}

bool BranchTrampoline::Write6Call(uintptr_t src, uintptr_t dst)
{
	return Write6Branch_Internal(src, dst, 0x15);
}

bool BranchTrampoline::Write5Branch(uintptr_t src, uintptr_t dst)
{
	return Write5Branch_Internal(src, dst, 0xE9);
}

bool BranchTrampoline::Write5Call(uintptr_t src, uintptr_t dst)
{
	return Write5Branch_Internal(src, dst, 0xE8);
}

bool BranchTrampoline::Write6Branch_Internal(uintptr_t src, uintptr_t dst, UInt8 op)
{
	bool result = false;

	uintptr_t * trampoline = (uintptr_t *)Allocate();
	if (trampoline)
	{
		uintptr_t	trampolineAddr = (uintptr_t)trampoline;
		uintptr_t	nextInstr = src + 6;
		ptrdiff_t	trampolineDispl = trampolineAddr - nextInstr;

		if ((trampolineDispl >= _I32_MIN) && (trampolineDispl <= _I32_MAX))
		{
			UInt8	code[6];

			// jmp [rip+imm32]
			code[0] = 0xFF;
			code[1] = op;
			*((SInt32 *)&code[2]) = (SInt32)trampolineDispl;

			SafeWriteBuf(src, code, sizeof(code));

			*trampoline = dst;

			result = true;
		}
	}

	// do this for now so it's obvious when something goes wrong
	ASSERT(result);

	return result;
}

bool BranchTrampoline::Write5Branch_Internal(uintptr_t src, uintptr_t dst, UInt8 op)
{
	bool result = false;

#pragma pack(push, 1)
	// code placed in trampoline
	struct TrampolineCode
	{
		// jmp [rip]
		UInt8	escape;	// FF
		UInt8	modrm;	// 25
		UInt32	displ;	// 00000000
		// rip points here
		UInt64	dst;	// target

		void Init(uintptr_t _dst)
		{
			escape = 0xFF;
			modrm = 0x25;
			displ = 0;
			dst = _dst;
		}
	};

	struct HookCode
	{
		// jmp disp32
		UInt8	op;		// E9 for jmp, E8 for call
		SInt32	displ;	// 

		void Init(SInt32 _displ, UInt8 _op)
		{
			op = _op;
			displ = _displ;
		}
	};
#pragma pack(pop)

	STATIC_ASSERT(sizeof(TrampolineCode) == 14);
	STATIC_ASSERT(sizeof(HookCode) == 5);

	TrampolineCode * trampolineCode = (TrampolineCode *)Allocate(sizeof(TrampolineCode));
	if (trampolineCode)
	{
		trampolineCode->Init(dst);

		HookCode hookCode;

		uintptr_t	trampolineAddr = uintptr_t(trampolineCode);
		uintptr_t	nextInstr = src + sizeof(hookCode);
		ptrdiff_t	trampolineDispl = trampolineAddr - nextInstr;

		// should never fail because we're branching in to the trampoline
		ASSERT((trampolineDispl >= _I32_MIN) && (trampolineDispl <= _I32_MAX));

		hookCode.Init(trampolineDispl, op);

		SafeWriteBuf(src, &hookCode, sizeof(hookCode));

		result = true;
	}

	// do this for now so it's obvious when something goes wrong
	ASSERT(result);

	return result;
}
