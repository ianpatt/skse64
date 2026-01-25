#include "skse64/NiExtraData.h"
#include "skse64/NiGeometry.h"

// ??_7BSFaceGenBaseMorphExtraData@@6B@
RelocAddr<uintptr_t> s_BSFaceGenBaseMorphExtraDataVtbl(0x01601508);
// ??_7NiStringsExtraData@@6B@
RelocAddr<uintptr_t> s_NiStringsExtraDataVtbl(0x01766B50);
// ??_7NiBinaryExtraData@@6B@
RelocAddr<uintptr_t> s_NiBinaryExtraDataVtbl(0x0176B0B0);
// ??_7NiBooleanExtraData@@6B@
RelocAddr<uintptr_t> s_NiBooleanExtraDataVtbl(0x0176B240);

NiExtraData* NiExtraData::Create(UInt32 size, uintptr_t vtbl)
{
	void* memory = Heap_Allocate(size);
	memset(memory, 0, size);
	NiExtraData* xData = (NiExtraData*)memory;
	((uintptr_t*)memory)[0] = vtbl;
	return xData;
}

NiStringsExtraData * NiStringsExtraData::Create(BSFixedString name, BSFixedString * stringData, UInt32 size)
{
	NiStringsExtraData* data = (NiStringsExtraData*)NiExtraData::Create(sizeof(NiStringsExtraData), s_NiStringsExtraDataVtbl.GetUIntPtr());
	data->m_pcName = const_cast<char*>(name.data);
	data->m_data = (char**)Heap_Allocate(sizeof(char*) * size);
	data->m_size = size;
	
	for (int i = 0; i < size; i++)
	{
		UInt32 strLength = strlen(stringData[i].data) + 1;
		data->m_data[i] = (char*)Heap_Allocate(sizeof(char*) * strLength);
		memcpy(data->m_data[i], stringData[i].data, sizeof(char) * strLength);
	}

	return data;
}

void NiStringsExtraData::SetData(BSFixedString * stringData, UInt32 size)
{
	if (size != m_size)
	{
		if (m_data)
		{
			for (int i = 0; i < size; i++)
			{
				Heap_Free(m_data[i]);
			}
			Heap_Free(m_data);
			m_data = NULL;
		}
		if (size > 0) {
			m_data = (char**)Heap_Allocate(sizeof(char*) * size);
		}
		
		m_size = size;
	}

	for (int i = 0; i < size; i++)
	{
		UInt32 strLength = strlen(stringData[i].data) + 1;
		m_data[i] = (char*)Heap_Allocate(sizeof(char*) * strLength);
		memcpy(m_data[i], stringData[i].data, sizeof(char) * strLength);
	}
}

NiBinaryExtraData * NiBinaryExtraData::Create(BSFixedString name, char * binary, UInt32 size)
{
	NiBinaryExtraData* data = (NiBinaryExtraData*)NiExtraData::Create(sizeof(NiBinaryExtraData), s_NiBinaryExtraDataVtbl.GetUIntPtr());
	data->m_pcName = const_cast<char*>(name.data);
	if (size > 0)
	{
		data->m_data = (char*)Heap_Allocate(size);
		memcpy(data->m_data, binary, size);
		data->m_size = size;
	}
	else
	{
		data->m_data = NULL;
		data->m_size = 0;
	}
	
	return data;
}

NiBooleanExtraData * NiBooleanExtraData::Create(BSFixedString name, bool value)
{
	NiBooleanExtraData* data = (NiBooleanExtraData*)NiExtraData::Create(sizeof(NiBooleanExtraData), s_NiBooleanExtraDataVtbl.GetUIntPtr());
	data->m_pcName = const_cast<char*>(name.data);
	data->m_data = value;
	return data;
}

void NiBinaryExtraData::SetData(char * data, UInt32 size)
{
	if (m_data)
	{
		Heap_Free(m_data);
		m_data = NULL;
	}

	if (size > 0)
	{
		m_data = (char*)Heap_Allocate(size);
		memcpy(m_data, data, size);
	}

	m_size = size;
}

BSFaceGenBaseMorphExtraData* BSFaceGenBaseMorphExtraData::Create(NiGeometryData * geometryData, bool copy)
{
	BSFaceGenBaseMorphExtraData* data = (BSFaceGenBaseMorphExtraData*)NiExtraData::Create(sizeof(BSFaceGenBaseMorphExtraData), s_BSFaceGenBaseMorphExtraDataVtbl.GetUIntPtr());
	data->m_pcName = const_cast<char*>(BSFixedString("FOD").data);
	data->m_uiRefCount = 0;
	data->modelVertexCount = 0;
	data->vertexCount = 0;
	data->vertexData = NULL;

	if (geometryData) {
		data->vertexCount = geometryData->m_usVertices;
		data->modelVertexCount = geometryData->m_usVertices;

		data->vertexData = (NiPoint3*)Heap_Allocate(sizeof(NiPoint3) * data->vertexCount);
		if(copy)
			memcpy(data->vertexData, geometryData->m_pkVertex, sizeof(NiPoint3) * data->vertexCount);
		else
			memset(data->vertexData, 0, sizeof(NiPoint3) * data->vertexCount);
	}

	return data;
}
