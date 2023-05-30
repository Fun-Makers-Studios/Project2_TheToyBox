#pragma once

#include "App.h"
#include "string.h"


enum class ScaleType
{
	NONE,
	NORMAL,
	TITLE,
	WORLD,
	HOUSE,
	FIGHT,
	UI_50,
	UI_100,
	UI_200,
};


class Scale
{
public:

	double ScaleTypeToInt(ScaleType scale)
	{
		switch (scale)
		{
		case ScaleType::NORMAL: return 1;
		case ScaleType::TITLE:	return 4;	// 4
		case ScaleType::WORLD:	return 2;	// 2
		case ScaleType::HOUSE:	return 1;
		case ScaleType::FIGHT:	return 3;	// 3
		case ScaleType::UI_50:	return 0.5f;
		case ScaleType::UI_100:	return 1;
		case ScaleType::UI_200:	return 2;
		default: return 1;
		}
	}

	std::string ScaleTypeToString(ScaleType scale)
	{
		switch (scale)
		{
		case ScaleType::NORMAL: return "normal";
		case ScaleType::WORLD:	return "world";
		case ScaleType::HOUSE:	return "house";
		case ScaleType::FIGHT:	return "fight";
		default: return "default";
		}
	}

	ScaleType GetCurrentScale()
	{
		return currentScale;
	}

	void SetCurrentScale(ScaleType scale)
	{
		this->currentScale = scale;
	}

private:

	ScaleType currentScale = ScaleType::WORLD;
};