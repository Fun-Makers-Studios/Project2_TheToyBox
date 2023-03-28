#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Player.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

enum class ItemType
{
	NONE,
	RING,
};

class Item : public Entity
{
public:

	Item() {};
	Item(int tileX, int tileY, const char* textPath, EntityType entityType = EntityType::ITEM) : Entity(entityType) {
		tile.x = tileX;
		tile.y = tileY;
		isPicked = false;
		texturePath = textPath;
	}

	virtual ~Item() {};

	virtual void UseItem(Player* player) {};
	virtual void UseItem() {};

public:
	ItemType itemType;

	SDL_Texture* texture;
	const char* texturePath;

	bool isPicked;

	int itemid;
};

#endif // __ITEM_H__