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

	Item();
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void ResetItem();

public:

	bool isPicked = false;

	Body* body;
	SString iType;

private:

	SDL_Texture* texture;
	const char* texturePath;

	SDL_Rect lifeRect;
	
	dPoint velocity;

	int timeMov = 0;

	int width;
	int height;

};

#endif // __ITEM_H__