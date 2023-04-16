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

public:
	uint startSFX = 0;
	List<PartyMember*> turnList;
	List<iPoint*> positions;


private:
	SDL_Texture* img = nullptr;

	const char* imgPath;
	const char* musicPath;
	const char* startSFXPath;

	// Declare a GUI Button and create it using the GuiManager
	GuiButton* attackButton18;
	GuiButton* defenseButton19;
	GuiButton* turnJumpButton20;
};