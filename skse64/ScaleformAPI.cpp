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

// C70DB2D0DA8EB136C1BB8E87A7E39C173A7E4C0B+21
RelocPtr<ScaleformHeap *> g_scaleformHeap(0x03032C50);

void * ScaleformHeap_Allocate(UInt32 size)
{
	return (*(g_scaleformHeap))->Allocate(size);
}

void ScaleformHeap_Free(void * ptr)
{
	(*(g_scaleformHeap))->Free(ptr);
}

RelocAddr<_InvokeFunction> InvokeFunction(0x00ED6AC0);
RelocAddr<_GFxAllocateHeap> GFxAllocateHeap(0x00F483C0);
