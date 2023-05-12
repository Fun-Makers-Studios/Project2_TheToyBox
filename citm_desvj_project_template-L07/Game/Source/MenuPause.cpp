#include "MenuPause.h"

#include "App.h"
#include "Menu.h"
#include "SceneManager.h"
#include "MenuManager.h"
#include "GuiManager.h"
#include "UI.h"

#include "Window.h"
#include "Audio.h"
#include "Fonts.h"
#include "Log.h"


MenuPause::MenuPause() : Menu()
{
	id = MenuID::MENU_PAUSE;

	imgPausePath = app->configNode.child("menuManager").child("imgPause").attribute("imgPausePath").as_string();
}


MenuPause::~MenuPause()
{}


bool MenuPause::Awake(pugi::xml_node& config)
{
	LOG("Loading MenuPause");
	bool ret = true;

	return ret;
}


bool MenuPause::Start()
{
	LOG("--STARTS PAUSE MENU--");

	active = false;

	// Load
	imgPause = app->tex->Load(imgPausePath);
	pauseRect = { 35, 69, 310, 555 };

	//UI
	resumeButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::RESUME, "resume", 7, { 515, 295, 252, 76 }, this);
	backToTitleButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::BACK_TO_TITLE, "back to title", 13, { 515, 375, 252, 76 }, this);
	settingsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::SETTINGS, "settings", 8, { 515, 455, 252, 76 }, this);
	saveExitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::SAVE_EXIT, "save and exit", 12, { 515, 535, 252, 76 }, this);

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


bool MenuPause::PreUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		app->audio->PlayFx(app->menuManager->closeMenuSFX);
		app->menuManager->SelectMenu();
	}
	return true;
}


bool MenuPause::Update(float dt)
{
	app->render->DrawRectangle({ 0, 0, app->render->camera.w, app->render->camera.w }, 0, 0, 0, 128, true, false, true);

	app->render->DrawTexture(imgPause,
		app->render->camera.x + app->render->camera.w / 2 - pauseRect.w / 2,
		app->render->camera.y + app->render->camera.h / 2 - pauseRect.h / 2, &pauseRect);

	return true;
}


bool MenuPause::PostUpdate()
{
	bool ret = true;

	return ret;
}


bool MenuPause::CleanUp()
{
	LOG("Freeing TITLE SCENE");

	app->tex->UnLoad(imgPause);

	//STORE IN A LIST THIS BUTTONS AND THEN CHECK HERE IF NULLPTR TO CLEAN THEM UP
	//guiControlsList.Clear();

	return true;
}

bool MenuPause::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{
	case (uint32)ControlID::RESUME:
		// HEKATE
		/*if (!app->sceneManager->sceneGame->dialogueManager->dialogueLoaded)
			app->sceneManager->sceneGame->gamePaused != app->sceneManager->sceneGame->gamePaused;*/

		//app->sceneManager->sceneGame->pauseMenu != app->sceneManager->sceneGame->pauseMenu;
		app->menuManager->menuState = MenuState::SWITCH;
		app->menuManager->nextMenu = MenuID::MENU_NULL;
		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case (uint32)ControlID::BACK_TO_TITLE:
		app->sceneManager->sceneState = SceneState::SWITCH;
		app->sceneManager->nextScene = SceneID::SCENE_TITLE;
		app->audio->PlayFx(app->menuManager->startSFX);
		break;

	case (uint32)ControlID::SETTINGS:
		app->menuManager->menuState = MenuState::SWITCH;
		app->menuManager->nextMenu = MenuID::MENU_SETTINGS;
		app->audio->PlayFx(app->menuManager->openMenuSFX);
		break;

	case (uint32)ControlID::SAVE_EXIT:
		//showSavingState = true;
		app->SaveGameRequest();
		//exitGame = !exitGame;
		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	default:
		break;

	}

	return true;
}