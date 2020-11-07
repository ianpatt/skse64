#pragma once

#include "skse64_common/Relocation.h"

class GFxMovieView;
class FxResponseArgsBase;
class ScaleformAllocator;

void * ScaleformHeap_Allocate(UInt32 size);
void ScaleformHeap_Free(void * ptr);

typedef void * (* _InvokeFunction)(GFxMovieView * movie, const char * fnName, FxResponseArgsBase * arguments);
extern RelocAddr<_InvokeFunction> InvokeFunction;

struct HeapDesc
{
	enum
	{
		kHeap_ThreadUnsafe		= (1 << 0),
		kHeap_FastTinyBlocks	= (1 << 1),
		kHeap_FixedGranularity	= (1 << 2),
		kHeap_Root				= (1 << 3),
		kHeap_NoDebugInfo		= (1 << 4),
		kHeap_UserDebug			= (1 << 12)
	};
	UInt32	flags;
	UInt32	granularity;
	UInt32	reserve;
	UInt32	threshold;
	UInt32	limit;
	UInt32	heapId;
	UInt32	arena;
};

typedef void * (* _GFxAllocateHeap)(HeapDesc * heap, ScaleformAllocator * allocator);
extern RelocAddr<_GFxAllocateHeap> GFxAllocateHeap;
