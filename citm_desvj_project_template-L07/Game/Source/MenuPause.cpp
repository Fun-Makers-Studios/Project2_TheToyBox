#include "MenuPause.h"

#include "App.h"
#include "Menu.h"
#include "SceneManager.h"
#include "MenuManager.h"
#include "GuiManager.h"

#include "Window.h"
#include "Audio.h"
#include "Fonts.h"
#include "Log.h"


MenuPause::MenuPause() : Menu()
{
	id = MenuID::MENU_PAUSE;

	imgPausePath = app->configNode.child("menuManager").child("menuPause").attribute("texturepath").as_string();
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

	// Load
	imgPause = app->tex->Load(imgPausePath);
	pauseRect = { 0, 0, 247, 330 };

	//UI
	resumeButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::RESUME, "resume", 7, { 515, 295, 252, 76 }, this);
	backToTitleButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::BACK_TO_TITLE, "back to title", 13, { 515, 375, 252, 76 }, this);
	settingsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::SETTINGS, "settings", 8, { 515, 455, 252, 76 }, this);
	saveExitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::SAVE_EXIT, "save and exit", 12, { 515, 535, 252, 76 }, this);

	return true;
}


bool MenuPause::PreUpdate()
{
	/*if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		app->audio->PlayFx(app->menuManager->closeMenuSFX);
		app->menuManager->SelectMenu();
	}*/
	return true;
}


bool MenuPause::Update(float dt)
{
	// Lower music volume
	if (app->musicValue >= 20)
		Mix_VolumeMusic(20);

	return true;
}


bool MenuPause::PostUpdate()
{
	bool ret = true;

	/*if (!control->data->easing->GetFinished())
	{
		double time = easing->TrackTime(dt);
		double endPosX = control->data->boundsReset.x;
		double endPosY = control->data->boundsReset.y;
		double easedX = control->data->easing->EasingAnimation(endPosX + 200, endPosX, time, EasingType::EASE_OUT_ELASTIC);

		control->data->bounds.x = easedX;
	}*/

	app->render->DrawTexture(imgPause, 197, 13, &pauseRect, SDL_FLIP_NONE, ScaleType::UI_200, false);

	/*app->render->DrawTexture(imgPause,
		app->render->camera.x + app->render->camera.w / 2 - pauseRect.w / 2,
		app->render->camera.y + app->render->camera.h / 2 - pauseRect.h / 2,
		&pauseRect, SDL_FLIP_NONE, ScaleType::UI_200, false);*/


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
		menuState = MenuState::SWITCH_OFF;
		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case (uint32)ControlID::BACK_TO_TITLE:
		app->sceneManager->sceneState = SceneState::SWITCH;
		app->sceneManager->nextScene = SceneID::SCENE_TITLE;
		app->audio->PlayFx(app->menuManager->startSFX);
		break;

	case (uint32)ControlID::SAVE_EXIT:
		//showSavingState = true;
		app->SaveGameRequest();
		app->audio->PlayFx(app->menuManager->selectSFX);
		app->menuManager->exitGame = true;
		break;

	default:
		break;

	}

	return true;
}