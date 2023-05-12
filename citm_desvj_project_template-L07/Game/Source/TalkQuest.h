#ifndef __TALKQUEST_H__
#define __TALKQUEST_H__

#include "Quest.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class TalkQuest : public Quest
{
public:
	TalkQuest(pugi::xml_node node);
	virtual ~TalkQuest();

	bool Update() override;

public:

};

#endif // __TALKQUEST_H__