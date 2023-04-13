#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "SceneManager.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

	// idle player
	idleanim.PushBack({ 0, 0, 32, 32 });
	idleanim.loop = false;
	idleanim.speed = 0.0f;
}

Player::~Player() {}

bool Player::Awake()
{
	// Get Player parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start()
{
	//initilize textures
	texture = app->tex->Load(texturePath);

	//// L07 DONE 5: Add physics to the player - initialize physics body
	//pbody = app->physics->CreateCircle(position.x+16, position.y+16, 16, bodyType::DYNAMIC);

	//// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	//pbody->listener = this; 

	//// L07 DONE 7: Assign collider type
	//pbody->ctype = ColliderType::PLAYER;

	return true;
}

bool Player::Update()
{
	//PLAYER MOVEMENT
	
	if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)) && position.y > 0)
	{
		position.y--;
	}
	else if ((app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)) && position.y < 21)
	{
		position.y++;
	}
	else if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)) && position.x > 0)
	{
		position.x--;
	}
	else if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)) && position.x < 37)
	{
		position.x++;
	}

	//WINNING SEQUENCE

	//ANIMATION STATE MACHINE

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x * 32, position.y * 32, &rect);
	currentAnim->Update();

	return true;
}

bool Player::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{
		case ColliderType::ITEM:
			LOG("Collision ITEM");
			app->audio->PlayFx(pickCoinFxId);
			break;
		case ColliderType::PLATFORM:
			LOG("Collision PLATFORM");
			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
	}
}