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
	void UnloadAssets() override;
	void ObjectTriggerCheck();
	void ResetPuzzle();
	void OpenDoor();
	bool CompletionEvent();

public:

	List<PuzzlePiece*> pieces;

private:

	uint activatedObj = 0;
	uint maxObj = 0;

	PuzzlePiece* piece = nullptr;
	bool isAttached = false;
};

#endif // __COLLECTQUEST_H__