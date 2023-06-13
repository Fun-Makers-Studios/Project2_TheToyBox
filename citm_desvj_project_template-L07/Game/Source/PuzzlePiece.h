#ifndef __PUZZLEPIECE_H__
#define __PUZZLEPIECE_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Collisions.h"
#include "SDL/include/SDL.h"

#define PIECE_BOUNDARY 25.0f

struct SDL_Texture;

enum class PieceType
{
	BUTTON,
	MOV_OBJ,
	DOOR,
	TRIGGER_PLATFORM,
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

private:

	SDL_Texture* texture = nullptr;
	const char* texturePath = nullptr;

	Animation* currentAnim = nullptr;
	Animation idleAnim;
	Animation activeAnim;

	int width;
	int height;

	SDL_RendererFlip fliped = SDL_FLIP_NONE;

public:

	dPoint startPos;

	Body boundaries;

	int order = 0;

	bool activated = false;

	PieceType pieceType;

	bool isAttached = false; //For pieces to move with th player
};

#endif // __NPC_H__