#include "App.h"
#include "SceneLogo.h"
#include "Input.h"
#include "Fonts.h"
#include "Textures.h"
#include "Audio.h"
#include "Window.h"
#include "Render.h"

#include "Defs.h"
#include "Log.h"

SceneLogo::SceneLogo() : Scene()
{
	id.Create("sceneLogo");
}

// Destructor
SceneLogo::~SceneLogo()
{}

void SceneLogo::OnCreate(pugi::xml_node& config)
{
	LOG("Loading SceneLogo");
	bool ret = true;

	// Load music
	musicPath = config.child("music").attribute("path").as_string();
	logoPath = config.child("textures").attribute("logo").as_string();
	font_text = app->fonts->Load(config.child("texturepaths").attribute("font").as_string(), "ABCDEFGHIJKLMNOPQRSTUWYZ0123456789-= ", 1);

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	debug = false;
}

void SceneLogo::OnDestroy() {}

// Called before the first frame
void SceneLogo::OnActivate()
{
	LOG("Starting Scene");
	bool ret = true;

	pugi::xml_node configNode = app->GetNode();
	pugi::xml_node config = configNode.child(id.GetString());

	app->audio->PlayMusic(musicPath);
	logoImg = app->tex->Load("Assets/Textures/SceneLogo/logo.png");

	debug = false;
}

void SceneLogo::OnDeactivate() {}

// Called each loop iteration
void SceneLogo::PreUpdate()
{
}

// Called each loop iteration
void SceneLogo::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		if (debug) { debug = false; }
		else if (!debug) { debug = true; }
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->sceneManager->SwitchTo("sceneTest");
	}

	app->render->DrawTexture(logoImg, 0, 0, NULL);
}

// Called each loop iteration
void SceneLogo::PostUpdate()
{
	//if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	//flag for esc exit
}

// Called before quitting
void SceneLogo::CleanUp()
{
	LOG("Freeing scene");
	app->tex->Unload(logoImg);
}