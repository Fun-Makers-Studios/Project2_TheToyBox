#ifndef __KIDENEMY_H__
#define __KIDENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Physics.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class KidEnemy : public Entity
{
public:

	KidEnemy();

	virtual ~KidEnemy();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	void MovementDirection(const iPoint& origin, const iPoint& destination);

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void ResetBat();

public:

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnim;

	//Set of animations
	Animation idleAnim;
	
	//Set of SFX
	uint stompSFX = 0;
	const char* stompSFXPath;
	uint powerUpSFX = 0;
	const char* powerUpSFXPath;
	uint batHitSFX = 0;
	const char* batHitSFXPath;

	bool onGround = false;
	bool dead = false;
	bool onCollision = false;

	PhysBody* pbody;

private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	SDL_RendererFlip fliped = SDL_FLIP_NONE;

	b2Vec2 velocity;
	b2Vec2 startPos;

	int width;
	int height;

};
#endif // __ENEMY_H__

