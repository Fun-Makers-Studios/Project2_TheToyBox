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
	id = SceneID::SCENE_LOGO;

	easingLogo = new Easing(2);

	/*Initialize xml*/
	imgPath = app->configNode.child("logo").child("backgroundimage").attribute("texturepath").as_string();
	logoFX = app->audio->LoadFx("Assets/Audio/Fx/SceneLogo/logo_screen.wav");

}

// Destructor
SceneLogo::~SceneLogo()
{
	delete easingLogo;
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

	//app->render->SetBackgroundColor({ 0, 0, 0, 255 });
	easingLogo->SetFinished(false);
	
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

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		LOG("PASA A SCENE DIRECTAMENTE");
		app->sceneManager->sceneState = SceneState::SWITCH;
		app->sceneManager->nextScene = SceneID::SCENE_GAME;
	}
	//SDL_RenderFillRect(app->render->renderer, NULL);

	if (time >= 150)
	{
		LOG("PASA A TITLE SCENE");
		app->sceneManager->sceneState = SceneState::SWITCH;
		app->sceneManager->nextScene = SceneID::SCENE_TITLE;
	}

	double easedY = 0;
	// HEKATE must pass dt!
	if (!easingLogo->GetFinished())
	{
		double time = easingLogo->TrackTime(dt);
		easedY = easingLogo->EasingAnimation(400, 0, time, EasingType::EASE_OUT_ELASTIC);
	}


	app->render->DrawRectangle({ 0, 0, 1280, 720 }, 255, 255, 255, 255);

	SDL_Rect fun = { 0, 0, 480, 720 };
	app->render->DrawTexture(img, 0, 0 - easedY, &fun);

	SDL_Rect makers = { 480, 0, 800, 720 };
	app->render->DrawTexture(img, 480, 0 + easedY, &makers);

	return true;
}

// Called each loop iteration
bool SceneLogo::PostUpdate()
{
	bool ret = true;

	/*if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;*/

	return ret;
}

// Called before quitting
bool SceneLogo::CleanUp()
{
	LOG("Freeing LOGO SCENE");
	app->tex->UnLoad(img);


	return true;
}
