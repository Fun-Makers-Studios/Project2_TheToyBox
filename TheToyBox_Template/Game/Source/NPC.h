#ifndef __NPC_H__
#define __NPC_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class NPC : public Entity
{
public:

	NPC();
	virtual ~NPC();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	SDL_Rect boundaries;

	int npcid;

private:

	SDL_Texture* texture;
	const char* texturePath;

	Animation NPCAnim;
};

#endif // __NPC_H__