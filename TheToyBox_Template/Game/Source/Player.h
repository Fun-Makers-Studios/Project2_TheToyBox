#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "List.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	bool PostUpdate();

	bool CleanUp();


public:

	Animation* currentAnim = nullptr;

	Animation idleanim;

	bool godMode = false;

private:

	// Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

};

#endif // __PLAYER_H__