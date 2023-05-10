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

	/*Initialize xml*/
	imgPath = app->configNode.child("title").child("backgroundimage").attribute("texturepath").as_string();
	popImgSettingsPath = app->configNode.child("title").child("popImage").attribute("settingtexturepath").as_string();
	popImgCreditsPath = app->configNode.child("title").child("popImage").attribute("creditstexturepath").as_string();
	selectSFXPath = app->configNode.child("title").child("selectsfx").attribute("selectSFXPath").as_string();
	select2SFXPath = app->configNode.child("player").child("SFXset").attribute("selectSFXPath").as_string();
}


SceneTitle::~SceneTitle()
{}


bool SceneTitle::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneTitle");
	bool ret = true;

	return ret;
}


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


bool SceneTitle::PreUpdate()
{
	return true;
}


bool SceneTitle::Update(float dt)
{
	//CHECK SAVE GAME
	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	app->render->DrawTexture(img, 0, 0, NULL);

	return true;
}


bool SceneTitle::PostUpdate()
{
	bool ret = true;


	if (exitGame)
		ret = false;

	return ret;
}


bool SceneTitle::CleanUp()
{
	LOG("Freeing TITLE SCENE");

	app->tex->UnLoad(img);
	app->tex->UnLoad(popImg_settings);
	app->tex->UnLoad(popImg_credits);

	// HEKATE Should check all scene and menu CleanUp
	 
	//STORE IN A LIST THIS BUTTONS AND THEN CHECK HERE IF NULLPTR TO CLEAN THEM UP
	//app->guiManager->guiControlsList.Clear();

	return true;
}

bool SceneTitle::OnGuiMouseClickEvent(GuiControl* control)
{
	// HEKATE should delete OnGuiMouseClickEvent on all Scenes
	return true;
}