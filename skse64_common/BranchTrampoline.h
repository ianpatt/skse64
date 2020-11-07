#pragma once

class BranchTrampoline
{
public:
	BranchTrampoline();
	~BranchTrampoline();

	bool Create(size_t len, void * module = NULL);
	void Destroy();
	void SetBase(size_t len, void* base);

	// allocate unsized
	void * StartAlloc();
	void EndAlloc(const void * end);

	void * Allocate(size_t size = sizeof(void *));

	size_t Remain() { return m_len - m_allocated; }

	// takes 6 bytes of space at src, 8 bytes in trampoline
	bool Write6Branch(uintptr_t src, uintptr_t dst);
	bool Write6Call(uintptr_t src, uintptr_t dst);

	// takes 5 bytes of space at src, 14 bytes in trampoline
	bool Write5Branch(uintptr_t src, uintptr_t dst);
	bool Write5Call(uintptr_t src, uintptr_t dst);

private:
	// takes 6 bytes of space at src, 8 bytes in trampoline
	bool Write6Branch_Internal(uintptr_t src, uintptr_t dst, UInt8 op);

	// takes 5 bytes of space at src, 14 bytes in trampoline
	bool Write5Branch_Internal(uintptr_t src, uintptr_t dst, UInt8 op);

	void	* m_base;
	size_t	m_len;			// bytes
	size_t	m_allocated;	// bytes

	void	* m_curAlloc;	// currently active StartAlloc base
};

extern BranchTrampoline g_branchTrampoline;
extern BranchTrampoline g_localTrampoline;
