#pragma once

#include "skse64_common/Utilities.h"
#include "skse64/GameAPI.h"

class TESForm;

// 08
struct BSIntrusiveRefCounted
{
public:
	volatile UInt32	m_refCount;	// 00
	UInt32			unk04;		// 04
};

// 04
template <typename T>
class BSTSmartPointer
{
public:
	// refcounted
	T	* ptr;
};


//// 04
//struct BSNonReentrantSpinLock
//{
//public:
//	volatile UInt32	uiLock;		// 00
//};
//

// 08
class SimpleLock
{
	enum
	{
		kFastSpinThreshold = 10000
	};

	volatile SInt32	threadID;	// 00
	volatile UInt32	lockCount;	// 04

public:
	SimpleLock() : threadID(0), lockCount(0) {}

	void Lock(UInt32 pauseAttempts = 0);
	void Release(void);
};
STATIC_ASSERT(sizeof(SimpleLock) == 0x8);

class SimpleLocker
{
public:
	SimpleLocker(SimpleLock * dataHolder) { m_lock = dataHolder; m_lock->Lock(); }
	~SimpleLocker() { m_lock->Release(); }

protected:
	SimpleLock	* m_lock;
};

// 08
class BSReadWriteLock
{
	enum
	{
		kFastSpinThreshold = 10000,
		kLockWrite = 0x80000000,
		kLockCountMask = 0xFFFFFFF
	};

	volatile SInt32	threadID;	// 00
	volatile UInt32	lockValue;	// 04

public:
	BSReadWriteLock() : threadID(0), lockValue(0) {}

	//void LockForRead();
	//void LockForWrite();
	MEMBER_FN_PREFIX(BSReadWriteLock);
	DEFINE_MEMBER_FN(LockForRead, void, 0x00C072D0);
	DEFINE_MEMBER_FN(LockForWrite, void, 0x00C07350);
	DEFINE_MEMBER_FN(UnlockRead, void, 0x00C07590);
	DEFINE_MEMBER_FN(UnlockWrite, void, 0x00C075A0);
	DEFINE_MEMBER_FN(LockForReadAndWrite, void, 0x00C07450);
	DEFINE_MEMBER_FN(TryLockForWrite, bool, 0x00C07540);
};
STATIC_ASSERT(sizeof(BSReadWriteLock) == 0x8);

class BSReadLocker
{
public:
	BSReadLocker(BSReadWriteLock * lock) { m_lock = lock; CALL_MEMBER_FN(m_lock, LockForRead)(); }
	~BSReadLocker() { CALL_MEMBER_FN(m_lock, UnlockRead)(); }

protected:
	BSReadWriteLock    * m_lock;
};

class BSWriteLocker
{
public:
	BSWriteLocker(BSReadWriteLock * lock) { m_lock = lock; CALL_MEMBER_FN(m_lock, LockForWrite)(); }
	~BSWriteLocker() { CALL_MEMBER_FN(m_lock, UnlockWrite)(); }

protected:
	BSReadWriteLock    * m_lock;
};

class BSReadAndWriteLocker
{
public:
	BSReadAndWriteLocker(BSReadWriteLock * lock) { m_lock = lock; CALL_MEMBER_FN(m_lock, LockForReadAndWrite)(); }
	~BSReadAndWriteLocker() { CALL_MEMBER_FN(m_lock, UnlockWrite)(); }

protected:
	BSReadWriteLock    * m_lock;
};

// 80808
class StringCache
{
public:
	struct Ref
	{
		const char	* data;

		MEMBER_FN_PREFIX(Ref);
		DEFINE_MEMBER_FN(ctor, Ref *, 0x00C28BF0, const char * buf);
		// E728381B6B25FD30DF9845889144E86E5CC35A25+38
		DEFINE_MEMBER_FN(ctor_ref, Ref *, 0x00C28C80, const Ref & rhs);
		DEFINE_MEMBER_FN(Set, Ref *, 0x00C28D60, const char * buf);
		// F3F05A02DE2034133B5965D694745B6369FC557D+F3
		DEFINE_MEMBER_FN(Set_ref, Ref *, 0x00C28E20, const Ref & rhs);
		// 77D2390F6DC57138CF0E5266EB5BBB0ACABDFBE3+A0
		DEFINE_MEMBER_FN(Release, void, 0x00C28D40);

		Ref();
		Ref(const char * buf);
		
		void Release() { CALL_MEMBER_FN(this, Release)(); }

		bool operator==(const Ref& lhs) const { return data == lhs.data; }
		bool operator<(const Ref& lhs) const { return data < lhs.data; }

		const char * c_str() const { return operator const char *(); }
		const char * Get() const { return c_str(); }
		operator const char *() const { return data ? data : ""; }
	};

	// 10
	struct Lock
	{
		UInt32	unk00;	// 00 - set to 80000000 when locked
		UInt32	pad04;	// 04
		UInt64	pad08;	// 08
	};

	void	* lut[0x10000];	// 00000
	Lock	lock[0x80];		// 80000
	UInt8	isInit;			// 80800
};

typedef StringCache::Ref BSFixedString;

//class BSAutoFixedString : public BSFixedString
//{
//public:
//	BSAutoFixedString() : BSFixedString() { }
//	BSAutoFixedString(const char * buf) : BSFixedString(buf) { }
//
//	~BSAutoFixedString()
//	{
//		Release();
//	}
//};

// 10
class BSString
{
public:
	BSString() :m_data(NULL), m_dataLen(0), m_bufLen(0) { }
	~BSString();

	const char *	Get(void) const;

	MEMBER_FN_PREFIX(BSString);
	DEFINE_MEMBER_FN(Set, bool, 0x000F9E90, const char * str, UInt32 len);	// len default 0

private:
	char	* m_data;	// 00
	UInt16	m_dataLen;	// 08
	UInt16	m_bufLen;	// 0A
	UInt32	pad0C;		// 0C
};


// Container types

// 18
template <class T, int nGrow = 10, int nShrink = 10>
class tArray
{
public:
	T* entries;			// 00
	UInt32 capacity;	// 08
	UInt32 pad0C;		// 0C
	UInt32 count;		// 10
	UInt32 pad14;		// 14

	tArray() : entries(NULL), capacity(0), count(0), pad0C(0), pad14(0) { }

	T& operator[](UInt64 index)
	{
		return entries[index];
	}

	void Clear()
	{
		Heap_Free(entries);
		entries = NULL;
		capacity = 0;
		count = 0;
	}

	bool Allocate(UInt32 numEntries)
	{
		entries = (T *)Heap_Allocate(sizeof(T) * numEntries);
		if(!entries) return false;

		for(UInt32 i = 0; i < numEntries; i++)
			new (&entries[i]) T;

		capacity = numEntries;
		count = numEntries;

		return true;
	}

	bool CopyFrom(const tArray<T> * rhs)
	{
		if (rhs->count == 0) return false;
		if (!rhs->entries) return false;
		
		if (entries)
			Clear();

		if (!Allocate(rhs->count)) return false;
		memcpy(entries, rhs->entries, sizeof(T) * count);
		return true;
	}



	bool Resize(UInt32 numEntries)
	{
		if(numEntries == capacity)
			return false;

		if(!entries) {
			Allocate(numEntries);
			return true;
		}
		if(numEntries < capacity) {
			// Delete the truncated entries
			for(UInt32 i = numEntries; i < capacity; i++)
				delete &entries[i];
		}
		
		T * newBlock = (T *)Heap_Allocate(sizeof(T) * numEntries);						// Create a new block
		memmove_s(newBlock, sizeof(T) * numEntries, entries, sizeof(T) * numEntries);	// Move the old memory to the new block
		if(numEntries > capacity) {														// Fill in new remaining entries
			for(UInt32 i = capacity; i < numEntries; i++)
				new (&entries[i]) T;
		}
		Heap_Free(entries);																// Free the old block
		entries = newBlock;																// Assign the new block
		capacity = numEntries;															// Capacity is now the number of total entries in the block
		count = min(capacity, count);													// Count stays the same, or is truncated to capacity
		return true;
	}

	bool Push(const T & entry)
	{
		if(!entries || count + 1 > capacity) {
			if(!Grow(nGrow))
				return false;
		}
 
		new (&entries[count]) T(entry);
		count++;
		return true;
	};
 
	bool Insert(UInt32 index, const T & entry)
	{
		if(!entries || index < count)
				return false;
 
		entries[index] = entry;
		return true;
	};

	bool Remove(UInt32 index)
	{
		if(!entries || index >= count)
			return false;
 
		// This might not be right for pointer types...
		(&entries[index])->~T();

		if(index + 1 < count) {
			UInt32 remaining = count - index;
			memmove_s(&entries[index + 1], sizeof(T) * remaining, &entries[index], sizeof(T) * remaining); // Move the rest up
		}
		count--;
 
		if(capacity > count + nShrink)
			Shrink();
 
		return true;
	}

	bool Shrink()
	{
		if(!entries || count == capacity) return false;
 
		try {
			UInt32 newSize = count;
			T * oldArray = entries;
			T * newArray = (T *)Heap_Allocate(sizeof(T) * newSize); // Allocate new block
			memmove_s(newArray, sizeof(T) * newSize, entries, sizeof(T) * newSize); // Move the old block
			entries = newArray;
			capacity = count;
			Heap_Free(oldArray); // Free the old block
			return true;
		}
		catch(...) {
			return false;
		}
 
		return false;
	}

	bool Grow(UInt32 numEntries)
	{
		if(!entries) {
			entries = (T *)Heap_Allocate(sizeof(T) * numEntries);
			count = 0;
			capacity = numEntries;
			return true;
		}
 
		try {
			UInt32 oldSize = capacity;
			UInt32 newSize = oldSize + numEntries;
			T * oldArray = entries;
			T * newArray = (T *)Heap_Allocate(sizeof(T) * newSize); // Allocate new block
			if(oldArray)
				memmove_s(newArray, sizeof(T) * newSize, entries, sizeof(T) * capacity); // Move the old block
			entries = newArray;
			capacity = newSize;

			if(oldArray)
				Heap_Free(oldArray); // Free the old block
 
			for(UInt32 i = oldSize; i < newSize; i++) // Allocate the rest of the free blocks
				new (&entries[i]) T;

			return true;
		}
		catch(...) {
			return false;
		}
 
		return false;
	}

	bool GetNthItem(UInt64 index, T& pT) const
	{
		if (index < count) {
			pT = entries[index];
			return true;
		}
		return false;
	}

	SInt64 GetItemIndex(T & pFind) const
	{
		for (UInt64 n = 0; n < count; n++) {
			T& pT = entries[n];
			if (pT == pFind)
				return n;
		}
		return -1;
	}

	DEFINE_STATIC_HEAP(Heap_Allocate, Heap_Free)
};

//template<class T>
//class tMutexArray : public tArray<T>
//{
//public:
//	SimpleLock lock;	// 18
//};

typedef tArray<UInt64> UnkArray;
typedef tArray<TESForm*> UnkFormArray;

// Returns if/where the element was found, otherwise indexOut can be used as insert position
template <typename T>
bool GetSortIndex(tArray<T> & arr, T & elem, SInt32 & indexOut)
{
	UInt32 count = arr.count;
	if (count == 0)
	{
		indexOut = 0;
		return false;
	}

	SInt32 leftIdx = 0;
	SInt32 rightIdx = count - 1;

	while (true)
	{
		UInt32 pivotIdx = leftIdx + ((rightIdx - leftIdx) / 2);

		T & p = arr[pivotIdx];

		if (elem == p)
		{
			indexOut = pivotIdx;
			return true;
		}
		else if (elem > p)
		{
			leftIdx = pivotIdx + 1;
		}
		else
		{
			rightIdx = pivotIdx - 1;
		}

		if (leftIdx > rightIdx)
		{
			indexOut = leftIdx;
			return false;
		}
	}
}

enum {
	eListCount = -3,
	eListEnd = -2,
	eListInvalid = -1,
};

// 10
template <class T>
class tList
{
	enum {
		eListCount = -3,
		eListEnd = -2,
		eListInvalid = -1,		
	};
	struct _Node
	{
		
		T *		item;	// 00
		_Node*	next;	// 04

		T *		Item() const { return item; }
		_Node*	Next() const { return next; }

		T *		Remove(_Node * pred)
		{
			T * pRemoved = item;
			_Node * pNext = next;

			// become the next entry and return my item
			if (pNext)
			{
				item = pNext->item;
				next = pNext->next;
				Heap_Free(pNext);
			}
			// tail?
			else
			{
				item = NULL;
				next = NULL;

				// has precedessor, so tail != head
				if (pred)
				{
					pred->next = NULL;
					Heap_Free(this);
				}
			}
			return pRemoved;
		}
	};

	_Node m_listHead;	// 00

private:

	_Node * Head() const { return const_cast<_Node*>(&m_listHead); }

	_Node * Tail() const
	{
		_Node * node = const_cast<_Node*>(&m_listHead);

		if (node)
			while (node->next) node = node->next;
		
		return node;
	}

	template <class Op>
	UInt32 FreeNodes(Op &compareOp)
	{
		UInt32 numFreed = 0;

		_Node * pPred = NULL;
		_Node * pCur = Head();

		while (pCur)
		{
			if (pCur->next)
			{
				if (compareOp.Accept(pCur->item))
				{
					pCur->Remove(pPred);
					numFreed++;
				}
				else
				{
					pPred = pCur;
					pCur = pCur->next;
				}
			}
			// List Tail?
			else
			{
				if (compareOp.Accept(pCur->item))
				{
					pCur->Remove(pPred);
					numFreed++;
				}
				break;
			}
		}

		return numFreed;
	}

	struct NodePos
	{
		NodePos(): node(NULL), index(eListInvalid) {}

		_Node*	node;
		SInt32	index;
	};


	NodePos GetNthNode(SInt32 index) const
	{
		NodePos pos;
		SInt32 n = 0;
		_Node* pCur = Head();

		while (pCur && pCur->Item())
		{
			if (n == index) break;
			if (eListEnd == index && !pCur->Next()) break;
			pCur = pCur->Next();
			++n;
		}

		pos.node = pCur;
		pos.index = n;

		return pos;
	}

public:

	// Allocate list as a single empty node
	static tList<T> * Create(void)
	{
		tList<T> * p = (tList<T> *)Heap_Allocate(sizeof(tList<T>));
		ASSERT(p);

		p->m_listHead.item = NULL;
		p->m_listHead.next = NULL;

		return p;
	}

	void Delete(void)
	{
		RemoveAll();
		Heap_Free(&m_listHead);
	}

	class Iterator
	{
		_Node *	m_cur;
	public:
		Iterator() : m_cur(NULL) {}
		Iterator(_Node* node) : m_cur(node) { }
		Iterator operator++()	{ if (!End()) m_cur = m_cur->Next(); return *this;}
		bool End()	{	return m_cur == NULL;	}
		const T * operator->() { return (m_cur) ? m_cur->Item() : NULL; }
		const T * operator*() { return (m_cur) ? m_cur->Item() : NULL; }
		const Iterator& operator=(const Iterator& rhs) {
			m_cur = rhs.m_cur;
			return *this;
		}
		T * Get() { return (m_cur) ? m_cur->Item() : NULL; }
	};
	
	const Iterator Begin() const { return Iterator(Head()); }

	void Insert(T * item)
	{
		// add new node if we aren't empty
		if (m_listHead.item)
		{
			// copy head in to new node
			_Node	* node = (_Node *)Heap_Allocate(sizeof(_Node));
			ASSERT(node);

			node->item = m_listHead.item;
			node->next = m_listHead.next;

			m_listHead.next = node;
		}

		m_listHead.item = item;
	}

	void Push(T * item)
	{
		_Node * tail = Tail();
		
		// add new node if we aren't empty
		if (tail->item)
		{
			_Node * node = (_Node *)Heap_Allocate(sizeof(_Node));
			ASSERT(node);

			tail->next = node;

			node->item = item;
			node->next = NULL;
		}
		else
		{
			tail->item = item;
		}
	}

	T * AddFront(void)
	{
		T	* item = (T *)Heap_Allocate(sizeof(T));
		if(!item)
			return NULL;

		new (item) T;

		Insert(item);
		return item;
	}

	T * AddBack(void)
	{
		T	* item = (T *)Heap_Allocate(sizeof(T));
		if(!item)
			return NULL;

		new (item) T;

		Push(item);
		return item;
	}

	void Append(Iterator source)
	{
		while (!source.End())
		{
			Push(source.Get());
			++source;
		}
	}

	UInt32 Count() const
	{
		NodePos pos = GetNthNode(eListCount);
		return (pos.index > 0) ? pos.index : 0;
	};

	T * GetNthItem(SInt32 n) const
	{
		NodePos pos = GetNthNode(n);
		return (pos.index == n && pos.node) ? pos.node->Item() : NULL;
	}

	T * GetLastItem() const
	{
		NodePos pos = GetNthNode(eListEnd);
		return pos.node->Item();
	}

	SInt32 AddAt(T * item, SInt32 index)
	{
		if (!m_listHead.item) {
			m_listHead.item = item;
			return 0;
		}

		NodePos pos = GetNthNode(index);
		_Node* pTargetNode = pos.node;
		_Node* newNode = (_Node*)Heap_Allocate(sizeof(_Node));
		if (newNode && pTargetNode) {
			if (index == eListEnd) {
				pTargetNode->next = newNode;
				newNode->item = item;
				newNode->next = NULL;
			} else {
				newNode->item = pTargetNode->item;
				newNode->next = pTargetNode->next;
				pTargetNode->item = item;
				pTargetNode->next = newNode;
			}
			return pos.index;
		}
		return eListInvalid;
	}

	template <class Op>
	void Visit(Op& op, _Node* prev = NULL) const {
		const _Node* pCur = (prev) ? prev->next : Head();
		bool bContinue = true;
		while (pCur && bContinue) {
			bContinue = op.Accept(pCur->Item());
			if (bContinue) {
				pCur = pCur->next;
			}
		}
	}

	template <class Op>
	T * Find(Op& op) const
	{
		const _Node* pCur = Head(); 

		bool bFound = false;
		while (pCur && !bFound)
		{
			if (!pCur->Item())
				pCur = pCur->Next();
			else
			{
				bFound = op.Accept(pCur->Item());
				if (!bFound)
					pCur = pCur->Next();
			}
		}
		return (bFound && pCur) ? pCur->Item() : NULL;
	}

	template <class Op>
	Iterator Find(Op& op, Iterator prev) const
	{
		Iterator curIt = (prev.End()) ? Begin() : ++prev;
		bool bFound = false;
		
		while(!curIt.End() && !bFound) {
			const T * pCur = *curIt;
			if (pCur) {
				bFound = op.Accept(pCur);
			}
			if (!bFound) {
				++curIt;
			}	
		}
		return curIt;
	}

	const _Node* FindString(char* str, Iterator prev) const
	{
		return Find(StringFinder_CI(str), prev);
	}

	template <class Op>
	UInt32 CountIf(Op& op) const
	{
		UInt32 count = 0;
		const _Node* pCur = Head();
		while (pCur)
		{
			if (pCur->Item() && op.Accept(pCur->Item()))
				count++;
			pCur = pCur->Next();
		}
		return count;
	}

	class AcceptAll {
	public:
		bool Accept(T * item) {
			return true;
		}
	};

	void RemoveAll()
	{
		FreeNodes(AcceptAll());
	}

	T * RemoveNth(SInt32 n) 
	{
		Item* pRemoved = NULL;
		if (n == 0) {
			pRemoved =  m_listHead.RemoveMe();
		} else if (n > 0) {
			NodePos nodePos = GetNthNode(n);
			if (nodePos.node && nodePos.index == n) {
				pRemoved = nodePos.node->RemoveMe();
			}
		}
		return pRemoved;
	};

	T * ReplaceNth(SInt32 n, T* item) 
	{
		Item* pReplaced = NULL;
		NodePos nodePos = GetNthNode(n);
		if (nodePos.node && nodePos.index == n) {
			pReplaced = nodePos.node->item;
			nodePos.node->item = item;
		}
		return pReplaced;
	}

	template <class Op>
	UInt32 RemoveIf(Op& op)
	{
		return FreeNodes(op);
	}

	template <class Op>
	SInt32 GetIndexOf(Op& op)
	{
		SInt32 idx = 0;
		const _Node* pCur = Head();
		while (pCur && pCur->Item() && !op.Accept(pCur->Item()))
		{
			idx++;
			pCur = pCur->Next();
		}

		if (pCur && pCur->Item())
			return idx;
		else
			return -1;
	}

	class AcceptEqual {
	public:
		T * item;

		AcceptEqual(T * a_item) : item(a_item) {}

		bool Accept(T * a_item) {
			return *item == *a_item;
		}
	};

	bool Contains(T * item) const
	{
		return Find(AcceptEqual(item)) != NULL;
	}
	
	void	Dump(void)
	{
		_MESSAGE("tList:");
		_MESSAGE("> count: %d", Count());

		const _Node* pCur = Head();
		UInt32 i = 0;
		while (pCur)
		{
			_MESSAGE("* %d :", i);
			//_MESSAGE("\t\titem: %08X", pCur->item);
			if (pCur->item)
				_MESSAGE("\t\t*item: %d", *pCur->item);
			_MESSAGE("\t\tnext: %08X", pCur->next);

			i++;
			pCur = pCur->next;
		}
	}
};

//
//STATIC_ASSERT(sizeof(tList<void *>) == 0x10);
//
// 30
template <typename Item, typename Key = Item>
class tHashSet
{
	class _Entry
	{
	public:
		Item	item;
		_Entry	* next;

		_Entry() : next(NULL) {}

		bool	IsFree() const	{ return next == NULL; }
		void	SetFree()		{ next = NULL; }

		void	Dump(void)
		{
			item.Dump();
			_MESSAGE("\t\tnext: %08X", next);
		}
	};

	// When creating a new tHashSet, init sentinel pointer with address of this entry
	static _Entry sentinel;

	void		* unk00;		// 000
	UInt32		unk_000;		// 008
	UInt32		m_size;			// 00C
	UInt32		m_freeCount;	// 010
	UInt32		m_freeOffset;	// 014
	_Entry 		* m_eolPtr;		// 018
	UInt64		unk_018;		// 020
	_Entry		* m_entries;	// 028


	_Entry * GetEntry(UInt32 hash) const
	{
		return (_Entry*) (((uintptr_t) m_entries) + sizeof(_Entry) * (hash & (m_size - 1)));
	}

	_Entry * GetEntryAt(UInt32 index) const
	{
		return (_Entry*) (((uintptr_t) m_entries) + sizeof(_Entry) * index);
	}

	_Entry * NextFreeEntry(void)
	{
		_Entry * result = NULL;

		if (m_freeCount == 0)
			return NULL;

		do
		{
			m_freeOffset = (m_size - 1) & (m_freeOffset - 1);
			_Entry * entry = GetEntryAt(m_freeOffset);

			if (entry->IsFree())
				result = entry;
		}
		while (!result);

		m_freeCount--;

		return result;
	}

	enum InsertResult
	{
		kInsert_Duplicate = -1,
		kInsert_OutOfSpace = 0,
		kInsert_Success = 1
	};

	InsertResult Insert(Item * item)
	{
		if (! m_entries)
			return kInsert_OutOfSpace;

		Key k = (Key)*item;
		_Entry * targetEntry = GetEntry(Item::GetHash(&k));

		// Case 1: Target entry is free
		if (targetEntry->IsFree())
		{
			targetEntry->item = *item;
			targetEntry->next = m_eolPtr;
			--m_freeCount;

			return kInsert_Success;
		}

		// -- Target entry is already in use

		// Case 2: Item already included
		_Entry * p = targetEntry;
		do
		{
			if (p->item == *item)
				return kInsert_Duplicate;
			p = p->next;
		}
		while (p != m_eolPtr);

		// -- Either hash collision or bucket overlap

		_Entry * freeEntry = NextFreeEntry();
		// No more space?
		if (!freeEntry)
			return kInsert_OutOfSpace;

		// Original position of the entry that currently uses the target position
		k = (Key)targetEntry->item;
		p = GetEntry(Item::GetHash(&k));

		// Case 3a: Hash collision - insert new entry between target entry and successor
        if (targetEntry == p)
        {
			freeEntry->item = *item;
			freeEntry->next = targetEntry->next;
			targetEntry->next = freeEntry;

			return kInsert_Success;
        }
		// Case 3b: Bucket overlap
		else
		{
			while (p->next != targetEntry)
				p = p->next;

			freeEntry->item = targetEntry->item;
			freeEntry->next = targetEntry->next;

			p->next = freeEntry;
			targetEntry->item = *item;
			targetEntry->next = m_eolPtr;

			return kInsert_Success;
		}
	}

	bool CopyEntry(_Entry * sourceEntry)
	{
		if (! m_entries)
			return false;

		Key k = (Key)sourceEntry->item;
		_Entry * targetEntry = GetEntry(Item::GetHash(&k));

		// Case 1: Target location is unused
		if (!targetEntry->next)
		{
			targetEntry->item = sourceEntry->item;
			targetEntry->next = m_eolPtr;
			--m_freeCount;

			return true;
		}

		// Target location is in use. Either hash collision or bucket overlap.

		_Entry * freeEntry = NextFreeEntry();
		k = (Key)targetEntry->item;
		_Entry * p = GetEntry(Item::GetHash(&k));

		// Case 2a: Hash collision - insert new entry between target entry and successor
		if (targetEntry == p)
		{
			freeEntry->item = sourceEntry->item;
			freeEntry->next = targetEntry->next;
			targetEntry->next = freeEntry;

			return true;
		}

		// Case 2b: Bucket overlap - forward until hash collision is found, then insert
		while (p->next != targetEntry)
			p = p->next;

		// Source entry takes position of target entry - not completely understood yet
		freeEntry->item = targetEntry->item;
		freeEntry->next = targetEntry->next;
		p->next = freeEntry;
		targetEntry->item = sourceEntry->item;
		targetEntry->next = m_eolPtr;

		return true;
	}

	void Grow(void)
	{
		UInt32 oldSize = m_size;
		UInt32 newSize = oldSize ? 2*oldSize : 8;

		_Entry * oldEntries = m_entries;
		_Entry * newEntries = (_Entry*)Heap_Allocate(newSize * sizeof(_Entry));
		ASSERT(newEntries);
		
		m_entries = newEntries;
		m_size = m_freeCount = m_freeOffset = newSize;

		// Initialize new table data (clear next pointers)
		_Entry * p = newEntries;
		for (UInt32 i = 0; i < newSize; i++, p++)
			p->SetFree();

		// Copy old entries, free old table data
		if (oldEntries)
		{
			_Entry * p = oldEntries;
			for (UInt32 i = 0; i < oldSize; i++, p++)
				if (p->next)
					CopyEntry(p);
			Heap_Free(oldEntries);
		}
	}

public:

	tHashSet() : m_size(0), m_freeCount(0), m_freeOffset(0), m_entries(NULL), m_eolPtr(&sentinel) { }

	UInt32	Size() const		{ return m_size; }
	UInt32	FreeCount() const	{ return m_freeCount; }
	UInt32	FillCount() const	{ return m_size - m_freeCount; }

	Item * Find(Key * key) const
	{
		if (!m_entries)
			return NULL;

		_Entry * entry = GetEntry(Item::GetHash(key));
		if (! entry->next)
			return NULL;

		while (!(entry->item == *key))
		{
			entry = entry->next;
			if (entry == m_eolPtr)
				return NULL;
		}

		return &entry->item;
	}

	bool Add(Item * item)
	{
		InsertResult result;

		for (result = Insert(item); result == kInsert_OutOfSpace; result = Insert(item))
			Grow();

		return result == kInsert_Success;
	}

	bool Remove(Key * key)
	{
		if ( !m_entries)
			return false;

		_Entry * entry = GetEntry(Item::GetHash(key));
		if (! entry->next)
			return NULL;

		_Entry * prevEntry = NULL;
		while (! (entry->item == *key))
		{
			prevEntry = entry;
			entry = entry->next;
			if (entry == m_eolPtr)
				return false;
		}

		// Remove tail?
		_Entry * nextEntry = entry->next;
		if (nextEntry == m_eolPtr)
		{
			if (prevEntry)
				prevEntry->next = m_eolPtr;
			entry->next = NULL;
		}
		else
		{
			entry->item = nextEntry->item;
			entry->next = nextEntry->next;
			nextEntry->next = NULL;
		}

		++m_freeCount;
		return true;
	}

	void Clear(void)
	{
		if (m_entries)
		{
			_Entry * p = m_entries;
			for (UInt32 i = 0; i < m_size; i++, p++)
				p->next = NULL;
		}
		else
		{
			m_size = 0;
		}
		m_freeCount = m_freeOffset = m_size;
	}

	template <typename T>
	void ForEach(T& functor)
	{
		if (!m_entries)
			return;

		if(m_size == m_freeCount) // The whole thing is free
			return;

		_Entry * cur	= m_entries;
		_Entry * end	= GetEntryAt(m_size); // one index beyond the entries data to check if we reached that point

		if (cur == end)
			return;

		if (cur->IsFree())
		{
			// Forward to first non-free entry
			do cur++;
			while (cur != end && cur->IsFree());
		}

		do
		{
			if (! functor(&cur->item))
				return;

			// Forward to next non-free entry
			do cur++;
			while (cur != end && cur->IsFree());

		} while (cur != end);
	}

	void Dump(void)
	{
		_MESSAGE("tHashSet:");
		_MESSAGE("> size: %d", Size());
		_MESSAGE("> free: %d", FreeCount());
		_MESSAGE("> filled: %d", FillCount());
		if (m_entries)
		{
			_Entry * p = m_entries;
			for (UInt32 i = 0; i < m_size; i++, p++) {
				_MESSAGE("* %d %s:", i, p->IsFree()?"(free)" : "");
				if (!p->IsFree())
					p->Dump();
			}
		}
	}

	DEFINE_STATIC_HEAP(Heap_Allocate, Heap_Free)
};

template <typename Key, typename Item>
typename tHashSet<Key,Item>::_Entry tHashSet<Key,Item>::sentinel = tHashSet<Key,Item>::_Entry();

// Don't know if this really is a native type or if sometimes locks are just placed before data structures.
template <typename T>
class SafeDataHolder
{
protected:
	mutable SimpleLock	m_lock;
public:
	T			m_data;

	void	Lock(void) { m_lock.Lock(); }
	void	Release(void) { m_lock.Release(); }

	template<typename T> friend class SafeDataLocker;
};

// 18
// Some generic type but I haven't figured it out yet
class UpdateRegistrationHolder
{
	// 018
	class Registration
	{
	public:
		volatile SInt32	refCount;	// 00
		bool			runOnce;	// 04
		UInt8			pad05;		// 05
		UInt16			pad06;		// 06
		UInt32			schedTime;	// 08
		UInt32			interval;	// 0C
		UInt64			handle;		// 10

		void Release(void)
		{
			if (InterlockedDecrement(&refCount) == 0) Heap_Free(this);
		}
	};

	tArray<Registration*>	m_regs;	// 00

	void	Order(UInt32 index);

public:
	bool	Remove(UInt64 & handle);
};
