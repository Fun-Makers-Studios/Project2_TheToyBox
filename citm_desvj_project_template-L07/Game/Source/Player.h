#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Collisions.h"
#include "ParticleSystem.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player(pugi::xml_node parameters);
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	void OnCollision();

	void ResetPlayerPos();

public:

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnim = nullptr;

	//Set of animations
	Animation idle;
	Animation walkDown;
	Animation walkRight;
	Animation walkUp;
	Animation walkLeft;

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

	dPoint newPos;

private:

	// Textures
	SDL_Texture* texture = nullptr;
	const char* texturePath = nullptr;
	SDL_Texture* shadowTexture = nullptr;
	const char* shadowTexturePath = nullptr;

	ParticleSystem* walkParticles;

	SDL_RendererFlip fliped = SDL_FLIP_NONE;
	
	dPoint startPos;

	int width;
	int height;

	bool isFliped = false;
};

#endif // __PLAYER_H__