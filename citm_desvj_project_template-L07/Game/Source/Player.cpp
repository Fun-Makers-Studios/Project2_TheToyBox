#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Map.h"
#include "EntityManager.h"
#include "ParticleSystemManager.h"

#include <math.h>

Player::Player(pugi::xml_node parameters) : Entity(EntityType::PLAYER)
{
	name.Create("Player");

	// Initialize parameters from xml
	this->parameters = parameters;
	startPos.x = parameters.attribute("x").as_int();
	startPos.y = parameters.attribute("y").as_int();

	texturePath = parameters.attribute("texturepath").as_string();
	shadowTexturePath = parameters.attribute("shadowtexture").as_string();

	jumpSFXPath = app->configNode.child("player").child("SFXset").attribute("jumpSFXPath").as_string();
	dieSFXPath = app->configNode.child("player").child("SFXset").attribute("dieSFXPath").as_string();
	hurtSFXPath = app->configNode.child("player").child("SFXset").attribute("hurtSFXPath").as_string();
	pickLifeSFXPath = app->configNode.child("player").child("SFXset").attribute("pickLifeSFXPath").as_string();
	pickCoinSFXPath = app->configNode.child("player").child("SFXset").attribute("pickCoinSFXPath").as_string();
	levelCompletedSFXPath = app->configNode.child("player").child("SFXset").attribute("levelCompletedSFXPath").as_string();
	selectSFXPath = app->configNode.child("player").child("SFXset").attribute("selectSFXPath").as_string();
	shortRangeAttackSFXPath = app->configNode.child("player").child("SFXset").attribute("shortRangeAttackSFXPath").as_string();

	// Create collider body
	body = new Body();
	body->type = ColliderType::PLAYER;
	body->shape = ColliderShape::CIRCLE;
	body->pos = { startPos.x, startPos.y };
	body->r = 8;
}

Player::~Player()
{
	app->tex->UnLoad(texture);
	texture = nullptr;

	delete body;
	body = nullptr;

	app->entityManager->DestroyEntity(this);
}

bool Player::Awake()
{

	return true;
}

bool Player::Start()
{
	width = 32;
	height = 32;

	idle.PushBack({ 0, 0, 32, 32 });
	idle.loop = true;
	idle.speed = 0.1f;

	walkDown.PushBack({ 0, 0, 32, 32 });
	walkDown.PushBack({ 32, 0, 32, 32 });
	walkDown.PushBack({ 64, 0, 32, 32 });
	walkDown.loop = true;
	walkDown.speed = 0.15f;
	
	walkRight.PushBack({ 96, 0, 32, 32 });
	walkRight.PushBack({ 128, 0, 32, 32 });
	walkRight.PushBack({ 160, 0, 32, 32 });
	walkRight.loop = true;
	walkRight.speed = 0.15f;
	
	walkUp.PushBack({ 192, 0, 32, 32 });
	walkUp.PushBack({ 224, 0, 32, 32 });
	walkUp.PushBack({ 256, 0, 32, 32 });
	walkUp.loop = true;
	walkUp.speed = 0.15f;
	
	walkLeft.PushBack({ 288, 0, 32, 32 });
	walkLeft.PushBack({ 320, 0, 32, 32 });
	walkLeft.PushBack({ 352, 0, 32, 32 });
	walkLeft.loop = true;
	walkLeft.speed = 0.15f;

	//initilize textures
	texture = app->tex->Load(texturePath);
	shadowTexture = app->tex->Load(shadowTexturePath);

	// Loading the set of SFX, BETTER HERE FOR ENABLE/DISABLE
	jumpSFX = app->audio->LoadFx(jumpSFXPath);
	dieSFX = app->audio->LoadFx(dieSFXPath);
	hurtSFX = app->audio->LoadFx(hurtSFXPath);
	pickCoinSFX = app->audio->LoadFx(pickCoinSFXPath);
	pickLifeSFX = app->audio->LoadFx(pickLifeSFXPath);
	levelCompletedSFX = app->audio->LoadFx(levelCompletedSFXPath);
	selectSFX = app->audio->LoadFx(selectSFXPath);
	shortRangeAttackSFX = app->audio->LoadFx(shortRangeAttackSFXPath);

	currentAnim = &idle;
	dead = false;

	newPos = { 0, 0 };


	// HEKATE pbody->mapZone = MapZone::PLAYER;


	return true;
}

bool Player::PreUpdate()
{

	return true;
}

bool Player::Update()
{
	int scale = app->scaleObj->ScaleTypeToInt(app->scaleObj->GetCurrentScale());
	double speed = std::min(6 * app->GetDT() / 16 / scale, 18.0f);

	currentAnim = &idle;

	if (app->scene->gamePaused != true) 
	{

		if (godMode)
		{
			body->vel = { 0, 0 };
			// HEKATE pbody->body->SetGravityScale(0);

			// Fly around the map
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
				body->vel.y = -speed;
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				body->vel.y = speed;
			}
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
				isFliped = true;
				body->vel.x = -speed;
				if (isFliped == true && fliped == SDL_FLIP_NONE) {
					fliped = SDL_FLIP_HORIZONTAL;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
				isFliped = false;
				body->vel.x = speed;
				if (isFliped == false && fliped == SDL_FLIP_HORIZONTAL) {
					fliped = SDL_FLIP_NONE;
				}
			}
			// HEKATE pbody->body->SetLinearVelocity(velocity);

		}
		else if (!godMode && !dead)
		{
			body->vel = { 0, 0 };
			// HEKATE pbody->body->SetGravityScale(0);

			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
				body->vel.y = -speed;
				currentAnim = &walkUp;
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				body->vel.y = speed;
				currentAnim = &walkDown;
			}
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
				body->vel.x = -speed;
				currentAnim = &walkLeft;
			}
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
				body->vel.x = speed;
				currentAnim = &walkRight;
			}
			
			if (currentAnim != &idle)
			{
				if (walkParticles == nullptr) {
					walkParticles = app->particleManager->CreateParticleSystem(body->pos, Blueprint::SAND);
				}
				else {
					walkParticles->TurnOff();
					walkParticles = nullptr;
				}
			}
			

			// HEKATE pbody->body->SetLinearVelocity(velocity);
			body->pos.x += body->vel.x;
			body->pos.y += body->vel.y;
		}

	}
	
	if (walkParticles != nullptr) {
		walkParticles->SetPosition(body->pos.x, body->pos.y);
	}
	
	SDL_Rect rect = currentAnim->GetCurrentFrame();
	ScaleType scaleType = app->scaleObj->GetCurrentScale();
	app->render->DrawTexture(shadowTexture, body->pos.x - 16, body->pos.y - 54, NULL, fliped, scaleType);
	app->render->DrawTexture(texture, body->pos.x - 16, body->pos.y - 24, &rect, fliped, scaleType);
	
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

void Player::OnCollision()
{
	// HEKATE
	// DO THIS ON COLLISIONS
	// Detect the type of collision

	/*
	switch (physB->mapZone)
	{

	case MapZone::HOUSE1_TO_TOWN:
		LOG("GO TO TOWN");
		app->scene->mapName = "town";
		newPos = {192, 256};
		app->scene->isMapChanging = true;
		app->scaleObj->SetCurrentScale(ScaleType::WORLD);
		break;

	case MapZone::TOWN_TO_HOUSE1:
		LOG("GO TO HOUSE BASEMENT");
		app->scene->mapName = "housebasement";
		newPos = { 640, 480 };
		app->scene->isMapChanging = true;
		app->scaleObj->SetCurrentScale(ScaleType::HOUSE);
		break;
	
	case MapZone::HOUSEBASE_TO_HOUSEFLOOR:
		LOG("GO TO HOUSE FLOOR");
		app->scene->mapName = "housefloor";
		newPos = { 960, 256 };
		app->scene->isMapChanging = true;
		app->scaleObj->SetCurrentScale(ScaleType::HOUSE);
		break;

	case MapZone::HOUSEFLOOR_TO_HOUSEFBASE:
		LOG("GO TO HOUSE BASEMENT");
		app->scene->mapName = "housebasement";
		newPos = { 672, 176 };
		app->scene->isMapChanging = true;
		app->scaleObj->SetCurrentScale(ScaleType::HOUSE);
		break;

	case MapZone::TOWN_TO_TAVERN:
		LOG("GO TO TAVERN");
		app->scene->mapName = "tavern";
		newPos = { 640, 500 };
		app->scene->isMapChanging = true;
		app->scaleObj->SetCurrentScale(ScaleType::HOUSE);
		break;

	case MapZone::TAVERN_TO_TOWN:
		LOG("GO TO TOWN");
		app->scene->mapName = "town";
		newPos = { 800, 352 };
		app->scene->isMapChanging = true;
		app->scaleObj->SetCurrentScale(ScaleType::WORLD);
		break;
	
	case MapZone::TOWN_TO_INN:
		LOG("GO TO INN");
		app->scene->mapName = "inn";
		newPos = { 960, 544 };
		app->scene->isMapChanging = true;
		app->scaleObj->SetCurrentScale(ScaleType::HOUSE);
		break;

	case MapZone::INN_TO_TOWN:
		LOG("GO TO TOWN");
		app->scene->mapName = "town";
		newPos = { 1184, 448 };
		app->scene->isMapChanging = true;
		app->scaleObj->SetCurrentScale(ScaleType::WORLD);
		break;
	
	case MapZone::UNKNOWN:
		LOG("GO TO UNKNOWN");

		break;
	}
	
	//Detect type of collision
	switch (physB->cType)
	{

	case ColliderType::ENEMY:
		LOG("Collision ENEMY");
		app->scene->FightKid();
		break;

	case ColliderType::ITEM:
		LOG("Collision ITEM");
	
		break;

	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
	*/
}


void Player::ResetPlayerPos()
{
	body->vel = { 0, 0 };
	// HEKATE
	//pbody->body->SetTransform(PIXEL_TO_METERS(startPos), 0.0f);
	dead = false;
	
	LOG("--RESETING PLAYER--");
}