#ifndef __SCENEFIGHT_H__
#define __SCENEFIGHT_H__

#include "Scene.h"
#include "PartyManager.h"
#include "GuiButton.h"
#include "SDL/include/SDL.h"

enum class TurnStep
{
	IN,
	ACTION,
	OUT,
	FINISHED
};

class SceneFight : public Scene
{
public:

	SceneFight();
	virtual ~SceneFight();

	bool Awake(pugi::xml_node& config);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();


	void ExecuteTurn();

	// Returns true if the reciever has been killed
	bool Attack(PartyMember* turnMember_, PartyMember* receiverMember_);

	void Escape();

	void SortBySpeed();

public:

	TurnStep turnStep;

	uint turn = 0;
	List<PartyMember*> turnList;
	List<PartyMember*> enemyList;
	List<iPoint*> positions;

	uint enemySelected;

	PartyMember* turnMember = nullptr;

private:

	SDL_Texture* tex_fightTownBG = nullptr;
	SDL_Texture* tex_fightCircusBG = nullptr;
	SDL_Texture* tex_arrow = nullptr;

	const char* path_fightTownBG;
	const char* path_fightCircusBG;
	const char* path_arrow;
	const char* path_startsSfx;

	const char* musicPath = nullptr;

	uint alliesAlive;
	uint enemiesAlive;

	Easing* easingPos = nullptr;

	int scale = app->scaleObj->ScaleTypeToInt(ScaleType::FIGHT);
};

#endif //__SCENEFIGHT_H__