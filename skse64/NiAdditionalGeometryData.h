#pragma once

#include "skse64/NiObjects.h"
#include "skse64/NiTypes.h"

class NiAdditionalGeometryData : public NiObject
{
public:
	UInt32	m_uiRevID;							// 08
	UInt16	m_usVertexCount;					// 0C
	UInt8	m_ucNumStreams;						// 0E
	UInt8	unk0F;								// 0F
	void	* m_pkStreamEntries;				// 10
	UInt32	unk14;								// 14
	void	* m_pkBlocks;						// 18
	UInt8	m_ucNumBlocks;						// 1C
	UInt8	unk1D;								// 1D
	UInt8	unk1E;								// 1E
	UInt8	unk1F;								// 1F
};
