#pragma once

// 4
template <typename T>
class NiPointer
{
public:
	T	* m_pObject;	// 00

	inline NiPointer(const NiPointer& rhs) :
		m_pObject(rhs.m_pObject)
	{
		if (m_pObject) {
			m_pObject->IncRef();
		}
	}

	inline NiPointer(T* pObject = (T*) 0)
	{
		m_pObject = pObject;
		if(m_pObject) m_pObject->IncRef();
	}

	inline ~NiPointer()
	{
		if(m_pObject) m_pObject->DecRef();
	}

	inline operator T *() const
	{
		return m_pObject;
	}

	inline T & operator*() const
	{
		return *m_pObject;
	}

	inline T * operator->() const
	{
		return m_pObject;
	}

	inline NiPointer <T> & operator=(const NiPointer & rhs)
	{
		if(m_pObject != rhs.m_pObject)
		{
			if(rhs) rhs.m_pObject->IncRef();
			if(m_pObject) m_pObject->DecRef();

			m_pObject = rhs.m_pObject;
		}

		return *this;
	}

	inline NiPointer <T> & operator=(T * rhs)
	{
		if(m_pObject != rhs)
		{
			if(rhs) rhs->IncRef();
			if(m_pObject) m_pObject->DecRef();

			m_pObject = rhs;
		}

		return *this;
	}

	inline bool operator==(T * pObject) const
	{
		return m_pObject == pObject;
	}

	inline bool operator!=(T * pObject) const
	{
		return m_pObject != pObject;
	}

	inline bool operator==(const NiPointer & ptr) const
	{
		return m_pObject == ptr.m_pObject;
	}

	inline bool operator!=(const NiPointer & ptr) const
	{
		return m_pObject != ptr.m_pObject;
	}

	inline T* get()
	{
		return m_pObject;
	}
};

#define MAKE_NI_POINTER(x)	class x; typedef NiPointer <x> x##Ptr

template <class T_to, class T_from>
T_to * niptr_cast(const T_from & src)
{
	return static_cast <T_to *>(src.m_pObject);
}

// 10
template <class T>
class NiRect
{
public:
	T	m_left;		// 00
	T	m_right;	// 04
	T	m_top;		// 08
	T	m_bottom;	// 0C
};

// 1C
class NiFrustum
{
public:
	float	m_fLeft;	// 00
	float	m_fRight;	// 04
	float	m_fTop;		// 08
	float	m_fBottom;	// 0C
	float	m_fNear;	// 10
	float	m_fFar;		// 14
	bool	m_bOrtho;	// 18
};

// 10
class NiQuaternion
{
public:
	// w is first

	float	m_fW;	// 0
	float	m_fX;	// 4
	float	m_fY;	// 8
	float	m_fZ;	// C
};

// 8
class NiPoint2
{
public:
	float	x;	// 0
	float	y;	// 4
};

// C
class NiPoint3
{
public:
	float	x;	// 0
	float	y;	// 4
	float	z;	// 8

	NiPoint3();
	NiPoint3(float X, float Y, float Z) : x(X), y(Y), z(Z) { };

	// Negative
	NiPoint3 operator- () const;

	// Basic operations
	NiPoint3 operator+ (const NiPoint3& pt) const;
	NiPoint3 operator- (const NiPoint3& pt) const;

	NiPoint3& operator+= (const NiPoint3& pt);
	NiPoint3& operator-= (const NiPoint3& pt);

	// Scalar operations
	NiPoint3 operator* (float fScalar) const;
	NiPoint3 operator/ (float fScalar) const;

	NiPoint3& operator*= (float fScalar);
	NiPoint3& operator/= (float fScalar);
};

// 0C
class NiColor
{
public:
	float	r;	// 0
	float	g;	// 4
	float	b;	// 8
};

// 10
class NiColorA
{
public:
	float	r;	// 0
	float	g;	// 4
	float	b;	// 8
	float	a;	// C
};

// math.h
#define MATH_PI       3.14159265358979323846

// 24
class NiMatrix33
{
public:
	union
	{
		float	data[3][3];
		float   arr[9];
	};

	void Identity();

	// Addition/Subtraction
	NiMatrix33 operator+(const NiMatrix33& mat) const;
	NiMatrix33 operator-(const NiMatrix33& mat) const;

	// Matric mult
	NiMatrix33 operator*(const NiMatrix33& mat) const;

	// Vector mult
	NiPoint3 operator*(const NiPoint3& pt) const;

	// Scalar multiplier
	NiMatrix33 operator*(float fScalar) const;

	NiMatrix33 Transpose() const;

	void GetEulerAngles(float * heading, float * attitude, float * bank);
	void SetEulerAngles(float heading, float attitude, float bank);
};

STATIC_ASSERT(sizeof(NiMatrix33) == 0x24);

// 34
class NiTransform
{
public:
	NiMatrix33	rot;	// 00
	NiPoint3	pos;	// 24
	float		scale;	// 30

	NiTransform();

	// Multiply transforms
	NiTransform operator*(const NiTransform &rhs) const;

	// Transform point
	NiPoint3 operator*(const NiPoint3 &pt) const;

	// Invert
	void Invert(NiTransform& kDest) const;
};

STATIC_ASSERT(sizeof(NiTransform) == 0x34);

// 10
class NiBound
{
public:
	NiPoint3	pos;
	float		radius;
};

STATIC_ASSERT(sizeof(NiBound) == 0x10);

// 20
// derives from NiTMapBase, we don't bother
template <typename T_key, typename T_data>
class NiTMap
{
public:
	virtual ~NiTMap();

	struct NiTMapItem
	{
		NiTMapItem	* next;
		T_key		key;
		T_data		data;
	};

	T_data	Get(T_key key)
	{
		UInt32	bucket = GetBucket(key);

		for(NiTMapItem * iter = buckets[bucket]; iter; iter = iter->next)
		{
			if(Compare(iter->key, key))
			{
				return iter->data;
			}
		}

		return T_data();
	}

	virtual UInt32	GetBucket(T_key key);					// return hash % numBuckets;
	virtual bool	Compare(T_key lhs, T_key rhs);			// return lhs == rhs;
	virtual void	FillItem(NiTMapItem * item, T_key key, T_data data);
															// item->key = key; item->data = data;
	virtual void	Fn_04(UInt32 arg);	// nop
	virtual NiTMapItem *	AllocItem(void);				// return new NiTMapItem;
	virtual void	FreeItem(NiTMapItem * item);			// item->data = 0; delete item;

//	void		** _vtbl;	// 00
	UInt32		numBuckets;	// 08
	UInt32		unk0C;		// 0C
	NiTMapItem	** buckets;	// 10
	UInt32		numEntries;	// 18
	UInt32		unk1C;		// 1C
};

// 10
template <typename T_key, typename T_data>
class NiTPointerMap : NiTMap <T_key, T_data>
{
public:
};

// 18
template <typename T>
class NiTArray
{
public:
	NiTArray();
	virtual ~NiTArray();

	// sparse array, can have NULL entries that should be skipped
	// iterate from 0 to m_emptyRunStart - 1

//	void	** _vtbl;			// 00
	T		* m_data;			// 08
	UInt16	m_arrayBufLen;		// 10 - max elements storable in m_data
	UInt16	m_emptyRunStart;	// 12 - index of beginning of empty slot run
	UInt16	m_size;				// 14 - number of filled slots
	UInt16	m_growSize;			// 16 - number of slots to grow m_data by
};
STATIC_ASSERT(sizeof(NiTArray <void *>) == 0x18);

// 20
template <typename T>
class NiTLargeArray
{
public:
	NiTLargeArray();
	virtual ~NiTLargeArray();

	// fast, doesn't search for empty slots
	void NiTLargeArray::Append(T item)
	{
		if(m_emptyRunStart == m_arrayBufLen)
		{
			// need to expand the array
			Resize(m_arrayBufLen + 1);
		}

		m_data[m_emptyRunStart] = item;
		m_emptyRunStart++;
		m_size++;
	}

	void NiTLargeArray::Resize(UInt32 size)
	{
		// not reclaiming memory yet
		if(size <= m_size) return;

		ASSERT(m_growSize);

		// obey min grow size
		UInt32	growSize = size - m_size;
		if(growSize < m_growSize)
			growSize = m_growSize;

		size = m_arrayBufLen + growSize;

		// create new array
		T	* newData = (T *)FormHeap_Allocate(sizeof(T) * size);

		for(UInt32 i = 0; i < size; i++)
		{
			new (&newData[i]) T;
			newData[i] = 0;
		}

		// copy over data, compacting as we go
		UInt32	iter = 0;

		for(UInt32 i = 0; i < m_emptyRunStart; i++)
		{
			if(m_data[i])
			{
				newData[iter] = m_data[i];
				iter++;
			}
		}

		// update pointers
		T		* oldData = m_data;
		UInt32	oldDataLen = m_emptyRunStart;

		m_data = newData;
		m_arrayBufLen = size;
		m_emptyRunStart = m_size;

		// delete old array
		if(oldData)
		{
			for(UInt32 i = 0; i < oldDataLen; i++)
				if(oldData[i])
					oldData[i].~T();

			FormHeap_Free(oldData);
		}
	}

//	void	** _vtbl;			// 00
	T		* m_data;			// 08
	UInt32	m_arrayBufLen;		// 10 - max elements storable in m_data
	UInt32	m_emptyRunStart;	// 14 - index of beginning of empty slot run
	UInt32	m_size;				// 18 - number of filled slots
	UInt32	m_growSize;			// 1C - number of slots to grow m_data by
};
