#pragma once

#include "skse64/NiObjects.h"
#include "skse64/GameAPI.h"
#include "skse64_common/Utilities.h"
#include "skse64/GameTypes.h"

class BSResourceStream;

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

// 44
class NiPixelFormat
{
public:
	// 0C
	class NiComponentSpec
	{
	public:
		UInt32	type;		// 00
		UInt32	format;		// 04
		UInt8	bits;		// 08
		UInt8	isSigned;	// 09
		UInt8	pad0A;		// 0A
		UInt8	pad0B;		// 0B
	};

	UInt8	flags;			// 00
	UInt8	bpp;			// 01
	UInt8	srgb;			// 02
	UInt8	pad03;			// 03
	UInt32	format;			// 04
	UInt32	tiling;			// 08
	UInt32	rendererHint;	// 0C
	UInt32	extraData;		// 10

	NiComponentSpec	components[4];	// 14
};


class NiPersistentSrcTextureRendererData : public NiObject
{
public:
	virtual ~NiPersistentSrcTextureRendererData();
};

MAKE_NI_POINTER(NiPersistentSrcTextureRendererData);

class NiDX9PersistentSrcTextureRendererData : public NiPersistentSrcTextureRendererData
{
public:
	virtual ~NiDX9PersistentSrcTextureRendererData();
};

class NiPixelData : public NiObject
{
public:
	virtual ~NiPixelData();

	NiPixelFormat	pixelFormat;		// 08
	void *			unk0C;				// 4C

	UInt8 *			m_pucPixels;		// 50	[mipmapLevels]
	UInt32 *		m_puiWidth;			// 54	[mipmapLevels]
	UInt32 *		m_puiHeight;		// 58	[mipmapLevels]
	UInt32 *		m_puiOffset;		// 5C	[mipmapLevels + 1]

	UInt32			m_uiMipmapLevels;	// 60
	UInt32			m_uiPixelStride;	// 64

	UInt32			unk68;				// 68
	UInt32			m_uiFaces;			// 6C
	UInt8			unk70;				// 70
	UInt8			pad71[3];			// 71

	UInt32 GetWidth(UInt32 mipmapLevel)
	{
		return m_puiWidth[mipmapLevel];
	}

	UInt32 GetHeight(UInt32 mipmapLevel)
	{
		return m_puiHeight[mipmapLevel];
	}

	UInt8 * GetPixels(UInt32 uiMipmapLevel = 0, UInt32 uiFace = 0)
	{
		return m_pucPixels + uiFace*m_puiOffset[uiMipmapLevel] +
			m_puiOffset[uiMipmapLevel];
	};
};

MAKE_NI_POINTER(NiPixelData);

// 14
class Ni2DBuffer : public NiObject
{
public:
	virtual ~Ni2DBuffer();

	UInt32	width;			// 08
	UInt32	height;			// 0C
	void	* rendererData;	// 10
};

// 24
class NiTexture : public NiObject
{
public:
	virtual ~NiTexture();

	virtual void	Unk_25(void);
	virtual void	Unk_26(void);
	virtual void	Unk_27(void);
	virtual void	Unk_28(UInt32 * unk1, UInt32 * unk2, UInt32 * unk3, UInt32 * unk4);
	virtual void	Unk_29(void);
	virtual void	Unk_2A(void);
	virtual void	Unk_2B(void);

	// 28
	class RendererData
	{
	public:
		RendererData(UInt32 w, UInt32 h) 
			: texture(nullptr)
			, unk08(0)
			, resourceView(nullptr)
			, width(w)
			, height(h)
			, unk1C(1)
			, unk1D(0x1C)
			, unk1E(0)
			, unk20(1)
			, unk24(0x00130012)
		{}

		ID3D11Texture2D				* texture;		// 00
		UInt64						unk08;			// 08
		ID3D11ShaderResourceView	* resourceView;	// 10
		UInt16						width;			// 18
		UInt16						height;			// 1A
		UInt8						unk1C;			// 1C
		UInt8						unk1D;			// 1D
		UInt16						unk1E;			// 1E
		UInt32						unk20;			// 20
		UInt32						unk24;			// 24

		DEFINE_STATIC_HEAP(Heap_Allocate, Heap_Free);
	};
	
	UInt32				unk10;			// 10 - 6
	UInt32				unk14;			// 14 - 3
	UInt32				unk18;			// 18 - 2
	UInt32				unk1C;			// 1C
	const char			* name;			// 20
	UInt32				unk28;			// 28 - FFFFFF
	UInt32				unk2C;			// 2C
	NiTexture			* prevTexture;	// 30
	NiTexture			* nextTexture;	// 38
	BSResourceStream	* resourceData;	// 40
	RendererData		* rendererData;	// 48
};

// ??
class NiRenderedTexture : public NiTexture
{
public:
	virtual ~NiRenderedTexture();

	MEMBER_FN_PREFIX(NiRenderedTexture);
	DEFINE_MEMBER_FN(ctor, void, 0x00000000);
	DEFINE_MEMBER_FN(UpdateVirtualImage, void, 0x00ECDF50, NiRenderedTexture * newTexture);
	DEFINE_MEMBER_FN(AddVirtualImage, UInt8, 0x00ECE010, BSFixedString linkageName);
	DEFINE_MEMBER_FN(ReleaseVirtualImage, void, 0x00ECDEE0);
};

// 58
class NiRenderedCubeMap : public NiRenderedTexture
{
public:
	virtual ~NiRenderedCubeMap();

	UInt32	unk3C;	// 3C
	UInt32	unk40;	// 40
	UInt32	unk44;	// 44
	UInt32	unk48;	// 48
	UInt32	unk4C;	// 4C
	UInt32	unk50;	// 50
	UInt32	unk54;	// 54
};

// 34
class NiSourceTexture : public NiTexture
{
public:
	UInt8	flags;	// 50 - C (normal), 8 (rendered)
};

typedef NiTexture * (*_CreateSourceTexture)(const BSFixedString & name);
extern RelocAddr<_CreateSourceTexture> CreateSourceTexture;

typedef void (*_LoadTexture)(const char * path, UInt8 unk1, NiPointer<NiTexture> & texture, bool unk2);
extern RelocAddr<_LoadTexture> LoadTexture;