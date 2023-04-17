#pragma once

#include "App.h"


enum class ScaleType
{
	NORMAL,
	WORLD,
	FIGHT,
	UI_50,
	UI_100
};


class Scale
{
public:

	int ScaleTypeToInt(ScaleType scale)
	{
		switch (scale)
		{
		case ScaleType::NORMAL: return 1;
		case ScaleType::WORLD:	return 2;
		case ScaleType::FIGHT:	return 3;
		case ScaleType::UI_50:	return 1;
		case ScaleType::UI_100: return 1;
		default: return 1;
		}
	}
};