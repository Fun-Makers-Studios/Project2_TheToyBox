#ifndef __MIRRORPUZZLE_H__
#define __MIRRORPUZZLE_H__

#include "Puzzle.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class MirrorPuzzle : public Puzzle
{
public:
	MirrorPuzzle(pugi::xml_node node);
	virtual ~MirrorPuzzle();

	bool Update() override;

public:

	int itemId;
};

#endif // __COLLECTQUEST_H__