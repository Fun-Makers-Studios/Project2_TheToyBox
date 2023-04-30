#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Point.h"
#include "Collisions.h"
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

	bool isPicked = false;

	SString itemType;

private:

	SDL_Texture* texture;
	const char* texturePath;

	SDL_Rect lifeRect;

	int timeMov = 0;

	int width;
	int height;
};

#endif // __ITEM_H__