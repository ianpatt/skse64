#include "ScaleformAPI.h"

class ScaleformHeap
{
public:
	virtual void	Fn_00(void);
	virtual void	Fn_01(void);
	virtual void	Fn_02(void);
	virtual void	Fn_03(void);
	virtual void	Fn_04(void);
	virtual void	Fn_05(void);
	virtual void	Fn_06(void);
	virtual void	Fn_07(void);
	virtual void	Fn_08(void);
	virtual void	Fn_09(void);
	virtual void *	Allocate(size_t size, UInt32 unk = 0);	// unk is probably align, maybe flags (haven't traced)
	virtual void	Fn_0B(void);
	virtual void	Free(void * ptr);
};

// ED9B05AE7833EE13354B2DFB8CD6E79144EE0D7E+3B
RelocPtr<ScaleformHeap *> g_scaleformHeap(0x030C6900);

void * ScaleformHeap_Allocate(UInt32 size)
{
	return (*(g_scaleformHeap))->Allocate(size);
}

void ScaleformHeap_Free(void * ptr)
{
	(*(g_scaleformHeap))->Free(ptr);
}

RelocAddr<_InvokeFunction> InvokeFunction(0x00F30640);
RelocAddr<_GFxAllocateHeap> GFxAllocateHeap(0x00FA2D20);
