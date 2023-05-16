#ifndef __PARTYMEMBERSQUEST_H__
#define __PARTYMEMBERSQUEST_H__

#include "Quest.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

struct PMember
{
	SString name;
	bool picked;
};

class PartyMembersQuest : public Quest
{
public:
	PartyMembersQuest(pugi::xml_node node);
	virtual ~PartyMembersQuest();

	bool Update() override;

	List<PMember> memberNames;

private:
	bool optionSelected = false;
	SString currentSpeaker;
	int members = 0;
	int membersLeft = 0;
};

#endif // __PARTYMEMBERSQUEST_H__