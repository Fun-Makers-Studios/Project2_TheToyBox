#ifndef __COLLECTQUEST_H__
#define __COLLECTQUEST_H__

#include "Quest.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class CollectQuest : public Quest
{
public:
	CollectQuest(pugi::xml_node node);
	virtual ~CollectQuest();

	bool Update() override;

public:

	int itemId;
};

#endif // __COLLECTQUEST_H__