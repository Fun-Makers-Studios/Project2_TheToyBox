#ifndef __MENU_H__
#define __MENU_H__

#include "App.h"


enum class MenuID
{
	MENU_TITLE,

	MENU_TABS,
	
	MENU_PAUSE,
	MENU_PARTY,
	MENU_QUEST,
	MENU_SAVE,
	MENU_SETTINGS,
	MENU_CREDITS,

	MENU_FIGHT,
	MENU_NULL
};

enum class MenuState
{
	ON,
	OFF,
	SWITCH_ON,
	SWITCH_OFF,
};

class GuiControl;

class Menu
{
public:

	Menu() {}

	virtual ~Menu() {}

	// Called when scene initially created. Called once.
	virtual bool Awake(pugi::xml_node& config) = 0;

	// Called whenever a scene is transitioned into. Can be 
	// called many times in a typical game cycle.
	virtual bool Start() { return true; };

	// Called before all Updates
	virtual bool PreUpdate() { return true; };

	// Called each loop iteration
	virtual bool Update(float dt) { return true; };

	// Called before all Updates
	virtual bool PostUpdate() { return true; };

	// Called before quitting
	virtual bool CleanUp() { return true; };

	virtual bool OnGuiMouseClickEvent(GuiControl* control) { return true; }

	// Returns the scene's ID
	virtual MenuID GetID() { return id; }

	// Sets the scene's ID
	virtual void SetID(MenuID id) { this->id = id; }


public:

	MenuID id;
	MenuState menuState = MenuState::OFF;
	List<GuiControl*> guiControlsList;
};

#endif // __MENU_H__