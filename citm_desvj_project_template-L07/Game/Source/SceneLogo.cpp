#include "SceneLogo.h"

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

#include "SDL/include/SDL_render.h"

#include "Defs.h"
#include "Log.h"

SceneLogo::SceneLogo() : Scene()
{
	sceneType = SceneType::ALWAYS_ACTIVE;
	id.Create("SceneLogo");
}

// Destructor
SceneLogo::~SceneLogo()
{
}

// Called before render is available
bool SceneLogo::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneLogo");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneLogo::Start()
{
	LOG("--STARTS LOGO SCENE--");

	app->render->SetBackgroundColor({ 0, 0, 0, 255 });

	/*Initialize*/
	imgPath = app->configNode.child("logo").child("backgroundimage").attribute("texturepath").as_string();
	logoFX = app->audio->LoadFx("Assets/Audio/Fx/SceneLogo/logo_screen.wav");

	/*Load*/
	img = app->tex->Load(imgPath);
	
	app->audio->PlayFx(logoFX);

	return true;
}

// Called each loop iteration
bool SceneLogo::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneLogo::Update(float dt)
{
	time++;
	frameCount--;

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		LOG("PASA A SCENE DIRECTAMENTE");
		app->sceneManager->SwitchTo("SceneGame");
		// HEKATE app->fade->FadeToBlack(this, (Module*)app->scene, 0);
	}

	fadeRatio = (float)frameCount / (float)maxFadeFrames;

	//SDL_RenderFillRect(app->render->renderer, NULL);

	if (time >= 150)
	{
		LOG("PASA A TITLE SCENE");
		app->sceneManager->SwitchTo("SceneTitle");
		// HEKATE app->fade->FadeToBlack(this, (Module*)app->titlescreen, 0);
	}

	app->render->DrawTexture(img, 0, 0, NULL);

	return true;
}

// Called each loop iteration
bool SceneLogo::PostUpdate()
{
	bool ret = true;

	/*if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;*/

	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, 0);
	SDL_RenderFillRect(app->render->renderer, NULL);

	return ret;
}

// Called before quitting
bool SceneLogo::CleanUp()
{
	LOG("Freeing LOGO SCENE");
	app->tex->UnLoad(img);


	return true;
}
