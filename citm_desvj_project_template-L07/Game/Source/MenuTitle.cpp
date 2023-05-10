#include "MenuTitle.h"

#include "App.h"
#include "Menu.h"
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
	playButton1 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "play", 5, { 965, 350, 252, 76 }, this);
	settingsButton2 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "settings", 9, { 965, 430, 252, 76 }, this);
	creditsButton3 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "credits", 8, { 965, 510, 252, 76 }, this);
	exitButton4 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "exit", 5, { 965, 590, 252, 76 }, this);

	//CHECK SAVE GAME button
	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");
	if (result != NULL)
	{
		continueButton5 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "continue", 9, { 965, 270, 252, 76 }, this);
	}


	// Set easing finished on title buttons
	ListItem<GuiControl*>* control = app->guiManager->guiControlsList.start;

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


bool MenuTitle::PreUpdate()
{
	return true;
}


bool MenuTitle::Update(float dt)
{
	//CHECK SAVE GAME
	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	if (result == NULL)
	{
		if (isSaved != false)
			continueButton5->state = GuiControlState::DISABLED;

		isSaved = false;
	}
	else
	{
		if (isSaved != true)
			continueButton5->state = GuiControlState::NORMAL;

		isSaved = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		LOG("PASA A GAME SCENE");
		app->sceneManager->SwitchTo(SceneID::SCENE_GAME);
		app->audio->PlayFx(startSFX);
	}

	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		app->render->viewGUIbounds = !app->render->viewGUIbounds;
		app->audio->PlayFx(selectSFX);
	}

	app->render->DrawTexture(img, 0, 0, NULL);

	if (settingMenu == true)
		app->render->DrawTexture(popImg_settings, 0, 0, NULL);

	//Draw GUI
	app->guiManager->Draw();

	// Principal buttons
	playButton1->state = GuiControlState::ENABLED;
	settingsButton2->state = GuiControlState::ENABLED;
	creditsButton3->state = GuiControlState::ENABLED;
	exitButton4->state = GuiControlState::ENABLED;


	// Setting Menu
	decreaseMusicButton8->state = GuiControlState::DISABLED;
	increaseMusicButton9->state = GuiControlState::DISABLED;
	decreaseSFXButton10->state = GuiControlState::DISABLED;
	increaseSFXButton11->state = GuiControlState::DISABLED;
	fullscreenButton12->state = GuiControlState::DISABLED;
	vsyncButton13->state = GuiControlState::DISABLED;

	if (settingMenu == true)
	{
		if (continueButton5 != nullptr && continueButton5->state != GuiControlState::DISABLED)
			continueButton5->state = GuiControlState::DISABLED;

		playButton1->state = GuiControlState::DISABLED;
		settingsButton2->state = GuiControlState::DISABLED;
		creditsButton3->state = GuiControlState::DISABLED;
		exitButton4->state = GuiControlState::DISABLED;

		decreaseMusicButton8->state = GuiControlState::NORMAL;
		increaseMusicButton9->state = GuiControlState::NORMAL;
		decreaseSFXButton10->state = GuiControlState::NORMAL;
		increaseSFXButton11->state = GuiControlState::NORMAL;
		fullscreenButton12->state = GuiControlState::NORMAL;
		vsyncButton13->state = GuiControlState::NORMAL;

		char music[10];
		sprintf_s(music, 10, "%d", app->musicValue);
		app->fonts->BlitText(630, 245, app->ui->font1_id, music);

		char sfx[10];
		sprintf_s(sfx, 10, "%d", app->sfxValue);
		app->fonts->BlitText(630, 362, app->ui->font1_id, sfx);

		char fullscreen[10];
		sprintf_s(fullscreen, 10, "%s", app->win->fullscreenMode ? "on" : "off");
		app->fonts->BlitText(632, 458, app->ui->font1_id, fullscreen);

		char vsync[10];
		sprintf_s(vsync, 10, "%s", app->render->limitFPS ? "on" : "off");
		app->fonts->BlitText(632, 565, app->ui->font1_id, vsync);

		//Close settings menu
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			settingMenu = !settingMenu;
			app->audio->PlayFx(closemenuSFX);
		}

	}

	// Credits Menu
	if (creditsMenu == true)
	{
		if (continueButton5 != nullptr && continueButton5->state != GuiControlState::DISABLED)
			continueButton5->state = GuiControlState::DISABLED;

		playButton1->state = GuiControlState::DISABLED;
		settingsButton2->state = GuiControlState::DISABLED;
		creditsButton3->state = GuiControlState::DISABLED;
		exitButton4->state = GuiControlState::DISABLED;

		app->render->DrawTexture(popImg_credits, 0, 0, NULL);

		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			creditsMenu = !creditsMenu;
			app->audio->PlayFx(closemenuSFX);
		}
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

	app->tex->UnLoad(img);
	app->tex->UnLoad(popImg_settings);
	app->tex->UnLoad(popImg_credits);


	//STORE IN A LIST THIS BUTTONS AND THEN CHECK HERE IF NULLPTR TO CLEAN THEM UP
	app->guiManager->guiControlsList.Clear();

	return true;
}

bool MenuTitle::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{
	case 5:
		// Continue button (only if "save_game.xml" exists)
		app->sceneManager->SwitchTo(SceneID::SCENE_GAME);

		// HEKATE app->fade->FadeToBlack(this, (Module*)app->scene, 90);
		app->sceneManager->sceneGame->continueGame = true;
		app->audio->PlayFx(startSFX);
		break;

	case 1:
		// Play button
		app->sceneManager->SwitchTo(SceneID::SCENE_GAME);

		// HEKATE app->fade->FadeToBlack(this, (Module*)app->scene, 90);
		app->audio->PlayFx(startSFX);
		if (remove("save_game.xml") != 0)
			LOG("Error at Deleting Save Game");
		else
			LOG("Save Game Successfully Deleted");
		break;

	case 2:
	case 6:
		// Settings button
		settingMenu = !settingMenu;
		if (settingMenu == false)
		{
			if (continueButton5 != nullptr)
				continueButton5->state = GuiControlState::NORMAL;
			playButton1->state = GuiControlState::NORMAL;
			settingsButton2->state = GuiControlState::NORMAL;
			creditsButton3->state = GuiControlState::NORMAL;
			exitButton4->state = GuiControlState::NORMAL;
		}
		app->audio->PlayFx(menuSelectionSFX);
		break;

	case 3:
	case 7:
		// Credits button
		creditsMenu = !creditsMenu;
		if (creditsMenu == false)
		{
			if (continueButton5 != nullptr)
				continueButton5->state = GuiControlState::NORMAL;
			playButton1->state = GuiControlState::NORMAL;
			settingsButton2->state = GuiControlState::NORMAL;
			creditsButton3->state = GuiControlState::NORMAL;
			exitButton4->state = GuiControlState::NORMAL;
		}
		app->audio->PlayFx(menuSelectionSFX);
		break;

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

	case 12:
		// Fullscreen button
		app->win->fullscreenMode = !app->win->fullscreenMode;
		if (app->win->fullscreenMode == true)
		{
			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
		}
		else if (app->win->fullscreenMode == false)
		{
			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_SHOWN);
		}
		app->audio->PlayFx(menuSelectionSFX);
		break;

	case 13:
		// V-Sync button
		app->render->limitFPS = !app->render->limitFPS;
		app->audio->PlayFx(menuSelectionSFX);
		break;

	case 4:
		// Exit button
		exitGame = !exitGame;
		app->audio->PlayFx(menuSelectionSFX);
		break;


	default:
		break;
	}

	return true;
}