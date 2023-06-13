#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Collisions.h"
#include "ParticleSystem.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

struct ItemData {
	SString name;
	const char* texturePath;
	SString itemType;
	bool animable = false;
	uint frames;
	bool isStackeable = false;
};

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

	uint GetInventoryPos() { return inventoryPos; }
	void SetInventoryPos(uint id) { inventoryPos = id; }

public:

	bool takeItem = false;

	ItemData itemData;

	uint itemStackQuantity = 1;

	SDL_Texture* texture;

private:

	Animation* anim = nullptr;
	Animation idle;

	Animation* currentAnim = nullptr;
	Animation itemAnim;

	SDL_Rect rect;

	int timeMov = 0;

	ParticleSystem* takeItemPS = nullptr;

	uint inventoryPos = 0;

	int taken;
};

#endif // __ITEM_H__