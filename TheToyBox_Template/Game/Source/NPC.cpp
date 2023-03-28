#include "NPC.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Log.h"
#include "Point.h"

NPC::NPC() : Entity(EntityType::NPC)
{
	name.Create("NPC");
}

NPC::~NPC() {}

bool NPC::Awake() {

	tile.x = parameters.attribute("x").as_int();
	tile.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	npcid = parameters.attribute("id").as_int();
	int character = parameters.attribute("character").as_int();

	NPCAnim.PushBack({ (1 + character) * 32,0 * 32,32,32 });
	NPCAnim.loop = false;
	NPCAnim.speed = 0.0f;

	boundaries = { (tile.x * 32) - 32,(tile.y * 32) - 32,96,96 };

	return true;
}

bool NPC::Start() {

	// initilize textures
	texture = app->tex->Load(texturePath);

	return true;
}

bool NPC::Update()
{
	SDL_Rect rect = NPCAnim.GetCurrentFrame();
	app->render->DrawTexture(texture, tile.x * 32, tile.y *32, &rect);
	NPCAnim.Update();

	return true;
}

bool NPC::CleanUp()
{
	return true;
}