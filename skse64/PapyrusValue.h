#pragma once

#include "skse64_common/Utilities.h"
#include "skse64/GameTypes.h"

class VMClassInfo;

// ??
class VMIdentifier
{
public:
	enum
	{
		kLockBit = 0x80000000,
		kFastSpinThreshold = 10000
	};

	SInt32			m_refCount;	// 00
	UInt32			unk04;		// 04
	VMClassInfo		* m_type;	// 08
	void			* unk08;	// 10
	void			* unk0C;	// 18
	volatile UInt64	m_handle;	// 20
	volatile SInt32	m_lock;		// 28

	UInt64	GetHandle(void);

	SInt32	Lock(void);
	void	Unlock(SInt32 oldLock);

	// lock and refcount?
	void	IncrementLock(void);
	SInt32	DecrementLock(void);

	void	Destroy(void);

	MEMBER_FN_PREFIX(VMIdentifier);
	DEFINE_MEMBER_FN(Destroy_Internal, void, 0x01233670);
};

// 10
class VMValue
{
public:
	VMValue() :type(kType_None) { data.p = 0; }
	~VMValue() { CALL_MEMBER_FN(this, Destroy)(); }

	VMValue(const VMValue & other)
	{
		if (&other != this)
		{
			type = kType_None;
			data.p = nullptr;
			CALL_MEMBER_FN(this, Set)(&other);
		}
	}
	VMValue& operator=(const VMValue& other)
	{
		if (&other == this)
			return *this;

		CALL_MEMBER_FN(this, Set)(&other);
		return *this;
	}

	enum
	{
		kType_None = 0,
		kType_Identifier = 1,
		kType_String = 2,
		kType_Int = 3,
		kType_Float = 4,
		kType_Bool = 5,

		kType_Unk0B = 0x0B,
		kType_ArraysStart = 11,
		kType_StringArray = 12,
		kType_IntArray = 13,
		kType_FloatArray = 14,
		kType_BoolArray = 15,
		kType_ArraysEnd = 16,

		kNumLiteralArrays = 4
	};

	// 18+
	struct ArrayData
	{
		volatile SInt32	refCount;	// 00
		UInt32			unk04;		// 04
		UInt64			unk08;		// 08
		UInt32			len;		// 10
		UInt32			unk14;		// 14
		UInt64			unk18;		// 18
		//VMValue			data[0];	// 20

		VMValue	*	GetData(void) { return (VMValue *)(this + 1); }

		MEMBER_FN_PREFIX(ArrayData);
		DEFINE_MEMBER_FN(Destroy, void, 0x01241E40);
	};

	UInt64	type;	// 00

	union
	{
		SInt32			i;
		UInt32			u;
		float			f;
		bool			b;
		void			* p;
		ArrayData		* arr;
		VMIdentifier	* id;
		const char		* str;	// BSFixedString

		BSFixedString *	GetStr(void) { return (BSFixedString *)(&str); }
	} data;			// 04

	MEMBER_FN_PREFIX(VMValue);
	DEFINE_MEMBER_FN(Set, void, 0x01236E50, const VMValue * src);
	DEFINE_MEMBER_FN(Destroy, void, 0x01236D10);
	DEFINE_MEMBER_FN(SetArray, void, 0x01235F60, ArrayData * data);

	bool	IsIdentifierArray()
	{
		return (type >= kType_ArraysEnd && type & kType_Identifier);
	}

	bool	IsLiteralArray()
	{
		return type - kType_ArraysStart <= kNumLiteralArrays;
	}

	bool	IsArray()
	{
		return IsLiteralArray() || IsIdentifierArray();
	}

	void	SetNone(void)
	{
		CALL_MEMBER_FN(this, Destroy)();

		type = kType_None;
		data.u = 0;
	}

	void	SetInt(SInt32 i)
	{
		CALL_MEMBER_FN(this, Destroy)();

		type = kType_Int;
		data.i = i;
	}

	void	SetFloat(float f)
	{
		CALL_MEMBER_FN(this, Destroy)();

		type = kType_Float;
		data.f = f;
	}

	void	SetBool(bool b)
	{
		CALL_MEMBER_FN(this, Destroy)();

		type = kType_Bool;
		data.b = b;
	}

	void	SetIdentifier(VMClassInfo * classInfo)
	{
		CALL_MEMBER_FN(this, Destroy)();

		type = (UInt64)classInfo;
		data.id = NULL;
	}

	void	SetIdentifier(VMIdentifier ** identifier)
	{
		if (GetUnmangledType() == kType_Identifier)
		{
			CALL_MEMBER_FN(this, Destroy)();

			if (*identifier)
				(*identifier)->IncrementLock();

			data.id = *identifier;
		}
	}

	void	SetString(const char * str)
	{
		CALL_MEMBER_FN(this, Destroy)();

		type = kType_String;
		CALL_MEMBER_FN(data.GetStr(), Set)(str);
	}

	// 00-0F are untouched
	// 10+ alternate between 0x01 and 0x0B
	UInt32	GetUnmangledType(void);

	bool	IsIdentifier(void) { return GetUnmangledType() == kType_Identifier; }
};
