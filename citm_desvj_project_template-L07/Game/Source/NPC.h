#ifndef __NPC_H__
#define __NPC_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Collisions.h"
#include "SDL/include/SDL.h"

#define NPC_BOUNDARY 48.0f

struct SDL_Texture;

class NPC : public Entity
{
public:

	NPC(pugi::xml_node parameters);
	virtual ~NPC();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	void OnCollision();

	void DialogTriggerCheck();

private:

	SDL_Texture* texture = nullptr;
	const char* texturePath = nullptr;
	SDL_Texture* shadowTexture = nullptr;
	const char* shadowTexturePath = nullptr;

	Animation* currentAnim = nullptr;
	Animation idleAnim;

	SDL_RendererFlip fliped = SDL_FLIP_NONE;

	dPoint startPos;

	int width;
	int height;

	bool isFliped = false;

public:

	Body* boundaries = nullptr;

	int npcid;
	int dialogueid;
};

#endif // __NPC_H__