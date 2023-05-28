#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "SceneManager.h"
#include "Log.h"
#include "Point.h"
#include "Map.h"
#include "EntityManager.h"
#include "ParticleSystemManager.h"
#include "ModuleController.h"
#include "MenuManager.h"
#include "Debug.h"

#include <math.h>

Player::Player() : Entity(EntityType::PLAYER)
{
	// Create collider body
	body = new Body();
	body->type = ColliderType::PLAYER;
	body->shape = ColliderShape::CIRCLE;
	body->pos = { NULL, NULL };
	body->r = NULL;
};

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
	walkDown.PushBack({ 0, 0, 32, 32 });
	walkDown.PushBack({ 64, 0, 32, 32 });
	walkDown.loop = true;
	walkDown.speed = 0.15f;
	
	walkRight.PushBack({ 96, 0, 32, 32 });
	walkRight.PushBack({ 128, 0, 32, 32 });
	walkRight.PushBack({ 96, 0, 32, 32 });
	walkRight.PushBack({ 160, 0, 32, 32 });
	walkRight.loop = true;
	walkRight.speed = 0.15f;
	
	walkUp.PushBack({ 192, 0, 32, 32 });
	walkUp.PushBack({ 224, 0, 32, 32 });
	walkUp.PushBack({ 192, 0, 32, 32 });
	walkUp.PushBack({ 256, 0, 32, 32 });
	walkUp.loop = true;
	walkUp.speed = 0.15f;
	
	walkLeft.PushBack({ 288, 0, 32, 32 });
	walkLeft.PushBack({ 320, 0, 32, 32 });
	walkLeft.PushBack({ 288, 0, 32, 32 });
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
	double speed;

	body->vel = { 0, 0 };
	isIdle = true;

	if (app->debug->godMode)
	{
		// HEAKTE something needed?
	}

	if (!dead && !app->sceneManager->sceneGame->isMapChanging)
	{
		if (app->menuManager->menuTabs->menuState == MenuState::OFF && !app->sceneManager->sceneGame->dialogueManager->dialogueLoaded)
		{
			//Sprint
			if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			{
				speed = std::min(5.6f * app->GetDT() / 16 / scale, 18.0f);
			}
			else
			{
				speed = std::min(4 * app->GetDT() / 16 / scale, 18.0f);
			}

			//WASD
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
				body->vel.y = -speed;
				currentAnim = &walkUp;
				isIdle = false;
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				body->vel.y = speed;
				currentAnim = &walkDown;
				isIdle = false;
			}
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
				body->vel.x = -speed;
				currentAnim = &walkLeft;
				isIdle = false;
			}
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
				body->vel.x = speed;
				currentAnim = &walkRight;
				isIdle = false;
			}	
		}

		body->pos.x += body->vel.x;
		body->pos.y += body->vel.y;
	}
	
	SDL_Rect rect = currentAnim->GetCurrentFrame();
	ScaleType scaleType = app->scaleObj->GetCurrentScale();
	app->render->DrawTexture(shadowTexture, body->pos.x - 16, body->pos.y - 54, NULL, fliped, scaleType);
	app->render->DrawTexture(texture, body->pos.x - 16, body->pos.y - 24, &rect, fliped, scaleType);
	
	if (isIdle)
		currentAnim->Reset();
	else
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

void Player::ResetPlayerPos()
{
	body->vel = { 0, 0 };
	dead = false;
	
	LOG("--RESETING PLAYER--");
}