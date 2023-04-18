#pragma once

#include "Defs.h"

struct Color
{
	uint r, g, b, a;

	Color() : r(0), g(0), b(0), a(255)
	{}

	Color(float r, float g, float b, float a = 255) : r(r), g(g), b(b), a(a)
	{}

	void Set(float r, float g, float b, float a = 255)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	float* operator & ()
	{
		return (float*)this;
	}
};

extern Color Red;
extern Color Orange;
extern Color Yellow;
extern Color Green;
extern Color Blue;
extern Color Magenta;
extern Color Pink;
extern Color Black;
extern Color White;
extern Color Grey;