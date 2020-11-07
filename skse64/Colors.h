#pragma once

#include "common/ITypes.h"

#define MIN3(x,y,z)  ((y) <= (z) ? ((x) <= (y) ? (x) : (y)) : ((x) <= (z) ? (x) : (z)))
#define MAX3(x,y,z)  ((y) >= (z) ? ((x) >= (y) ? (x) : (y)) : ((x) >= (z) ? (x) : (z)))

class ARGBColor
{
public:
	ARGBColor(UInt32 argb) {
		m_alpha = COLOR_ALPHA(argb);
		m_red = COLOR_RED(argb);
		m_green = COLOR_GREEN(argb);
		m_blue = COLOR_BLUE(argb);
	};
	ARGBColor(UInt8 _a, UInt8 _r, UInt8 _g, UInt8 _b) : m_alpha(_a), m_red(_r), m_green(_g), m_blue(_b) {};

	UInt32 GetColor();

	void SetARGB(UInt8 alpha, UInt8 red, UInt8 green, UInt8 blue);
	void GetARGB(UInt8 & alpha, UInt8 & red, UInt8 & green, UInt8 & blue);

	void SetHSV(double hue, double saturation, double value);
	void GetHSV(double & hue, double & saturation, double & value);

	UInt8 GetAlpha();
	UInt8 GetRed();
	UInt8 GetGreen();
	UInt8 GetBlue();

	void SetAlpha(UInt8 alpha);
	void SetRed(UInt8 red);
	void SetGreen(UInt8 green);
	void SetBlue(UInt8 blue);

	void SetHue(double hue);
	void SetSaturation(double saturation);
	void SetValue(double value);

	double GetHue();
	double GetSaturation();
	double GetValue();

private:
	UInt8 m_alpha;
	UInt8 m_red;
	UInt8 m_green;
	UInt8 m_blue;
};
