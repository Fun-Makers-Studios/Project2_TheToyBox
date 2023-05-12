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
	musicPath = app->configNode.child("title").child("music").attribute("musicPath").as_string();
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
	app->debug->debug = false;

	app->map->Disable();
	app->menuManager->Enable();

	
	/*Load*/
	img = app->tex->Load(imgPath);
	titleIntroSFX = app->audio->LoadFx("Assets/Audio/Fx/SceneTitle/scene_title.wav");

	// AUDIO
	app->audio->PlayFx(titleIntroSFX);
	app->audio->PlayMusic(musicPath);


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