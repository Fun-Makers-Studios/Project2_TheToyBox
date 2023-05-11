#include "MenuSettings.h"

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


MenuSettings::MenuSettings() : Menu()
{
	id = MenuID::MENU_SETTINGS;

	popImgSettingsPath = app->configNode.child("title").child("popImage").attribute("settingtexturepath").as_string();
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
	decreaseMusicButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 8, "decrease", 9, { 325, 200, 252, 76 }, this);
	increaseMusicButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, "increase", 9, { 700, 200, 252, 76 }, this);

	decreaseSFXButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "decrease", 9, { 325, 315, 252, 76 }, this);
	increaseSFXButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "increase", 9, { 700, 315, 252, 76 }, this);

	fullscreenButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ButtonID::FULL_SCREEN, "off", 4, { 520, 424, 252, 76 }, this);

	vsyncButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ButtonID::V_SYNC, "off", 4, { 520, 532, 252, 76 }, this);

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
	app->render->DrawTexture(popImg_settings, 0, 0, NULL);

	//Close settings menu
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		app->menuManager->SetDefaultMenu();
	}


	// HEKATE - GuiControl Label?
	char music[10];
	sprintf_s(music, 10, "%d", app->musicValue);
	app->fonts->BlitText(630, 245, app->ui->font1_id, music);

	char sfx[10];
	sprintf_s(sfx, 10, "%d", app->sfxValue);
	app->fonts->BlitText(630, 362, app->ui->font1_id, sfx);


	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		switch (control->data->id)
		{
			case (uint32)ButtonID::FULL_SCREEN:
			{
				if (app->win->fullscreenMode)
				{
					char fullscreen[3];
					sprintf_s(fullscreen, 3, "%s", "on");
					control->data->text = fullscreen;
				}
				else
				{
					char fullscreen[4];
					sprintf_s(fullscreen, 4, "%s", "off");
					control->data->text = fullscreen;
				}

				break;
			} 

			case (uint32)ButtonID::V_SYNC:
			{
				if (app->render->limitFPS)
				{
					char vsync[3];
					sprintf_s(vsync, 3, "%s", "on");
					control->data->text = vsync;
				}
				else
				{
					char vsync[4];
					sprintf_s(vsync, 4, "%s", "off");
					control->data->text = vsync;
				}

				break;
			}

			default:
				break;
		}
		
		control = control->next;
	}

	return true;
}


bool MenuSettings::PostUpdate()
{
	bool ret = true;

	//if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	//	ret = false;

	if (exitGame == true)
		ret = false;

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
		app->musicValue = app->musicValue - 1;
		if (app->musicValue <= 0)
			app->musicValue = 0;
		if (app->musicValue >= 100)
			app->musicValue = 100;
		Mix_VolumeMusic(app->musicValue);
		app->audio->PlayFx(menuSelectionSFX);
		break;

	case 9:
		// Increase music volume
		app->musicValue = app->musicValue + 1;
		if (app->musicValue <= 0)
			app->musicValue = 0;
		if (app->musicValue >= 100)
			app->musicValue = 100;
		Mix_VolumeMusic(app->musicValue);
		app->audio->PlayFx(menuSelectionSFX);
		break;

	case 10:
		// Decrease SFX volume
		app->sfxValue = app->sfxValue - 1;
		if (app->sfxValue <= 0)
			app->sfxValue = 0;
		if (app->sfxValue >= 100)
			app->sfxValue = 100;
		Mix_Volume(-1, app->sfxValue);
		app->audio->PlayFx(menuSelectionSFX);
		break;

	case 11:
		// Increase SFX volume
		app->sfxValue = app->sfxValue + 1;
		if (app->sfxValue <= 0)
			app->sfxValue = 0;
		if (app->sfxValue >= 100)
			app->sfxValue = 100;
		Mix_Volume(-1, app->sfxValue);
		app->audio->PlayFx(menuSelectionSFX);
		break;

	case (uint32)ButtonID::FULL_SCREEN:
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
		app->audio->PlayFx(menuSelectionSFX);
		break;

	case (uint32)ButtonID::V_SYNC:
		// V-Sync button
		app->render->limitFPS = !app->render->limitFPS;
		app->audio->PlayFx(menuSelectionSFX);
		break;

	default:
		break;
		
	}

	return true;
}