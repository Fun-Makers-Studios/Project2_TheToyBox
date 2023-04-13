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

}

void SceneLogo::OnDestroy() {}

// Called before the first frame
void SceneLogo::OnActivate()
{
	LOG("Starting Scene");
	bool ret = true;

	pugi::xml_node configNode = app->GetNode();
	pugi::xml_node config = configNode.child(id.GetString());

	// Load music
	musicPath = config.child("music").attribute("path").as_string();
	app->audio->PlayMusic(musicPath);

	font_text = app->fonts->Load(config.child("texturepaths").attribute("font").as_string(), "ABCDEFGHIJKLMNOPQRSTUWYZ0123456789-= ", 1);

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

}