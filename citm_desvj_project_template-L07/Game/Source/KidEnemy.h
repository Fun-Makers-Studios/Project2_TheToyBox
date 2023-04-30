#ifndef __KIDENEMY_H__
#define __KIDENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Collisions.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class KidEnemy : public Entity
{
public:

	KidEnemy(pugi::xml_node parameters);

	virtual ~KidEnemy();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	void OnCollision();

	void ResetBat();

public:

	Animation* currentAnim = nullptr;

	Animation idleAnim;

	uint stompSFX = 0;
	const char* stompSFXPath = nullptr;
	uint powerUpSFX = 0;
	const char* powerUpSFXPath = nullptr;
	uint batHitSFX = 0;
	const char* batHitSFXPath = nullptr;

	bool onGround = false;
	bool dead = false;
	bool onCollision = false;

	Body* body = nullptr;

private:

	SDL_Texture* texture = nullptr;
	const char* texturePath = nullptr;

	SDL_RendererFlip fliped = SDL_FLIP_NONE;

	dPoint startPos;

	int width;
	int height;

};
#endif // __ENEMY_H__

