#ifndef __PUZZLE_H__
#define __PUZZLE_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

enum class PuzzleType
{
	BUTTON_ORDER,
	MIRROR,
	UNKNOWN
};

class Puzzle
{
public:
	Puzzle() {};
	Puzzle(PuzzleType type) : type(type) {}

	virtual bool Start() { return true; }
	virtual bool Update() { return false; }

	virtual void LoadAssets(pugi::xml_node node) {}
	//virtual void UnloadAssets();

public:

	SString name;
	PuzzleType type;
	int orderID;
	int nextPuzzleId;
	SString map;

};

#endif // __QUEST_H__