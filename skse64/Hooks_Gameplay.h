#pragma once

class TESObjectREFR;

void Hooks_Gameplay_EnableForceContainerCategorization(bool enable);
void Hooks_Gameplay_EnableMapMenuMouseWheel(bool enable);

TESObjectREFR* Hooks_Gameplay_GetCrosshairRef();

void Hooks_Gameplay_Commit(void);
