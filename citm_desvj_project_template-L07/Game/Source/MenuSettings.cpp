#include "MenuSettings.h"

#include "App.h"
#include "Menu.h"
#include "MenuPause.h"
#include "SceneManager.h"
#include "MenuManager.h"
#include "GuiManager.h"

#include "Window.h"
#include "Audio.h"
#include "Fonts.h"
#include "Log.h"


MenuSettings::MenuSettings() : Menu()
{
	id = MenuID::MENU_SETTINGS;

	popImgSettingsPath = app->configNode.child("menuManager").child("popImage").attribute("settingtexturepath").as_string();
}


MenuSettings::~MenuSettings()
{}


bool MenuSettings::Awake(pugi::xml_node& config)
{
	LOG("Loading MenuSettings");
	bool ret = true;

	return ret;
}


bool MenuSettings::Start()
{
	LOG("--STARTS TITLE MENU--");

	// Load
	popImg_settings = app->tex->Load(popImgSettingsPath);

	// Create UI
	fullscreenButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::FULL_SCREEN, "off", 4, { 520, 424, 252, 76 }, this);
	vsyncButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::V_SYNC, "off", 4, { 520, 532, 252, 76 }, this);

	//Sliders
	S_music = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 33, "music", 6, { 620, 241, 40, 40 }, this, ButtonType::UNKNOWN, { 377, 253, 530, 15 });
	S_music->slider = GuiSliderType::MUSIC;

	S_fx = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 34, "fx", 3,{ 620, 343, 40, 40 }, this, ButtonType::UNKNOWN, { 377, 353, 530, 15 });
	S_fx->slider = GuiSliderType::FX;
		
	// Set easing finished on title buttons
	ListItem<GuiControl*>* control =  guiControlsList.start;

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


bool MenuSettings::PreUpdate()
{
	return true;
}


bool MenuSettings::Update(float dt)
{

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		app->audio->PlayFx(app->menuManager->closeMenuSFX);
		app->menuManager->SetDefaultMenu();
	}

	
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		switch (control->data->id)
		{
			case (uint32)ControlID::FULL_SCREEN:
			{
				if (app->win->fullscreenMode)
				{
					control->data->textSize = 3;
					control->data->text = on;
				}
				else
				{
					control->data->textSize = 4;
					control->data->text = off;
				}

				break;
			} 

			case (uint32)ControlID::V_SYNC:
			{
				if (app->render->limitFPS)
				{
					control->data->textSize = 3;
					control->data->text = on;
				}
				else
				{
					control->data->textSize = 4;
					control->data->text = off;
				}

				break;
			}

			default:
				break;
		}
		
		control = control->next;
	}

	// Lower music volume
	if (app->menuManager->currentMenu == app->menuManager->menuSettings) {

		if (app->musicValue >= 20)
			Mix_VolumeMusic(20);
	}
	else
	{
		Mix_VolumeMusic(app->musicValue);
	}

	return true;
}


bool MenuSettings::PostUpdate()
{
	bool ret = true;

	app->render->DrawRectangle({ 0, 0, app->render->camera.w, app->render->camera.w }, 0, 0, 0, 128, true, false, true);

	app->render->DrawTexture(popImg_settings, app->render->camera.x, app->render->camera.y - 3, NULL);

	//if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	//	ret = false;

	/*if (exitGame == true)
		ret = false;*/

	return ret;
}


bool MenuSettings::CleanUp()
{
	LOG("Freeing TITLE SCENE");

	app->tex->UnLoad(popImg_settings);

	//STORE IN A LIST THIS BUTTONS AND THEN CHECK HERE IF NULLPTR TO CLEAN THEM UP
	//guiControlsList.Clear();

	return true;
}

bool MenuSettings::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{
		case 8:
			// Decrease music volume
			app->musicValue = app->musicValue - 5;
			if (app->musicValue <= 0)
				app->musicValue = 0;
			if (app->musicValue >= 100)
				app->musicValue = 100;
			Mix_VolumeMusic(app->musicValue);
			app->audio->PlayFx(app->menuManager->selectSFX);
			break;

		case 9:
			// Increase music volume
			app->musicValue = app->musicValue + 5;
			if (app->musicValue <= 0)
				app->musicValue = 0;
			if (app->musicValue >= 100)
				app->musicValue = 100;
			Mix_VolumeMusic(app->musicValue);
			app->audio->PlayFx(app->menuManager->selectSFX);
			break;

		case 10:
			// Decrease SFX volume
			app->sfxValue -= 5;
			if (app->sfxValue <= 0)
				app->sfxValue = 0;
			if (app->sfxValue >= 100)
				app->sfxValue = 100;
			Mix_Volume(-1, app->sfxValue);
			app->audio->PlayFx(app->menuManager->selectSFX);
			break;

		case 11:
			// Increase SFX volume
			app->sfxValue += 5;
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