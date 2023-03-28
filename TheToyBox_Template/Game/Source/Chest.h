#ifndef __CHEST_H__
#define __CHEST_H__

#include "Item.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Chest : public Item
{
public:
	Chest(int posX, int posY, const char* textPath);
	virtual ~Chest();

	bool Update() override;
	void Draw();
};

#endif // __CHEST_H__