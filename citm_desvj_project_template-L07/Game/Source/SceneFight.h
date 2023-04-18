#pragma once

#include "Module.h"
#include "PartyManager.h"
#include "GuiButton.h"
#include "SDL/include/SDL.h"

class SceneFight : public Module
{
public:
	SceneFight();

	// Destructor
	virtual ~SceneFight();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	void Attack(PartyMember* turnMember_, PartyMember* receiverMember_);

	void Escape();

public:
	uint startSFX = 0;
	uint turn = 0;
	List<PartyMember*> turnList;
	List<PartyMember*> enemyList;
	List<iPoint*> positions;

	uint enemySelected;

	PartyMember* turnMember = nullptr;

private:
	SDL_Texture* tex_bg = nullptr;
	SDL_Texture* tex_arrow = nullptr;

	const char* path_bg;
	const char* path_arrow;
	const char* path_startsSfx;

	// Declare a GUI Button and create it using the GuiManager
	GuiButton* attackButton18;
	GuiButton* defenseButton19;
	GuiButton* escapeButton20;

	uint alliesAlive;
	uint enemiesAlive;
};