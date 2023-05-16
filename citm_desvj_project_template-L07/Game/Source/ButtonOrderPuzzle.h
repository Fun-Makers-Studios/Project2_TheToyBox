#ifndef __BUTTONORDERPUZZLE_H__
#define __BUTTONORDERPUZZLE_H__

#include "Puzzle.h"
#include "PuzzlePiece.h"
#include "Point.h"
#include "Collisions.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class ButtonOrderPuzzle : public Puzzle
{
public:
	ButtonOrderPuzzle(pugi::xml_node node);
	virtual ~ButtonOrderPuzzle();

	bool Update() override;

	void LoadAssets(pugi::xml_node node);
	void ButtonTriggerCheck();
	void ResetPuzzle();
	void OpenDoor();

public:

	SString map;
	List<PuzzlePiece*> pieces;

private:

	PuzzlePiece* piece = nullptr;

	uint actualOrder = 1;

};

#endif // __COLLECTQUEST_H__