#include "skse64/NiObjects.h"
#include "skse64/NiExtraData.h"

// A941556DD95C558F6FC490138ACEA2962E9A4F8D+2E
RelocPtr<float> g_worldToCamMatrix(0x02F4C910);
// A941556DD95C558F6FC490138ACEA2962E9A4F8D+B0
RelocPtr<NiRect<float>> g_viewPort(0x02F4DED0);

RelocAddr<_WorldPtToScreenPt3_Internal> WorldPtToScreenPt3_Internal(0x00C66580);

RelocAddr<_NiAllocate> NiAllocate(0x00C24750);
RelocAddr<_NiFree> NiFree(0x00C249B0);

void NiRefObject::IncRef(void)
{
	InterlockedIncrement(&m_uiRefCount);
}

bool NiRefObject::Release(void)
{
	return InterlockedDecrement(&m_uiRefCount) == 0;
}

void NiRefObject::DecRef(void)
{
	if (Release())
		DeleteThis();
}

int ExtraDataCompare(const void * ls, const void * rs)
{
	return (int)((*(NiExtraData**)ls)->m_pcName - (*(NiExtraData**)rs)->m_pcName);
}

void NiObjectNET::AddExtraData(NiExtraData * extraData)
{
	extraData->IncRef();

	// No capacity, allocate and grow
	UInt32 newSize = 0;
	if (m_extraDataCapacity == 0) {
		newSize = 1;
		NiExtraData ** extraDataList = (NiExtraData **)Heap_Allocate(newSize * sizeof(NiExtraData*));
		extraDataList[0] = extraData;
		m_extraData = extraDataList;
		m_extraDataCapacity = newSize;
		m_extraDataLen = m_extraDataCapacity;
		return;
	}

	// Reached capacity, reallocate and grow
	if (m_extraDataLen == m_extraDataCapacity) {
		newSize = (m_extraDataCapacity * 2) + 1;
		NiExtraData ** extraDataList = (NiExtraData **)Heap_Allocate(newSize * sizeof(NiExtraData*));
		// Copy the entries over
		memcpy_s(extraDataList, newSize * sizeof(NiExtraData*), m_extraData, m_extraDataLen * sizeof(NiExtraData*));
		// Zero the allocated entries
		for (UInt16 i = m_extraDataLen; i < newSize; i++)
			extraDataList[i] = NULL;

		Heap_Free(m_extraData);
		m_extraData = extraDataList;
		m_extraDataCapacity = newSize;
	}

	m_extraData[m_extraDataLen++] = extraData;
	qsort(m_extraData, m_extraDataLen, sizeof(NiExtraData*), ExtraDataCompare);
}

SInt32 NiObjectNET::GetIndexOf(NiExtraData * extraData)
{
	if (!extraData)
		return -1;
	if (!extraData->m_pcName)
		return -1;

	SInt16 min = 0;
	SInt16 max = (SInt16)m_extraDataLen - 1;
	while (max >= min) { // Iterative binary search
		SInt16 mid = (min + max) >> 1;
		if (m_extraData[mid]->m_pcName == extraData->m_pcName)
			return mid; // Found entry
		else if (m_extraData[mid]->m_pcName < extraData->m_pcName)
			min = mid + 1;
		else
			max = mid - 1;
	}

	return -1;
}

bool NiObjectNET::RemoveExtraData(NiExtraData * extraData)
{
	SInt32 index = GetIndexOf(extraData);
	if (index >= 0) { // Remove the entry
		extraData->DecRef();
		// Shift everything up
		for (UInt16 i = index; i < m_extraDataLen - 1; i++)
			m_extraData[i] = m_extraData[i + 1];
		// Remove the duplicate tail entry
		m_extraData[--m_extraDataLen] = NULL;
		if (m_extraDataLen == 0)
		{
			Heap_Free(m_extraData);
			m_extraData = NULL;
			m_extraDataLen = 0;
			m_extraDataCapacity = 0;
		}
		return true;
	}

	return false;
}

// This should be a BSFixedString before passing to this
NiExtraData * NiObjectNET::GetExtraData(BSFixedString name)
{
	SInt16 min = 0;
	SInt16 max = (SInt16)m_extraDataLen - 1;

	while (max >= min) { // Iterative binary search
		SInt16 mid = (min + max) >> 1;
		if (m_extraData[mid]->m_pcName == name.data)
			return m_extraData[mid];
		else if (m_extraData[mid]->m_pcName < name.data)
			min = mid + 1;
		else
			max = mid - 1;
	}

	return NULL;
}
