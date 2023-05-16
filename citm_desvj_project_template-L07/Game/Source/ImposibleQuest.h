#ifndef __IMPOSIBLEQUEST_H__
#define __IMPOSIBLEQUEST_H__

#include "Quest.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class ImposibleQuest : public Quest
{
public:
	ImposibleQuest(pugi::xml_node node);
	virtual ~ImposibleQuest();

	bool Update() override;

public:

};

#endif // __IMPOSIBLEQUEST_H__