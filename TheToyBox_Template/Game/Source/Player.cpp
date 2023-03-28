#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Log.h"
#include "Point.h"
#include "Scene.h"

#define FACING_LEFT false
#define FACING_RIGHT true
#define IDLE_STATIC 1
#define IDLE 2
#define RUNNING 3
#define JUMPING 4
#define FALLING 5
#define CHARGING 6
#define DYING 7
#define INITCHARGING 8
#define ENDCHARGING 9
#define WINING 10
#define ATTACKING 11
#define SPECIAL 12


Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

	// idle player
	idleanim.PushBack({ 0, 0, 32, 32 });
	idleanim.loop = false;
	idleanim.speed = 0.0f;

}

Player::~Player() {

}

bool Player::Awake() {
	// Get Player parameters from XML
	tile.x = parameters.attribute("x").as_int();
	tile.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	// initilize textures
	texture = app->tex->Load(texturePath);

	currentAnim = &idleanim;

	return true;
}

bool Player::Update()
{
	//PLAYER MOVEMENT
	if (app->scene->questMenu) {}
	else if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)) && tile.y > 0)
	{
		tile.y --;
	}
	else if ((app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)) && tile.y < 21)
	{
		tile.y ++;
	}
	else if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)) && tile.x > 0)
	{
		tile.x --;
	}
	else if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)) && tile.x < 37)
	{
		tile.x ++;
	}
	
	//WINNING SEQUENCE
	
	//ANIMATION STATE MACHINE

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, tile.x * 32, tile.y * 32, &rect);
	currentAnim->Update();

	return true;
}

bool Player::PostUpdate() {

	return true;
}

bool Player::CleanUp()
{
	return true;
}
