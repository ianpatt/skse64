#pragma once

#include "skse64/GameTypes.h"
#include "skse64/GameFormComponents.h"

// 9C
class BSNavmesh
{
public:
	// 28
	struct Data50
	{
		UInt32	unk00;	// 00 - init'd to 0
		UInt32	unk04;	// 04
		UInt32	unk08;	// 08
		UInt32	unk0C;	// 0C - init'd to 7F7FFFFF
		UInt32	unk10;	// 10
		UInt32	unk14;	// 14
		UInt32	unk18;	// 18
		UInt32	unk1C;	// 1C
		UInt32	unk20;	// 20
		UInt32	unk24;	// 24 - init'd to 0
	};

	// 10
	class Data84
	{
	public:
		virtual	~Data84();

		//		void	** _vtbl;	// 00
		UInt32	unk04;		// 04 - ctor arg, prime?
		void	* unk08;	// 08 - buf, len = unk04 * 4
		UInt32	unk0C;		// 0C
	};

	virtual ~BSNavmesh();

	//	void	** _vtbl;						// 00
	BSIntrusiveRefCounted		refCount;	// 04
	UnkArray	unk08;		// 08
	UnkArray	unk14;		// 14
	UnkArray	unk20;		// 20
	UnkArray	unk2C;		// 2C
	UnkArray	unk38;		// 38
	UnkArray	unk44;		// 44
	Data50						unk50;		// 50
	UnkArray	unk78;		// 78
	Data84						* unk84;	// 84
	UnkArray	unk88;		// 88
	UInt32						unk94;		// 94
	UInt32						unk98;		// 98
};

//STATIC_ASSERT(sizeof(BSNavmesh) == 0x9C);

// 8
class BSNavmeshInfoMap
{
public:
	virtual ~BSNavmeshInfoMap();

	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);

//	void	** _vtbl;	// 00
	UInt32	unk04;		// 04
};

// ?
class BSPrecomputedNavmeshInfoPathMap
{
public:
};

// 2C
class PrecomputedNavmeshInfoPathMap : public BSPrecomputedNavmeshInfoPathMap
{
public:
	// 20
	struct Data0C
	{
		UInt32	unk00;		// 00
		UInt32	unk04;		// 04
		UInt32	unk08;		// 08 - 0
		UInt32	unk0C;		// 0C - 0
		UInt32	unk10;		// 10 - 0
		void	* unk14;	// 14 - ptr to 0xDEADBEEF
		UInt32	unk18;		// 18
		UInt32	unk1C;		// 1C
	};

	UnkArray	unk00;	// 00
	Data0C						unk0C;	// 0C
};

//STATIC_ASSERT(sizeof(PrecomputedNavmeshInfoPathMap) == 0x2C);
