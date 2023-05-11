#include "MenuCredits.h"

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


MenuCredits::MenuCredits() : Menu()
{
	id = MenuID::MENU_CREDITS;

	popImgCreditsPath = app->configNode.child("menuManager").child("popImage").attribute("creditstexturepath").as_string();
}


MenuCredits::~MenuCredits()
{}


bool MenuCredits::Awake(pugi::xml_node& config)
{
	LOG("Loading MenuPause");
	bool ret = true;

	return ret;
}


bool MenuCredits::Start()
{
	LOG("--STARTS PAUSE MENU--");

	// Load
	popImg_credits = app->tex->Load(popImgCreditsPath);

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


bool MenuCredits::PreUpdate()
{
	return true;
}


bool MenuCredits::Update(float dt)
{
	app->render->DrawTexture(popImg_credits, 0, 0, NULL);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		app->audio->PlayFx(app->menuManager->closeMenuSFX);
		app->menuManager->SetDefaultMenu();
	}

	return true;
}


bool MenuCredits::PostUpdate()
{
	bool ret = true;

	//if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	//	ret = false;

	/*if (exitGame == true)
		ret = false;*/

	return ret;
}


bool MenuCredits::CleanUp()
{
	LOG("Freeing MenuCredits");

	app->tex->UnLoad(popImg_credits);

	//STORE IN A LIST THIS BUTTONS AND THEN CHECK HERE IF NULLPTR TO CLEAN THEM UP
	//guiControlsList.Clear();

	return true;
}

bool MenuCredits::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{
	case 8:
		// Decrease music volume
		app->musicValue = app->musicValue - 1;
		if (app->musicValue <= 0)
			app->musicValue = 0;
		if (app->musicValue >= 100)
			app->musicValue = 100;
		Mix_VolumeMusic(app->musicValue);
		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case 9:
		// Increase music volume
		app->musicValue = app->musicValue + 1;
		if (app->musicValue <= 0)
			app->musicValue = 0;
		if (app->musicValue >= 100)
			app->musicValue = 100;
		Mix_VolumeMusic(app->musicValue);
		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case 10:
		// Decrease SFX volume
		app->sfxValue = app->sfxValue - 1;
		if (app->sfxValue <= 0)
			app->sfxValue = 0;
		if (app->sfxValue >= 100)
			app->sfxValue = 100;
		Mix_Volume(-1, app->sfxValue);
		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case 11:
		// Increase SFX volume
		app->sfxValue = app->sfxValue + 1;
		if (app->sfxValue <= 0)
			app->sfxValue = 0;
		if (app->sfxValue >= 100)
			app->sfxValue = 100;
		Mix_Volume(-1, app->sfxValue);
		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case (uint32)ControlID::FULL_SCREEN:
		// Fullscreen button
		app->win->fullscreenMode = !app->win->fullscreenMode;
		if (app->win->fullscreenMode)
		{
			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
		}
		else if (!app->win->fullscreenMode)
		{
			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_SHOWN);
		}
		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case (uint32)ControlID::V_SYNC:
		// V-Sync button
		app->render->limitFPS = !app->render->limitFPS;
		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	default:
		break;

	}

	return true;
}