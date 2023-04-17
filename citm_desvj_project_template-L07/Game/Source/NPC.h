#ifndef __NPC_H__
#define __NPC_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Physics.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class NPC : public Entity
{
public:

	NPC();
	virtual ~NPC();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void DialogTriggerCheck();

private:

	SDL_Texture* texture;
	const char* texturePath;

	Animation* currentAnim;
	Animation idleAnim;

	SDL_RendererFlip fliped = SDL_FLIP_NONE;

	b2Vec2 startPos;
	b2Vec2 velocity;

	int width;
	int height;

	bool isFliped = false;

public:

	PhysBody* pbody;

	SDL_Rect boundaries;

	int npcid;
	int dialogueid;
};

#endif // __NPC_H__