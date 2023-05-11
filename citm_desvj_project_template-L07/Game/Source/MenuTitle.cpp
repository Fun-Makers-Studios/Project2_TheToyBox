#include "MenuTitle.h"

#include "App.h"
#include "Menu.h"
#include "SceneManager.h"
#include "MenuManager.h"
#include "GuiManager.h"
#include "Audio.h"
#include "Log.h"


MenuTitle::MenuTitle() : Menu()
{
	id = MenuID::MENU_TITLE;
}


MenuTitle::~MenuTitle()
{}


bool MenuTitle::Awake(pugi::xml_node& config)
{
	LOG("Loading MenuTitle");
	bool ret = true;

	return ret;
}


bool MenuTitle::Start()
{
	LOG("--STARTS TITLE MENU--");

	// Declare a GUI Button and create it using the GuiManager
	playButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "play", 5, { 965, 350, 252, 76 }, this);
	settingsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "settings", 9, { 965, 430, 252, 76 }, this);
	creditsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "credits", 8, { 965, 510, 252, 76 }, this);
	exitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "exit", 5, { 965, 590, 252, 76 }, this);

	//CHECK SAVE GAME button
	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");
	if (result != NULL)
	{
		continueButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "continue", 9, { 965, 270, 252, 76 }, this);
	}


	// Set easing finished on title buttons
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		// HEKATE CHECK OUT OF BOUNDS
		control->data->easing->SetFinished(false);
		control->data->easing->SetTotalTime(1 + 0.2 * control->data->id);
		control = control->next;
	}

	return true;
}


bool MenuTitle::PreUpdate()
{
	return true;
}


bool MenuTitle::Update(float dt)
{
	int fool = 0;
	//CHECK SAVE GAME
	
	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	if (result == NULL)
	{
		if (isSaved != false)
			continueButton->state = GuiControlState::DISABLED;

		isSaved = false;
	}
	else
	{
		if (isSaved != true)
			continueButton->state = GuiControlState::NORMAL;

		isSaved = true;
	}
	

	/*
	if (settingMenu == true)
		app->render->DrawTexture(popImg_settings, 0, 0, NULL);
	

	// Principal buttons
	playButton1->state = GuiControlState::ENABLED;
	settingsButton2->state = GuiControlState::ENABLED;
	creditsButton3->state = GuiControlState::ENABLED;
	exitButton4->state = GuiControlState::ENABLED;	
	*/

	// Easing
	ListItem<GuiControl*>* control = app->menuManager->currentMenu->guiControlsList.start;

	while (control != nullptr)
	{
		if (!control->data->easing->GetFinished())
		{
			// HEKATE must pass dt!
			double time = control->data->easing->TrackTime(dt);
			double endPosX = control->data->boundsReset.x;
			double endPosY = control->data->boundsReset.y;
			double easedX = control->data->easing->EasingAnimation(endPosX + 200, endPosX, time, EasingType::EASE_OUT_ELASTIC);

			control->data->bounds.x = easedX;
		}
		control = control->next;
	}
	

	return true;
}


bool MenuTitle::PostUpdate()
{
	bool ret = true;

	//if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	//	ret = false;

	if (exitGame == true)
		ret = false;

	return ret;
}


bool MenuTitle::CleanUp()
{
	LOG("Freeing TITLE SCENE");


	//STORE IN A LIST THIS BUTTONS AND THEN CHECK HERE IF NULLPTR TO CLEAN THEM UP
	guiControlsList.Clear();

	return true;
}

bool MenuTitle::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{
	case 5:
		// Continue button (only if "save_game.xml" exists)
		app->sceneManager->sceneState = SceneState::SWITCH;
		app->sceneManager->nextScene = SceneID::SCENE_GAME;

		app->sceneManager->sceneGame->continueGame = true;
		app->audio->PlayFx(startSFX);
		break;

	case 1:
		// Play button
		app->sceneManager->sceneState = SceneState::SWITCH;
		app->sceneManager->nextScene = SceneID::SCENE_GAME;

		app->audio->PlayFx(startSFX);
		if (remove("save_game.xml") != 0)
			LOG("Error at Deleting Save Game");
		else
			LOG("Save Game Successfully Deleted");
		break;

	case 2:
		// Settings button
		app->menuManager->SwitchTo(MenuID::MENU_SETTINGS);
		app->audio->PlayFx(menuSelectionSFX);
		break;

	case 3:
		// Credits button
		app->menuManager->SwitchTo(MenuID::MENU_CREDITS);
		app->audio->PlayFx(menuSelectionSFX);
		break;

	default:
		break;
	}

	return true;
}