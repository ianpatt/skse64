#include "ScaleformTypes.h"
#include "ScaleformAPI.h"

GString::Data * GString::GetData(void)
{
	return (Data *)(data.heapInfo & ~kHeapInfoMask);
}

UInt32 GString::GetHeapInfo(void)
{
	return data.heapInfo & kHeapInfoMask;
}

void GString::Destroy(void)
{
	GString::Data	* data = GetData();

	data->Release();
}

void GString::Data::IncRef(void)
{
	InterlockedExchangeAdd(&refCount, 1);
}

void GString::Data::Release(void)
{
	SInt32	oldRefCount = InterlockedExchangeAdd(&refCount, -1);	// decref

	// all references gone?
	if(oldRefCount == 1)
	{
		ScaleformHeap_Free(this);
	}
}
