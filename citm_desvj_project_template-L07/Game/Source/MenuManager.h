#ifndef __MENU_MANAGER_H__
#define __MENU_MANAGER_H__

#include "App.h"
#include "Menu.h"
#include "MenuTitle.h"
#include "MenuSettings.h"
#include "MenuCredits.h"
#include "MenuPause.h"
#include "MenuQuest.h"
#include "MenuParty.h"
#include "MenuFight.h"

enum class MenuState
{
	CONTINUE,
	SWITCH
};

class MenuManager : public Module
{
public:

	MenuManager();
	~MenuManager();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	// Adds a menu to the menu manager.
	bool AddMenu(Menu* menu, pugi::xml_node& config);

	// Transitions to a menu?
	void SwitchTo(MenuID id);

	// Removes menu from menu manager.
	void RemoveMenu(MenuID id);

	ListItem<Menu*>* FindMenuByID(MenuID id);

	void SetControlState(Menu* menu, GuiControlState _state);
	void SetDefaultMenu();
	void SelectMenu();

public:

	Menu* currentMenu = nullptr;
	List<Menu*> menus;

	MenuState menuState = MenuState::CONTINUE;
	MenuID nextMenu;

	//Menus
	MenuTitle* menuTitle = nullptr;
	MenuSettings* menuSettings = nullptr;
	MenuCredits* menuCredits = nullptr;
	MenuPause* menuPause = nullptr;
	MenuQuest* menuQuest = nullptr;
	MenuParty* menuParty = nullptr;
	MenuFight* menuFight = nullptr;

	bool exitGame = false;

	//SFX
	uint startSFX = 0;
	uint selectSFX = 0;
	uint openMenuSFX = 0;
	uint closeMenuSFX = 0;

	const char* startSFXPath;
	const char* selectSFXPath;
	const char* openMenuSFXPath;
	const char* closeMenuSFXPath;

	//FONTS
	uint font1_id = -1;
	uint font2_id = -1;
	uint font2Red_id = -1;
	uint font3_id = -1;

	const char* font1Path;
	const char* font2Path;
	const char* font2_RedPath;
	const char* font3Path;
};


#endif // __MENU_MANAGER_H__