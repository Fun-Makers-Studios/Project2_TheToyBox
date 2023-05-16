#ifndef __GOTOQUEST_H__
#define __GOTOQUEST_H__

#include "Quest.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class GoToQuest : public Quest
{
public:
	GoToQuest(pugi::xml_node node);
	virtual ~GoToQuest();

	bool Update() override;

public:

};

#endif // __GOTOQUEST_H__