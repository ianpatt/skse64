#pragma once

#include "skse64_common/Utilities.h"

class NiAVObject;

class BSModelDB
{
public:
	struct ModelData
	{
		UInt64	unk00;			// 00

		enum
		{
			kFlag_Unk1			= (1 << 0),
			kFlag_Dynamic		= (1 << 1),
			kFlag_PostProcess	= (1 << 3),
			kFlag_Unk2			= (1 << 4),
			kFlag_Unk3			= (1 << 5)
		};

		UInt8	modelFlags;		// 08
		// ...
	};

	class BSModelProcessor
	{
	public:
		virtual ~BSModelProcessor() { };

		virtual void Process(ModelData * modelData, const char * modelName, NiAVObject ** root, UInt32 * typeOut) { };
	};

	class TESProcessor : public BSModelProcessor
	{
	public:
		TESProcessor() { }
		virtual ~TESProcessor() { CALL_MEMBER_FN(this, dtor)(); };

		virtual void Process(ModelData * modelData, const char * modelName, NiAVObject ** root, UInt32 * typeOut) override { CALL_MEMBER_FN(this, Impl_Process)(modelData, modelName, root, typeOut); }

		MEMBER_FN_PREFIX(TESProcessor);
		// find from ??_7BSModelProcessor@BSModelDB@@6B@
		DEFINE_MEMBER_FN(dtor, void, 0x00D2ECA0);
		DEFINE_MEMBER_FN(Impl_Process, void, 0x00181800, ModelData * modelData, const char * modelName, NiAVObject ** root, UInt32 * typeOut);
	};
};

extern RelocPtr <BSModelDB::BSModelProcessor*> g_TESProcessor;
