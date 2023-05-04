#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Collisions.h"
#include "ParticleSystem.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Item : public Entity
{
public:

	Item(pugi::xml_node parameters);
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	void OnCollision();

	void ResetItem();

public:

	bool takeItem = false;

	SString itemType;

private:

	SDL_Texture* texture;
	const char* texturePath;

	Animation* currentAnim = nullptr;
	Animation itemAnim;

	SDL_Rect rect;

	int timeMov = 0;

	int width;
	int height;

	ParticleSystem* takeItemPS = nullptr;
};

#endif // __ITEM_H__