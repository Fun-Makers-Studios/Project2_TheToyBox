#include "MenuTabs.h"

#include "App.h"
#include "Menu.h"
#include "SceneManager.h"
#include "MenuManager.h"
#include "GuiManager.h"

#include "Window.h"
#include "Audio.h"
#include "Fonts.h"
#include "Log.h"


MenuTabs::MenuTabs() : Menu()
{
	id = MenuID::MENU_TABS;

	imgTabsPath = app->configNode.child("menuManager").child("menuTabs").attribute("texturepath").as_string();
}


MenuTabs::~MenuTabs()
{}


bool MenuTabs::Awake(pugi::xml_node& config)
{
	LOG("Loading MenuTabs");
	bool ret = true;

	return ret;
}


bool MenuTabs::Start()
{
	LOG("--STARTS TABS MENU--");

	// Load
	imgTabs = app->tex->Load(imgTabsPath);
	//pauseRect = { 35, 69, 310, 555 };

	// Animation
	popOut.PushBack({ 0, 0, 48, 224 });
	popOut.PushBack({ 48, 0, 48, 224 });
	popOut.PushBack({ 96, 0, 48, 224 });
	popOut.PushBack({ 144, 0, 48, 224 });
	popOut.PushBack({ 192, 0, 48, 224 });
	popOut.PushBack({ 240, 0, 48, 224 });
	popOut.PushBack({ 288, 0, 48, 224 });
	popOut.PushBack({ 336, 0, 48, 224 });
	popOut.PushBack({ 384, 0, 48, 224 });
	popOut.PushBack({ 432, 0, 48, 224 });

	popOut.loop = false;
	popOut.speed = 0.22f;

	currentAnim = &popOut;

	//UI
	partyButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::PARTY, "", 0, { 443, 77, 25, 26 }, this);
	questsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::QUESTS, "", 0, { 443, 116, 25, 26 }, this);
	savesButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::SAVES, "", 0, { 443, 155, 25, 26 }, this);
	settingsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::SETTINGS, "", 0, { 443, 194, 25, 26 }, this);
	creditsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::CREDITS, "", 0, { 443, 272, 25, 26 }, this);

	// Set easing finished on title buttons
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		if (control->data->id < 5)
		{
			control->data->easing->SetFinished(false);
			control->data->easing->SetTotalTime(1 + 0.2 * control->data->id);
		}

		control = control->next;
	}

	return true;
}


bool MenuTabs::PreUpdate()
{
	// Menu position

	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		GuiButton* button = dynamic_cast<GuiButton*>(control->data);

		if (app->menuManager->menuPause->menuState == MenuState::ON)
		{			
			button->bounds.x = 443;
			button->bounds.w = 25;
			button->buttonType = ButtonType::TABS_CLOSED;
		}
		else
		{
			if (button->buttonType == ButtonType::TABS_SELECTED)
			{
				button->bounds.x = 558;
				button->bounds.w = 37;
			}
			else
			{
				button->bounds.x = 558;
				button->bounds.w = 30;
				button->buttonType = ButtonType::TABS_OPEN;
			}		
		}

		control = control->next;
	}

	/*if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		app->audio->PlayFx(app->menuManager->closeMenuSFX);
		app->menuManager->SelectMenu();
	}*/

	return true;
}


bool MenuTabs::Update(float dt)
{
	rectMenu = currentAnim->GetCurrentFrame();

	currentAnim->Update();
		

	return true;
}


bool MenuTabs::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(imgTabs, 439, 77, &rectMenu, SDL_FLIP_NONE, ScaleType::UI_200, false);

	return ret;
}


bool MenuTabs::CleanUp()
{
	LOG("Freeing TITLE SCENE");

	app->tex->UnLoad(imgTabs);

	//STORE IN A LIST THIS BUTTONS AND THEN CHECK HERE IF NULLPTR TO CLEAN THEM UP
	//guiControlsList.Clear();

	return true;
}

bool MenuTabs::OnGuiMouseClickEvent(GuiControl* control)
{
	bool menuPauseOFF = false;

	switch (control->id)
	{
		case (uint32)ControlID::PARTY:
			app->menuManager->menuParty->menuState = MenuState::SWITCH_ON;
			app->menuManager->menuQuest->menuState = MenuState::SWITCH_OFF;
			app->menuManager->menuSettings->menuState = MenuState::SWITCH_OFF;
			app->menuManager->menuCredits->menuState = MenuState::SWITCH_OFF;
			menuPauseOFF = true;
			app->audio->PlayFx(app->menuManager->selectSFX);
			break;

		case (uint32)ControlID::QUESTS:
			app->menuManager->menuParty->menuState = MenuState::SWITCH_OFF;
			app->menuManager->menuQuest->menuState = MenuState::SWITCH_ON;
			app->menuManager->menuSettings->menuState = MenuState::SWITCH_OFF;
			app->menuManager->menuCredits->menuState = MenuState::SWITCH_OFF;
			menuPauseOFF = true;
			app->audio->PlayFx(app->menuManager->startSFX);
			break;

		case (uint32)ControlID::SAVES:
			//app->menuManager->menuSave->menuState = MenuState::SWITCH_ON;
			menuPauseOFF = true;
			app->audio->PlayFx(app->menuManager->startSFX);
			break;

		case (uint32)ControlID::SETTINGS:
			app->menuManager->menuParty->menuState = MenuState::SWITCH_OFF;
			app->menuManager->menuQuest->menuState = MenuState::SWITCH_OFF;
			app->menuManager->menuSettings->menuState = MenuState::SWITCH_ON;
			app->menuManager->menuCredits->menuState = MenuState::SWITCH_OFF;
			menuPauseOFF = true;
			app->audio->PlayFx(app->menuManager->openMenuSFX);
			break;

		case (uint32)ControlID::CREDITS:
			app->menuManager->menuParty->menuState = MenuState::SWITCH_OFF;
			app->menuManager->menuQuest->menuState = MenuState::SWITCH_OFF;
			app->menuManager->menuSettings->menuState = MenuState::SWITCH_OFF;
			app->menuManager->menuCredits->menuState = MenuState::SWITCH_ON;
			menuPauseOFF = true;
			app->audio->PlayFx(app->menuManager->selectSFX);
			break;

		default:
			break;
	}

	if (menuPauseOFF && app->menuManager->menuPause->menuState == MenuState::ON)
	{
		app->menuManager->menuPause->menuState = MenuState::SWITCH_OFF;
	}

	return true;
}