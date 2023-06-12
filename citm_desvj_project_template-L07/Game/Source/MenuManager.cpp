#include "MenuManager.h"

#include "MenuTitle.h"
#include "MenuPause.h"
#include "MenuTabs.h"
#include "MenuParty.h"
#include "MenuQuest.h"
#include "MenuSettings.h"
#include "MenuFight.h"

#include "App.h"
#include "SceneManager.h"
#include "GuiManager.h"
#include "Audio.h"
#include "Fonts.h"

MenuManager::MenuManager() : Module()
{
	name.Create("menumanager");
}

MenuManager::~MenuManager() {}

bool MenuManager::Awake(pugi::xml_node& config)
{
	menuTitle = new MenuTitle();
	menuTabs = new MenuTabs();
	menuPause = new MenuPause();	
	menuParty = new MenuParty();
	menuQuest = new MenuQuest();
	menuSettings = new MenuSettings();
	menuCredits = new MenuCredits();
	menuFight = new MenuFight();

	AddMenu(menuTitle, config);
	AddMenu(menuTabs, config);
	AddMenu(menuPause, config);
	AddMenu(menuParty, config);	
	AddMenu(menuQuest, config);
	AddMenu(menuSettings, config);
	AddMenu(menuCredits, config);
	AddMenu(menuFight, config);


	/*Initialize from xml*/
	startSFXPath = app->configNode.child("menuManager").child("startsfx").attribute("startSFXPath").as_string();
	selectSFXPath = app->configNode.child("menuManager").child("selectsfx").attribute("selectSFXPath").as_string();
	openMenuSFXPath = app->configNode.child("menuManager").child("openMenuSFX").attribute("openMenuSFXPath").as_string();
	closeMenuSFXPath = app->configNode.child("menuManager").child("closeMenuSFX").attribute("closeMenuSFXPath").as_string();
	
	font1Path = app->configNode.child("menuManager").child("font1").attribute("texturepath").as_string();
	font2Path = app->configNode.child("menuManager").child("font2").attribute("texturepath").as_string();
	font2_RedPath = app->configNode.child("menuManager").child("font2Red").attribute("texturepath").as_string();
	font3Path = app->configNode.child("menuManager").child("font3").attribute("texturepath").as_string();
	font4Path = app->configNode.child("menuManager").child("font4").attribute("texturepath").as_string();

	return true;
}

bool MenuManager::Start()
{
	exitGame = false;

	ListItem<Menu*>* control = menus.start;

	// Load SFX
	startSFX = app->audio->LoadFx(startSFXPath);
	selectSFX = app->audio->LoadFx(selectSFXPath);
	openMenuSFX = app->audio->LoadFx(openMenuSFXPath);
	closeMenuSFX = app->audio->LoadFx(closeMenuSFXPath);

	//Load fonts
	char lookupTableFont1[] = { "! @,_./0123456789$;<&?abcdefghijklmnopqrstuvwxyz" };
	font1_id = app->fonts->Load(font1Path, lookupTableFont1, 2);

	char lookupTableFont2[] = { "! %&'()*+,-./0123456789:;<=>abcdefghijklmnopqrstuvwxyz" };
	font2_id = app->fonts->Load(font2Path, lookupTableFont2, 1);

	char lookupTableFont2Red[] = { "! %&'()*+,-./0123456789:;<=>abcdefghijklmnopqrstuvwxyz" };
	font2Red_id = app->fonts->Load(font2_RedPath, lookupTableFont2Red, 1);

	char lookupTableFont3[] = { "abcdefghijklmnopqrstuvwxyz 0123456789.,;:$#'! /?%&()@ -+=      " };
	font3_id = app->fonts->Load(font3Path, lookupTableFont3, 7);

	char lookupTableFont4[] = { "abcdefghijklmnopqrstuvwxyz0123456789" };
	font4_id = app->fonts->Load(font4Path, lookupTableFont4, 1);

	while (control != nullptr)
	{
		control->data->Start();
		control = control->next;
	}

	return true;
}

bool MenuManager::PreUpdate()
{
	SelectMenu();

	ListItem<Menu*>* menuItem = menus.start;

	while (menuItem != nullptr)
	{
		switch (menuItem->data->menuState)
		{
			case MenuState::SWITCH_ON:
				SetControlState(menuItem->data, GuiControlState::ENABLED);
				SetSelectedTab(menuItem->data->id, ButtonType::TABS_SELECTED);
				menuItem->data->menuState = MenuState::ON;

			case MenuState::ON:
				menuItem->data->PreUpdate();
				break;

			case MenuState::SWITCH_OFF:
				SetControlState(menuItem->data, GuiControlState::DISABLED);
				SetSelectedTab(menuItem->data->id, ButtonType::TABS_OPEN);
				menuItem->data->menuState = MenuState::OFF;
									
			case MenuState::OFF:
				break;
			
			default:
				break;
		}

		menuItem = menuItem->next;
	}

	
	return true;
}

bool MenuManager::Update(float dt)
{
	ListItem<Menu*>* menuItem = menus.start;

	while (menuItem != nullptr)
	{
		if (menuItem->data->menuState == MenuState::ON)
		{
			menuItem->data->Update(dt);
		}

		menuItem = menuItem->next;
	}

	return true;
}

bool MenuManager::PostUpdate()
{
	if (exitGame)
		return false;

	// Black BG
	if (menuTabs->menuState == MenuState::ON)
	{
		app->render->DrawRectangle({ 0, 0, app->render->camera.w, app->render->camera.w }, 0, 0, 0, 128, true, false, ScaleType::UI_100);
	}

	ListItem<Menu*>* menuItem = menus.start;

	while (menuItem != nullptr)
	{
		if (menuItem->data->menuState == MenuState::ON)
		{
			menuItem->data->PostUpdate();
		}

		menuItem = menuItem->next;
	}

	app->guiManager->Draw();

	return true;
}

bool MenuManager::CleanUp()
{
	// HEKATE Audio Cleanup?

	app->fonts->UnLoad(font1_id);
	app->fonts->UnLoad(font2_id);
	app->fonts->UnLoad(font2Red_id);
	app->fonts->UnLoad(font3_id);
	app->fonts->UnLoad(font4_id);

	return true;
}

bool MenuManager::AddMenu(Menu* menu, pugi::xml_node& config)
{
	if (menu == nullptr) return false;

	menus.Add(menu);
	menu->Awake(config);

	return true;
}

void MenuManager::SwitchTo(MenuID id)
{
	/*SetControlState(currentMenu, GuiControlState::DISABLED);

	if (id == MenuID::MENU_NULL)
	{
		currentMenu = nullptr;
		return;
	}
	auto menu = FindMenuByID(id);
	currentMenu = menu->data;

	SetControlState(currentMenu, GuiControlState::ENABLED);*/
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
	// Disable all controls
	ListItem<Menu*>* menuItem = menus.start;

	while (menuItem != nullptr)
	{
		if (menuItem->data->menuState == MenuState::ON)
		{
			menuItem->data->menuState = MenuState::SWITCH_OFF;
		}

		menuItem = menuItem->next;
	}

	// Enable Default Menu controls for currentScene
	switch (app->sceneManager->currentScene->id)
	{
		case SceneID::SCENE_TITLE:
			menuTitle->menuState = MenuState::SWITCH_ON;
			break;

		case SceneID::SCENE_FIGHT:
			menuFight->menuState = MenuState::SWITCH_ON;
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

			if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
			{
				if (menuPause->menuState == MenuState::ON)
				{
					menuPause->menuState = MenuState::SWITCH_OFF;
					menuTabs->menuState = MenuState::SWITCH_OFF;
				}
				else if (menuTabs->menuState == MenuState::OFF)
				{
					menuPause->menuState = MenuState::SWITCH_ON;				
					menuTabs->menuState = MenuState::SWITCH_ON;
					menuTabs->currentAnim->Reset();
				}
				else if (menuTabs->menuState == MenuState::ON)
				{
					// Disable all controls
					ListItem<Menu*>* menuItem = menus.start;

					while (menuItem != nullptr)
					{
						if (menuItem->data->menuState == MenuState::ON)
						{
							menuItem->data->menuState = MenuState::SWITCH_OFF;
						}

						menuItem = menuItem->next;
					}
				}
			}
			else if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
			{
				menuParty->menuState = MenuState::SWITCH_ON;

				if (menuTabs->menuState == MenuState::OFF)
				{
					menuTabs->menuState = MenuState::SWITCH_ON;
				}
			}
			else if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
			{
				menuQuest->menuState = MenuState::SWITCH_ON;

				if (menuTabs->menuState == MenuState::OFF)
				{
					menuTabs->menuState = MenuState::SWITCH_ON;
				}
			}
			
			break;

		default:
			break;
	}
}

void MenuManager::SetSelectedTab(MenuID menuID, ButtonType buttonType)
{
	switch (menuID)
	{
		case MenuID::MENU_PARTY:	app->menuManager->menuTabs->partyButton->buttonType = buttonType;		break;	
		case MenuID::MENU_QUEST:	app->menuManager->menuTabs->questsButton->buttonType = buttonType;		break;
		case MenuID::MENU_SAVE:		app->menuManager->menuTabs->savesButton->buttonType = buttonType;		break;
		case MenuID::MENU_SETTINGS: app->menuManager->menuTabs->settingsButton->buttonType = buttonType;	break;
		case MenuID::MENU_CREDITS:	app->menuManager->menuTabs->creditsButton->buttonType = buttonType;		break;
		default: break;
	}
}

std::string MenuManager::MenuIDToString(MenuID menuID)
{
	switch (menuID)
	{
		case MenuID::MENU_TITLE: return "title";
		case MenuID::MENU_TABS: return "tabs";
		case MenuID::MENU_PAUSE: return "pause";
		case MenuID::MENU_PARTY: return "party";
		case MenuID::MENU_QUEST: return "quest";
		case MenuID::MENU_SAVE: return "save";
		case MenuID::MENU_SETTINGS: return "settings";
		case MenuID::MENU_CREDITS: return "credits";
		case MenuID::MENU_FIGHT: return "fight";
		case MenuID::MENU_NULL: return "null";
		default: break;
	}
}