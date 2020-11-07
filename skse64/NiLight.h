#pragma once

#include "skse64/NiTypes.h"
#include "skse64/NiObjects.h"

// D4
class NiLight : public NiAVObject
{
public:
	virtual ~NiLight();

	float	unkA8;
	float	unkAC;
	float	unkB0;
	float	unkB4;
	float	unkB8;
	float	unkBC;
	float	unkC0;
	float	unkC4;
	float	unkC8;
	float	unkCC;
	UInt32	unkD0;
};

// D4
class NiAmbientLight : public NiLight
{
public:
	virtual ~NiAmbientLight();
};

// E0
class NiPointLight : public NiLight
{
public:
	virtual ~NiPointLight();

	float	unkD4;
	float	unkD8;
	float	unkDC;
};

// EC
class NiDirectionalLight : public NiLight
{
public:
	virtual ~NiDirectionalLight();

	float	unkD4;
	float	unkD8;
	float	unkDC;
	float	unkE0;
	float	unkE4;
	float	unkE8;
};
