#include "PapyrusColorForm.h"

#include "Colors.h"
 
#include "GameForms.h"
#include "GameObjects.h"
#include "GameRTTI.h"

namespace papyrusColorComponent
{
	UInt32 GetAlpha(StaticFunctionTag* base, UInt32 argb)
	{
		return COLOR_ALPHA(argb);
	}
	UInt32 GetRed(StaticFunctionTag* base, UInt32 argb)
	{
		return COLOR_RED(argb);
	}
	UInt32 GetGreen(StaticFunctionTag* base, UInt32 argb)
	{
		return COLOR_GREEN(argb);
	}
	UInt32 GetBlue(StaticFunctionTag* base, UInt32 argb)
	{
		return COLOR_BLUE(argb);
	}
	float GetHue(StaticFunctionTag* base, UInt32 argb)
	{
		ARGBColor rgb(argb);
		return rgb.GetHue();
	}
	float GetSaturation(StaticFunctionTag* base, UInt32 argb)
	{
		ARGBColor color(argb);
		return color.GetSaturation();
	}
	float GetValue(StaticFunctionTag* base, UInt32 argb)
	{
		ARGBColor color(argb);
		return color.GetValue();
	}
	UInt32 SetColor(StaticFunctionTag* base, UInt32 red, UInt32 green, UInt32 blue, UInt32 alpha)
	{
		return MAKE_COLOR(alpha, red, green, blue);
	}
	UInt32 SetAlpha(StaticFunctionTag* base, UInt32 argb, UInt32 alpha)
	{
		return (argb & ~0xFF000000) | (alpha << 24);
	}
	UInt32 SetRed(StaticFunctionTag* base, UInt32 argb, UInt32 red)
	{
		return (argb & ~0x00FF0000) | (red << 16);
	}
	UInt32 SetGreen(StaticFunctionTag* base, UInt32 argb, UInt32 green)
	{
		return (argb & ~0x0000FF00) | (green << 8);
	}
	UInt32 SetBlue(StaticFunctionTag* base, UInt32 argb, UInt32 blue)
	{
		return (argb & ~0x000000FF) | blue;
	}
	UInt32 SetHue(StaticFunctionTag* base, UInt32 argb, float hue)
	{
		ARGBColor color(argb);
		color.SetHue(hue);
		return color.GetColor();
	}
	UInt32 SetSaturation(StaticFunctionTag* base, UInt32 argb, float sat)
	{
		ARGBColor color(argb);
		color.SetSaturation(sat);
		return color.GetColor();
	}
	UInt32 SetValue(StaticFunctionTag* base, UInt32 argb, float val)
	{
		ARGBColor color(argb);
		color.SetValue(val);
		return color.GetColor();
	}
};

namespace papyrusColorForm
{
	UInt32 GetColor(BGSColorForm* colorForm)
	{
		return (colorForm) ? (MACRO_SWAP32(colorForm->abgr) >> 8) | (colorForm->color.alpha << 24) : 0;
	}

	void SetColor(BGSColorForm* colorForm, UInt32 color)
	{
		if(colorForm) {
			colorForm->color.red = COLOR_RED(color);
			colorForm->color.green = COLOR_GREEN(color);
			colorForm->color.blue = COLOR_BLUE(color);
		}
	}
}

#include "PapyrusVM.h"
#include "PapyrusNativeFunctions.h"

void papyrusColorComponent::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, UInt32>("GetAlpha", "ColorComponent", papyrusColorComponent::GetRed, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, UInt32>("GetRed", "ColorComponent", papyrusColorComponent::GetRed, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, UInt32>("GetGreen", "ColorComponent", papyrusColorComponent::GetGreen, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, UInt32>("GetBlue", "ColorComponent", papyrusColorComponent::GetBlue, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, float, UInt32>("GetHue", "ColorComponent", papyrusColorComponent::GetHue, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, float, UInt32>("GetSaturation", "ColorComponent", papyrusColorComponent::GetSaturation, registry));

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, float, UInt32>("GetValue", "ColorComponent", papyrusColorComponent::GetValue, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, UInt32, UInt32, UInt32>("SetAlpha", "ColorComponent", papyrusColorComponent::SetAlpha, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, UInt32, UInt32, UInt32>("SetRed", "ColorComponent", papyrusColorComponent::SetRed, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, UInt32, UInt32, UInt32>("SetGreen", "ColorComponent", papyrusColorComponent::SetGreen, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, UInt32, UInt32, UInt32>("SetBlue", "ColorComponent", papyrusColorComponent::SetBlue, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, UInt32, UInt32, float>("SetHue", "ColorComponent", papyrusColorComponent::SetHue, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, UInt32, UInt32, float>("SetSaturation", "ColorComponent", papyrusColorComponent::SetSaturation, registry));

	registry->RegisterFunction(
		new NativeFunction2 <StaticFunctionTag, UInt32, UInt32, float>("SetValue", "ColorComponent", papyrusColorComponent::SetValue, registry));

	registry->SetFunctionFlags("ColorComponent", "GetAlpha", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ColorComponent", "GetRed", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ColorComponent", "GetGreen", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ColorComponent", "GetBlue", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ColorComponent", "GetHue", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ColorComponent", "GetSaturation", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ColorComponent", "GetValue", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ColorComponent", "SetAlpha", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ColorComponent", "SetRed", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ColorComponent", "SetGreen", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ColorComponent", "SetBlue", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ColorComponent", "SetHue", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ColorComponent", "SetSaturation", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ColorComponent", "SetValue", VMClassRegistry::kFunctionFlag_NoWait);
}

void papyrusColorForm::RegisterFuncs(VMClassRegistry* registry)
{
	registry->RegisterForm(BGSColorForm::kTypeID, "ColorForm");

	registry->RegisterFunction(
		new NativeFunction0 <BGSColorForm, UInt32>("GetColor", "ColorForm", papyrusColorForm::GetColor, registry));

	registry->RegisterFunction(
		new NativeFunction1 <BGSColorForm, void, UInt32>("SetColor", "ColorForm", papyrusColorForm::SetColor, registry));

	registry->SetFunctionFlags("ColorForm", "GetColor", VMClassRegistry::kFunctionFlag_NoWait);
	registry->SetFunctionFlags("ColorForm", "SetColor", VMClassRegistry::kFunctionFlag_NoWait);
}
