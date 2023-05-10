#ifndef __MENU_MANAGER_H__
#define __MENU_MANAGER_H__

#include "App.h"
#include "Menu.h"
#include "MenuTitle.h"


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

public:

	Menu* currentMenu = nullptr;

	List<Menu*> menus;

	//Menus
	MenuTitle* menuTitle;

};


#endif // __MENU_MANAGER_H__