#ifndef __MIRRORPUZZLE_H__
#define __MIRRORPUZZLE_H__

#include "Puzzle.h"
#include "PuzzlePiece.h"
#include "Point.h"
#include "Collisions.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class MirrorPuzzle : public Puzzle
{
public:
	MirrorPuzzle(pugi::xml_node node);
	virtual ~MirrorPuzzle();

	bool Update() override;

	void LoadAssets(pugi::xml_node node) override;
	void ObjectTriggerCheck();
	void ResetPuzzle();
	void OpenDoor();

public:

	List<PuzzlePiece*> pieces;

private:

	PuzzlePiece* piece = nullptr;

};

#endif // __COLLECTQUEST_H__