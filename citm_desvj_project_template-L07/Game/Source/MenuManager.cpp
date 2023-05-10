#include "MenuManager.h"

#include "MenuTitle.h"
#include "MenuSettings.h"
#include "MenuPause.h"
#include "MenuQuest.h"
#include "MenuParty.h"
#include "MenuFight.h"

#include "App.h"
#include "SceneManager.h"
#include "GuiManager.h"

MenuManager::MenuManager() : Module()
{
	name.Create("menumanager");
}

MenuManager::~MenuManager() {}

bool MenuManager::Awake(pugi::xml_node& config)
{
	menuTitle = new MenuTitle();
	menuSettings = new MenuSettings();
	/*menuPause = new MenuPause();
	menuQuest = new MenuQuest();
	menuParty = new MenuParty();
	menuFight = new MenuFight();*/

	AddMenu(menuTitle, config);
	AddMenu(menuSettings, config);
	/*AddMenu(menuPause, config);
	AddMenu(menuQuest, config);
	AddMenu(menuParty, config);
	AddMenu(menuFight, config);*/

	return true;
}

bool MenuManager::Start()
{
	ListItem<Menu*>* control = menus.start;

	while (control != nullptr)
	{
		control->data->Start();
		control = control->next;
	}

	return true;
}

bool MenuManager::PreUpdate()
{
	if (currentMenu)
	{
		SetControlState(currentMenu, GuiControlState::ENABLED);

		currentMenu->PreUpdate();
	}

	return true;
}

bool MenuManager::Update(float dt)
{
	if (currentMenu)
	{
		currentMenu->Update(dt);		
	}

	return true;
}

bool MenuManager::PostUpdate()
{
	if (currentMenu)
	{
		currentMenu->PostUpdate();
		app->guiManager->Draw();
	}

	return true;
}

bool MenuManager::CleanUp() { return true; }


bool MenuManager::AddMenu(Menu* menu, pugi::xml_node& config)
{
	if (menu == nullptr) return false;

	menus.Add(menu);
	menu->Awake(config);

	return true;
}


void MenuManager::SwitchTo(MenuID id)
{
	SetControlState(currentMenu, GuiControlState::DISABLED);

	auto menu = FindMenuByID(id);
	currentMenu = menu->data;
	currentMenu->Start();
}


void MenuManager::RemoveMenu(MenuID id)
{
	auto menu = FindMenuByID(id);

	delete menu;

	menus.Del(menu);
}

ListItem<Menu*>* MenuManager::FindMenuByID(MenuID id)
{
	ListItem<Menu*>* menu;

	for (menu = menus.start; menu != NULL; menu = menu->next)
	{
		if (menu->data->GetID() == id) { return menu; }
	}
}

void MenuManager::SetControlState(Menu* menu, GuiControlState _state)
{
	ListItem<GuiControl*>* control = menu->guiControlsList.start;

	while (control != nullptr)
	{
		control->data->state = GuiControlState::ENABLED;
		control = control->next;
	}
}

void MenuManager::SetDefaultMenu()
{
	switch (app->sceneManager->currentScene->id)
	{
	case SceneID::SCENE_LOGO:
		currentMenu = nullptr;
		break;

	case SceneID::SCENE_TITLE:
		currentMenu = FindMenuByID(MenuID::MENU_TITLE)->data;
		break;

	case SceneID::SCENE_GAME:
		currentMenu = nullptr;
		break;

	case SceneID::SCENE_FIGHT:
		currentMenu = FindMenuByID(MenuID::MENU_FIGHT)->data;
		break;

	case SceneID::SCENE_ENDING:
		break;

	default:
		break;
	}
}