#include "ScaleformCallbacks.h"
#include <typeinfo>

UInt32 g_GFxFunctionHandler_count = 0;

GFxFunctionHandler::GFxFunctionHandler()
{
	g_GFxFunctionHandler_count++;
}

GFxFunctionHandler::~GFxFunctionHandler()
{
	g_GFxFunctionHandler_count--;
}

FunctionHandlerCache g_functionHandlerCache;

RelocAddr<FxDelegateHandler::Callback> PlaySoundCallback(0x008DA5C0);

FxResponseArgsList::FxResponseArgsList()
{
	args.values = nullptr;
	args.size = 0;
	args.capacity = 0;
}
FxResponseArgsList::~FxResponseArgsList()
{
	Clear();
}

void FxResponseArgsList::Clear()
{
	if (args.values) {
		for (UInt32 i = 0; i < args.size; i++)
			args.values[i].CleanManaged();
		ScaleformHeap_Free(args.values);
		args.values = NULL;
		args.size = 0;
	}
}
