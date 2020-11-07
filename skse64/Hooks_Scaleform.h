#pragma once

#include "skse64/PluginAPI.h"

void SetLastControlDown(const char * control, UInt32 keycode);
void SetLastControlUp(const char * control, UInt32 keycode);

void Hooks_Scaleform_Commit(void);

bool RegisterScaleformPlugin(const char * name, SKSEScaleformInterface::RegisterCallback callback);
void RegisterScaleformInventory(SKSEScaleformInterface::RegisterInventoryCallback callback);
