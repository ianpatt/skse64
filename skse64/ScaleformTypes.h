#pragma once

#include "skse64_common/Utilities.h"
#include "skse64/ScaleformAPI.h"

// not sure why they nest it like this, but whatever
class GRefCountImplCore
{
public:
	GRefCountImplCore() :refCount(1) { }
	virtual ~GRefCountImplCore() { }

//	void	** _vtbl;			// 00
	volatile SInt32	refCount;	// 04

	// redirect new/delete to the scaleform heap
	static void * operator new(std::size_t size)
	{
		return ScaleformHeap_Allocate(size);
	}

	static void * operator new(std::size_t size, const std::nothrow_t &)
	{
		return ScaleformHeap_Allocate(size);
	}

	// placement new
	static void * operator new(std::size_t size, void * ptr)
	{
		return ptr;
	}

	static void operator delete(void * ptr)
	{
		ScaleformHeap_Free(ptr);
	}

	// placement delete
	static void operator delete(void *, void *)
	{
		//
	}

	void	AddRef(void)
	{
		InterlockedIncrement(&refCount);
	}

	void	Release(void)
	{
		if(InterlockedDecrement(&refCount) == 0)
		{
			delete this;
		}
	}
};

class GRefCountImpl : public GRefCountImplCore
{
public:
	GRefCountImpl() { }
	virtual ~GRefCountImpl() { }
};

class GRefCountBaseStatImpl : public GRefCountImpl
{
public:
	GRefCountBaseStatImpl() { }
	virtual ~GRefCountBaseStatImpl() { }
};

class GRefCountBase : public GRefCountBaseStatImpl
{
public:
	GRefCountBase() { }
	virtual ~GRefCountBase() { }
};

#pragma warning (push)
#pragma warning (disable : 4200)

class GString
{
public:
	GString()	{ }
	GString(const char * string)	{ CALL_MEMBER_FN(this, ctor)(string); }
	~GString()	{ }

	enum
	{
		kHeapInfoMask =	3,
	};

	struct Data
	{
		UInt32	len;
		SInt32	refCount;
		char	data[0];

		void	IncRef(void);
		void	Release(void);
	};

	union
	{
		Data	* ptr;		// do not use directly, clear lower 3 bits first
		UInt32	heapInfo;
	} data;

	Data *	GetData(void);
	UInt32	GetHeapInfo(void);

	void	Destroy(void);

	MEMBER_FN_PREFIX(GString);
	DEFINE_MEMBER_FN(ctor, GString *, 0x00ED3A50, const char * string);
	
};

#pragma warning (pop)

template <typename T>
class GRect
{
public:
	T	left;
	T	top;
	T	right;
	T	bottom;
};

class GMatrix3D
{
public:
	float	m[4][4];
};

typedef GRect<float>	GRectF;

template <typename T>
class GArray
{
public:
	T * values;			// 00
	UInt64	size;		// 08
	UInt64	capacity;	// 10
};

class GViewport
{
public:
	enum
	{
		kFlags_IsRenderTexture    = 1 << 0,
		kFlags_AlphaComposite     = 1 << 1,
		kFlags_UseScissorRect     = 1 << 2,
		kFlags_NoSetState         = 1 << 3,
		kFlags_RenderTextureAlpha = kFlags_IsRenderTexture|kFlags_AlphaComposite
	};

	SInt32	bufferWidth;
	SInt32	bufferHeight;
	SInt32	left;
	SInt32	top;
	SInt32	width;
	SInt32	height;
	SInt32	scissorLeft;
	SInt32	scissorTop;
	SInt32	scissorWidth;
	SInt32	scissorHeight;
	float	scale;
	float	aspectRatio;
	UInt32	flags;
};

class GSysAllocBase
{
public:
	virtual ~GSysAllocBase();

	virtual void Unk_01(void);
	virtual void Unk_02(void);
};

class GSysAlloc : public GSysAllocBase
{
public:
	virtual ~GSysAlloc();
};

class ScaleformAllocator : public GSysAlloc
{
public:
	virtual ~ScaleformAllocator();

	virtual void Unk_03(void);
	virtual void Unk_04(void);
};
