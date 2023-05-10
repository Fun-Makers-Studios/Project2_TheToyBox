#include "SceneTitle.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Entity.h"
#include "Collisions.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "MenuManager.h"
#include "Map.h"
#include "GuiManager.h"
#include "Fonts.h"
#include "UI.h"
#include "Debug.h"

#include "SDL/include/SDL_render.h"

#include "Defs.h"
#include "Log.h"

SceneTitle::SceneTitle() : Scene()
{
	id = SceneID::SCENE_TITLE;

	/*Initialize*/
	imgPath = app->configNode.child("title").child("backgroundimage").attribute("texturepath").as_string();
	popImgSettingsPath = app->configNode.child("title").child("popImage").attribute("settingtexturepath").as_string();
	popImgCreditsPath = app->configNode.child("title").child("popImage").attribute("creditstexturepath").as_string();
	selectSFXPath = app->configNode.child("title").child("selectsfx").attribute("selectSFXPath").as_string();
	select2SFXPath = app->configNode.child("player").child("SFXset").attribute("selectSFXPath").as_string();
}

// Destructor
SceneTitle::~SceneTitle()
{}

// Called before render is available
bool SceneTitle::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneTitle");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneTitle::Start()
{
	LOG("--STARTS TITLE SCENE--");

	app->render->camera.x = 0;

	app->map->Disable();
	app->menuManager->Enable();

	
	/*Load*/
	img = app->tex->Load(imgPath);
	popImg_settings = app->tex->Load(popImgSettingsPath);
	popImg_credits = app->tex->Load(popImgCreditsPath);
	menuSelectionSFX = app->audio->LoadFx(selectSFXPath);
	selectSFX = app->audio->LoadFx(select2SFXPath);
	startSFX = app->audio->LoadFx("Assets/Audio/Fx/SceneTitle/fx5.wav");
	titleSFX = app->audio->LoadFx("Assets/Audio/Fx/SceneTitle/title_screen.wav");
	closemenuSFX = app->audio->LoadFx("Assets/Audio/Fx/SceneTitle/close_menu.wav");

	app->debug->debug = false;
	settingMenu = false;
	creditsMenu = false;
	

	// AUDIO
	app->audio->PlayFx(titleSFX);
	app->audio->PlayMusic("Assets/Audio/Music/menu.ogg");


	return true;
}

// Called each loop iteration
bool SceneTitle::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneTitle::Update(float dt)
{
	//CHECK SAVE GAME
	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	app->render->DrawTexture(img, 0, 0, NULL);

	return true;
}

// Called each loop iteration
bool SceneTitle::PostUpdate()
{
	bool ret = true;


	if (exitGame)
		ret = false;

	return ret;
}

// Called before quitting
bool SceneTitle::CleanUp()
{
	LOG("Freeing TITLE SCENE");

	app->tex->UnLoad(img);
	app->tex->UnLoad(popImg_settings);
	app->tex->UnLoad(popImg_credits);

	
	//STORE IN A LIST THIS BUTTONS AND THEN CHECK HERE IF NULLPTR TO CLEAN THEM UP
	//app->guiManager->guiControlsList.Clear();

	return true;
}

bool SceneTitle::OnGuiMouseClickEvent(GuiControl* control)
{
	//switch (control->id)
	//{
	//case 5:
	//	// Continue button (only if "save_game.xml" exists)
	//	app->sceneManager->SwitchTo(SceneID::SCENE_GAME);

	//	// HEKATE app->fade->FadeToBlack(this, (Module*)app->scene, 90);
	//	app->sceneManager->sceneGame->continueGame = true;
	//	app->audio->PlayFx(startSFX);
	//	break;

	//case 1:
	//	// Play button
	//	app->sceneManager->SwitchTo(SceneID::SCENE_GAME);

	//	// HEKATE app->fade->FadeToBlack(this, (Module*)app->scene, 90);
	//	app->audio->PlayFx(startSFX);
	//	if (remove("save_game.xml") != 0)
	//		LOG("Error at Deleting Save Game");
	//	else
	//		LOG("Save Game Successfully Deleted");
	//	break;

	//case 2:
	//case 6:
	//	// Settings button
	//	settingMenu = !settingMenu;
	//	if (settingMenu == false) 
	//	{
	//		if (continueButton5 != nullptr)
	//			continueButton5->state = GuiControlState::NORMAL;
	//		playButton1->state = GuiControlState::NORMAL;
	//		settingsButton2->state = GuiControlState::NORMAL;
	//		creditsButton3->state = GuiControlState::NORMAL;
	//		exitButton4->state = GuiControlState::NORMAL;
	//	}
	//	app->audio->PlayFx(menuSelectionSFX);
	//	break;

	//case 3:
	//case 7:
	//	// Credits button
	//	creditsMenu = !creditsMenu;
	//	if (creditsMenu == false)
	//	{
	//		if (continueButton5 != nullptr)
	//			continueButton5->state = GuiControlState::NORMAL;
	//		playButton1->state = GuiControlState::NORMAL;
	//		settingsButton2->state = GuiControlState::NORMAL;
	//		creditsButton3->state = GuiControlState::NORMAL;
	//		exitButton4->state = GuiControlState::NORMAL;
	//	}
	//	app->audio->PlayFx(menuSelectionSFX);
	//	break;

	//case 8:
	//	// Decrease music volume
	//	app->musicValue = app->musicValue - 1;
	//	if (app->musicValue <= 0)
	//		app->musicValue = 0;
	//	if (app->musicValue >= 100)
	//		app->musicValue = 100;
	//	Mix_VolumeMusic(app->musicValue);
	//	app->audio->PlayFx(menuSelectionSFX);
	//	break;

	//case 9:
	//	// Increase music volume
	//	app->musicValue = app->musicValue + 1;
	//	if (app->musicValue <= 0)
	//		app->musicValue = 0;
	//	if (app->musicValue >= 100)
	//		app->musicValue = 100;
	//	Mix_VolumeMusic(app->musicValue);
	//	app->audio->PlayFx(menuSelectionSFX);
	//	break;

	//case 10:
	//	// Decrease SFX volume
	//	app->sfxValue = app->sfxValue - 1;
	//	if (app->sfxValue <= 0)
	//		app->sfxValue = 0;
	//	if (app->sfxValue >= 100)
	//		app->sfxValue = 100;
	//	Mix_Volume(-1, app->sfxValue);
	//	app->audio->PlayFx(menuSelectionSFX);
	//	break;

	//case 11:
	//	// Increase SFX volume
	//	app->sfxValue = app->sfxValue + 1;
	//	if (app->sfxValue <= 0)
	//		app->sfxValue = 0;
	//	if (app->sfxValue >= 100)
	//		app->sfxValue = 100;
	//	Mix_Volume(-1, app->sfxValue);
	//	app->audio->PlayFx(menuSelectionSFX);
	//	break;

	//case 12:
	//	// Fullscreen button
	//	app->win->fullscreenMode = !app->win->fullscreenMode;
	//	if (app->win->fullscreenMode == true)
	//	{
	//		SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
	//	}
	//	else if(app->win->fullscreenMode == false)
	//	{
	//		SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_SHOWN);
	//	}
	//	app->audio->PlayFx(menuSelectionSFX);
	//	break;

	//case 13:
	//	// V-Sync button
	//	app->render->limitFPS = !app->render->limitFPS;
	//	app->audio->PlayFx(menuSelectionSFX);
	//	break;

	//case 4:
	//	// Exit button
	//	exitGame = !exitGame;
	//	app->audio->PlayFx(menuSelectionSFX);
	//	break;


	//default:
	//	break;
	//}

	return true;
}