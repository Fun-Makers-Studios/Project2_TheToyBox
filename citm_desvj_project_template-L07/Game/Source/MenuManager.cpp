#include "MenuManager.h"

#include "App.h"
#include "SceneManager.h"

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
	return true;
}

bool MenuManager::PreUpdate()
{
	switch (app->sceneManager->currentScene->id)
	{
		case SceneID::SCENE_LOGO:
			break;
		case SceneID::SCENE_TITLE:
			break;
		case SceneID::SCENE_GAME:
			break;
		case SceneID::SCENE_FIGHT:
			break;
		case SceneID::SCENE_ENDING:
			break;
		default:
			break;
	}

	if (currentMenu)
	{
		currentMenu->PreUpdate();
		return true;
	}
	return false;
}

bool MenuManager::Update(float dt)
{
	if (currentMenu)
	{
		currentMenu->Update(dt);
		return true;
	}
	return false;
}

bool MenuManager::PostUpdate()
{
	if (currentMenu)
	{
		currentMenu->PostUpdate();
		return true;
	}
	return false;
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