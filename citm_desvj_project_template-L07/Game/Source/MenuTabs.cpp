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

	imgTabsPath = app->configNode.child("menuManager").child("imgPause").attribute("imgPausePath").as_string();
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

	//UI
	partyButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::PARTY, "", 0, { 515, 295, 252, 76 }, this);
	questsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::QUESTS, "", 0, { 515, 375, 252, 76 }, this);
	savesButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::SAVES, "", 0, { 515, 375, 252, 76 }, this);
	settingsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::SETTINGS, "", 0, { 515, 455, 252, 76 }, this);
	creditsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::CREDITS, "", 0, { 515, 535, 252, 76 }, this);

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
	if (app->menuManager->menuPause->menuState == MenuState::ON)
	{

	}
	else
	{

	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		app->audio->PlayFx(app->menuManager->closeMenuSFX);
		app->menuManager->SelectMenu();
	}
	return true;
}


bool MenuTabs::Update(float dt)
{

	return true;
}


bool MenuTabs::PostUpdate()
{
	bool ret = true;

	app->render->DrawRectangle({ 0, 0, app->render->camera.w, app->render->camera.w }, 0, 0, 0, 128, true, false, true);

	/*app->render->DrawTexture(imgTabs,
		app->render->camera.x + app->render->camera.w / 2 - pauseRect.w / 2,
		app->render->camera.y + app->render->camera.h / 2 - pauseRect.h / 2, &pauseRect);*/


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
	switch (control->id)
	{
	case (uint32)ControlID::PARTY:
		app->menuManager->menuParty->menuState = MenuState::SWITCH_ON;
		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case (uint32)ControlID::QUESTS:
		app->menuManager->menuQuest->menuState = MenuState::SWITCH_ON;
		app->audio->PlayFx(app->menuManager->startSFX);
		break;

	case (uint32)ControlID::SAVES:
		//app->menuManager->menuSave->menuState = MenuState::SWITCH_ON;
		app->audio->PlayFx(app->menuManager->startSFX);
		break;

	case (uint32)ControlID::SETTINGS:
		app->menuManager->menuSettings->menuState = MenuState::SWITCH_ON;
		app->audio->PlayFx(app->menuManager->openMenuSFX);
		break;

	case (uint32)ControlID::CREDITS:
		app->menuManager->menuCredits->menuState = MenuState::SWITCH_ON;
		app->audio->PlayFx(app->menuManager->selectSFX);

		break;

	default:
		break;

	}

	return true;
}