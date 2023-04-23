#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Physics.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void ResetPlayerPos();

public:

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnim = nullptr;

	//Set of animations (JUMP ANIM DDOESN'T EXISTS)
	Animation idlePlayer;
	Animation runPlayer;
	Animation attackPlayer;
	Animation diePlayer;
	Animation hitPlayer;

	//Set of SFX
	uint jumpSFX = 0;
	const char* jumpSFXPath = nullptr;
	uint dieSFX = 0;
	const char* dieSFXPath = nullptr;
	uint hurtSFX = 0;
	const char* hurtSFXPath = nullptr;
	uint pickCoinSFX = 0;
	const char* pickCoinSFXPath = nullptr;
	uint pickLifeSFX = 0;
	const char* pickLifeSFXPath = nullptr;
	uint levelCompletedSFX = 0;
	const char* levelCompletedSFXPath = nullptr;
	uint selectSFX = 0;
	const char* selectSFXPath = nullptr;
	uint shortRangeAttackSFX = 0;
	const char* shortRangeAttackSFXPath = nullptr;

	bool dead = false;
	bool godMode = false;
	bool onCollision = false;

	PhysBody* pbody = nullptr;

	b2Vec2 newPos;

private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture = nullptr;
	const char* texturePath = nullptr;
	SDL_Texture* texture2 = nullptr;
	const char* texture2Path = nullptr;
	SDL_Texture* shadowTexture = nullptr;
	const char* shadowTexturePath = nullptr;

	SDL_RendererFlip fliped = SDL_FLIP_NONE;
	
	b2Vec2 velocity;
	b2Vec2 startPos;

	int width;
	int height;

	bool isFliped = false;
	bool changeTexture = false;
};

#endif // __PLAYER_H__