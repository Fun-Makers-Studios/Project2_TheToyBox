#ifndef __PUZZLEPIECE_H__
#define __PUZZLEPIECE_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Collisions.h"
#include "SDL/include/SDL.h"

#define PIECE_BOUNDARY 48.0f

struct SDL_Texture;

enum class PieceType
{
	BUTTON,
	MOV_OBJ,
	DOOR,
	UNKNOWN
};

class PuzzlePiece : public Entity
{
public:

	PuzzlePiece(pugi::xml_node parameters);
	virtual ~PuzzlePiece();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	//void TriggerCheck();

private:

	SDL_Texture* texture = nullptr;
	const char* texturePath = nullptr;

	Animation* currentAnim = nullptr;
	Animation idleAnim;
	Animation activeAnim;

	dPoint startPos;

	int width;
	int height;

	SDL_RendererFlip fliped = SDL_FLIP_NONE;

public:

	Body boundaries;

	uint order = 0;

	bool activated = false;

	int pieceID;

	PieceType pieceType;
};

#endif // __NPC_H__