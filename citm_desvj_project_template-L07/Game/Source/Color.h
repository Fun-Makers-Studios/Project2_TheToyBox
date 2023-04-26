#pragma once

#include "Defs.h"

struct Color
{
	uchar r, g, b, a;

	Color() : r(0), g(0), b(0), a(255)
	{}

	Color(uchar r, uchar g, uchar b, uchar a = 255) : r(r), g(g), b(b), a(a)
	{}

	void Set(uchar r, uchar g, uchar b, uchar a = 255)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	
	void SetAlpha(Color colorcito, uchar a)
	{
		this->r = colorcito.r;
		this->g = colorcito.g;
		this->b = colorcito.b;
		this->a = a;
	}

	uchar* operator & ()
	{
		return (uchar*)this;
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
extern Color Beige;