#include "Colors.h"

UInt32 ARGBColor::GetColor()
{
	return MAKE_COLOR(m_alpha, m_red, m_green, m_blue);
}

void ARGBColor::SetARGB(UInt8 alpha, UInt8 red, UInt8 green, UInt8 blue)
{
	m_alpha = alpha;
	m_red = red;
	m_green = green;
	m_blue = blue;
}

void ARGBColor::GetARGB(UInt8 & alpha, UInt8 & red, UInt8 & green, UInt8 & blue)
{
	alpha = m_alpha;
	red = m_red;
	green = m_green;
	blue = m_blue;
}

void ARGBColor::SetHSV(double hue, double saturation, double value)
{
	double h,s,v;
	double r,g,b;

	h = hue;
	s = saturation;
	v = value;

	if (hue < 0.0)
		hue += 360.0;

	if (s != 0.0) {
		double f, p, q, t;
		if (h == 360.0)
			h = 0.0;
		h /= 60.0;

		int i = (int)h;
		f = h - i;
		p = v * (1.0 - s);
		q = v * (1.0 - (s * f));
		t = v * (1.0 - (s * (1.0 - f)));

		switch (i) {
				case 0: r = v;	g = t;	b = p;	break;
				case 1: r = q;	g = v;	b = p;	break;
				case 2: r = p;	g = v;	b = t;	break;
				case 3: r = p;	g = q;	b = v;	break;
				case 4: r = t;	g = p;	b = v;	break;
				case 5: r = v;	g = p;	b = q;	break;
		}
	}
	else {
		r = v;
		g = v;
		b = v;
	}

	m_red = r * 255;
	m_green = g * 255;
	m_blue = b * 255;
}

void ARGBColor::GetHSV(double & hue, double & saturation, double & value)
{
	double r,g,b, rgb_max, rgb_min, delta, h, s;
	r = m_red / 255.0;
	g = m_green / 255.0;
	b = m_blue / 255.0;
	rgb_max = MAX3(r, g, b);
	rgb_min = MIN3(r, g, b);
	delta = rgb_max - rgb_min;
	h = 0;
	s = 0;

	if ( rgb_max != 0.0 )
		s = delta / rgb_max;

	if ( s != 0.0 )
	{
		double rc = (rgb_max - r) / delta;
		double gc = (rgb_max - g) / delta;
		double bc = (rgb_max - b) / delta;

		if ( r == rgb_max )
			h = bc - gc;
		else if ( g == rgb_max )
			h = 2.0f + rc - bc;
		else if ( b == rgb_max )
			h = 4.0f + gc - rc;

		h *= 60.0f;
		if ( h < 0.0 )
			h += 360.0f;
	}

	hue = h;
	saturation = s;
	value = rgb_max;
}

UInt8 ARGBColor::GetAlpha()
{
	return m_alpha;
}

UInt8 ARGBColor::GetRed()
{
	return m_red;
}

UInt8 ARGBColor::GetGreen()
{
	return m_green;
}

UInt8 ARGBColor::GetBlue()
{
	return m_blue;
}

void ARGBColor::SetAlpha(UInt8 alpha)
{
	m_alpha = alpha;
}

void ARGBColor::SetRed(UInt8 red)
{
	m_red = red;
}

void ARGBColor::SetGreen(UInt8 green)
{
	m_green = green;
}

void ARGBColor::SetBlue(UInt8 blue)
{
	m_blue = blue;
}

void ARGBColor::SetHue(double newHue)
{
	double hue;
	double sat;
	double val;
	GetHSV(hue, sat, val);
	SetHSV(newHue, sat, val);
}

void ARGBColor::SetSaturation(double newSat)
{
	double hue;
	double sat;
	double val;
	GetHSV(hue, sat, val);
	SetHSV(hue, newSat, val);
}

void ARGBColor::SetValue(double newVal)
{
	double hue;
	double sat;
	double val;
	GetHSV(hue, sat, val);
	SetHSV(hue, sat, newVal);
}

double ARGBColor::GetHue()
{
	double hue;
	double sat;
	double val;
	GetHSV(hue, sat, val);
	return hue;
}

double ARGBColor::GetSaturation()
{
	double hue;
	double sat;
	double val;
	GetHSV(hue, sat, val);
	return sat;
}

double ARGBColor::GetValue()
{
	double hue;
	double sat;
	double val;
	GetHSV(hue, sat, val);
	return val;
}
