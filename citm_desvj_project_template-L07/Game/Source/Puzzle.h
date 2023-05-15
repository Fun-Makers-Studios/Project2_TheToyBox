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

public:

	SString name;
	PuzzleType type;
	int id;
	int nextPuzzleId;

};

#endif // __QUEST_H__