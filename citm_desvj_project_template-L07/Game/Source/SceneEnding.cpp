#include "SceneEnding.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Entity.h"
#include "Collisions.h"
#include "EntityManager.h"
#include "Player.h"
#include "Map.h"
#include "SceneManager.h"

#include "SDL/include/SDL_render.h"

#include "Defs.h"
#include "Log.h"

SceneEnding::SceneEnding() : Scene()
{
	sceneType = SceneType::ALWAYS_ACTIVE;
	id = SceneID::SCENE_ENDING;
}

// Destructor
SceneEnding::~SceneEnding()
{}

// Called before render is available
bool SceneEnding::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneEnding");
	bool ret = true;


	return ret;
}

// Called before the first frame
bool SceneEnding::Start()
{
	LOG("--STARTS ENDING SCENE--");

	/*Initialize*/
	imgPath = app->configNode.child("ending").child("backgroundimage").attribute("texturepath").as_string();
	musicPath = app->configNode.child("ending").child("music").attribute("musicPath").as_string();
	startSFXPath = app->configNode.child("ending").child("endingsfx").attribute("startSFXPath").as_string();
	
	/*Load*/
	img = app->tex->Load(imgPath);
	startSFX = app->audio->LoadFx(startSFXPath);
	app->audio->PlayMusic(musicPath);

	return true;
}

// Called each loop iteration
bool SceneEnding::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneEnding::Update(float dt)
{
	

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		LOG("PASA A GAME SCENE");
		app->sceneManager->SwitchTo(SceneID::SCENE_GAME);
		// HEKATE app->fade->FadeToBlack(this, (Module*)app->scene, 90);
		app->audio->PlayFx(startSFX);
	}

	return true;
}

// Called each loop iteration
bool SceneEnding::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	app->render->DrawTexture(img, app->render->camera.x, 0, NULL);

	return ret;
}

// Called before quitting
bool SceneEnding::CleanUp()
{
	LOG("Freeing ENDING SCENE");

	if (img != nullptr) {
		app->tex->UnLoad(img);
	}

	return true;
}
