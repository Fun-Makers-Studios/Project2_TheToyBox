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
#include "Audio.h"

MenuManager::MenuManager() : Module()
{
	name.Create("menumanager");
}

MenuManager::~MenuManager() {}

bool MenuManager::Awake(pugi::xml_node& config)
{
	menuTitle = new MenuTitle();
	menuSettings = new MenuSettings();
	menuCredits = new MenuCredits();
	menuPause = new MenuPause();
	menuQuest = new MenuQuest();
	menuParty = new MenuParty();
	menuFight = new MenuFight();

	AddMenu(menuTitle, config);
	AddMenu(menuSettings, config);
	AddMenu(menuCredits, config);
	AddMenu(menuPause, config);
	AddMenu(menuQuest, config);
	AddMenu(menuParty, config);
	AddMenu(menuFight, config);


	// Properties from xml
	startSFXPath = app->configNode.child("menuManager").child("startsfx").attribute("startSFXPath").as_string();
	selectSFXPath = app->configNode.child("menuManager").child("selectsfx").attribute("selectSFXPath").as_string();
	openMenuSFXPath = app->configNode.child("menuManager").child("openMenuSFX").attribute("openMenuSFXPath").as_string();
	closeMenuSFXPath = app->configNode.child("menuManager").child("closeMenuSFX").attribute("closeMenuSFXPath").as_string();

	return true;
}

bool MenuManager::Start()
{
	exitGame = false;

	ListItem<Menu*>* control = menus.start;

	while (control != nullptr)
	{
		control->data->Start();
		control = control->next;
	}

	// Load SFX
	startSFX = app->audio->LoadFx(startSFXPath);
	selectSFX = app->audio->LoadFx(selectSFXPath);
	openMenuSFX = app->audio->LoadFx(openMenuSFXPath);
	closeMenuSFX = app->audio->LoadFx(closeMenuSFXPath);

	return true;
}

bool MenuManager::PreUpdate()
{
	if (menuState == MenuState::SWITCH)
	{
		SwitchTo(nextMenu);
		menuState = MenuState::CONTINUE;
	}

	if (currentMenu == nullptr)
	{
		SelectMenu();
		return true;
	}
	
	SetControlState(currentMenu, GuiControlState::ENABLED);
	currentMenu->PreUpdate();

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
	if (exitGame)
		return false;

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

	if (id == MenuID::MENU_NULL)
	{
		currentMenu = nullptr;
		return;
	}
	auto menu = FindMenuByID(id);
	currentMenu = menu->data;

	SetControlState(currentMenu, GuiControlState::ENABLED);
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
	if (menu == nullptr)
		return;

	ListItem<GuiControl*>* control = menu->guiControlsList.start;

	while (control != nullptr)
	{
		control->data->state = _state;
		control = control->next;
	}
}

// Sets a default menu after Switching Scene
void MenuManager::SetDefaultMenu()
{
	if (currentMenu != nullptr)
		SetControlState(currentMenu, GuiControlState::DISABLED);

	switch (app->sceneManager->currentScene->id)
	{
		case SceneID::SCENE_LOGO:
			currentMenu = nullptr;
			break;

		case SceneID::SCENE_TITLE:
			currentMenu = menuTitle;
			break;

		case SceneID::SCENE_GAME:
			currentMenu = nullptr;
			break;

		case SceneID::SCENE_FIGHT:
			currentMenu = menuFight;
			break;

		case SceneID::SCENE_ENDING:
			break;

		default:
			break;
	}
}

// Sets menu after user imput
void MenuManager::SelectMenu()
{
	switch (app->sceneManager->currentScene->id)
	{
	case SceneID::SCENE_GAME:

		if (currentMenu == nullptr)
		{
			if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
			{
				menuState = MenuState::SWITCH;
				nextMenu = MenuID::MENU_PAUSE;
			}
			if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
			{
				menuState = MenuState::SWITCH;
				nextMenu = MenuID::MENU_PARTY;
			}
			if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
			{
				menuState = MenuState::SWITCH;
				nextMenu = MenuID::MENU_QUEST;
			}
		}
		else if (currentMenu == menuPause || currentMenu == menuQuest || currentMenu == menuParty)
		{
			if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
			{
				menuState = MenuState::SWITCH;
				nextMenu = MenuID::MENU_NULL;
			}
		}
		else if (currentMenu == menuSettings)
		{
			if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
			{
				menuState = MenuState::SWITCH;
				nextMenu = MenuID::MENU_PAUSE;
			}
		}
		
		break;

	default:
		break;
	}
}
