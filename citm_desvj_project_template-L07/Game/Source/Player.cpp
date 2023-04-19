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
#include "ModuleFadeToBlack.h"


Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	return true;
}

bool Player::Start() {

	//L02: DONE 1: Initialize Player parameters
	startPos.x = parameters.attribute("x").as_int();
	startPos.y = parameters.attribute("y").as_int();

	texturePath = parameters.attribute("texturepath").as_string();

	width = 32;
	height = 32;

	idlePlayer.PushBack({ 0, 0, 65, 33 });
	idlePlayer.PushBack({ 64, 0, 65, 33 });
	idlePlayer.PushBack({ 128, 0, 65, 33 });
	idlePlayer.PushBack({ 192, 0, 65, 33 });
	idlePlayer.PushBack({ 256, 0, 65, 33 });
	idlePlayer.PushBack({ 319, 0, 65, 33 });
	idlePlayer.PushBack({ 384, 0, 65, 33 });
	idlePlayer.PushBack({ 448, 0, 65, 33 });
	idlePlayer.loop = true;
	idlePlayer.speed = 0.1f;

	runPlayer.PushBack({ 0, 32, 65, 33 });
	runPlayer.PushBack({ 64, 32, 65, 33 });
	runPlayer.PushBack({ 128, 32, 65, 33 });
	runPlayer.PushBack({ 192, 32, 65, 33 });
	runPlayer.PushBack({ 256, 32, 65, 33 });
	runPlayer.PushBack({ 319, 32, 65, 33 });
	runPlayer.PushBack({ 384, 32, 65, 33 });
	runPlayer.PushBack({ 448, 32, 65, 33 });
	runPlayer.loop = true;
	runPlayer.speed = 0.3f;

	attackPlayer.PushBack({ 0, 128, 65, 33 });
	attackPlayer.PushBack({ 64, 128, 65, 33 });
	attackPlayer.PushBack({ 128, 128, 65, 33 });
	attackPlayer.PushBack({ 192, 128, 65, 33 });
	attackPlayer.PushBack({ 256, 128, 65, 33 });
	attackPlayer.PushBack({ 319, 128, 65, 33 });
	attackPlayer.PushBack({ 384, 128, 65, 33 });
	attackPlayer.PushBack({ 448, 128, 65, 33 });
	attackPlayer.loop = false;
	attackPlayer.speed = 0.3f;

	diePlayer.PushBack({ 0, 192, 65, 33 });
	diePlayer.PushBack({ 64, 192, 65, 33 });
	diePlayer.PushBack({ 134, 192, 65, 33 });
	diePlayer.PushBack({ 197, 192, 65, 33 });
	diePlayer.PushBack({ 262, 192, 65, 33 });
	diePlayer.loop = false;
	diePlayer.speed = 0.1f;

	hitPlayer.PushBack({ 450, 64, 65, 33 });
	hitPlayer.loop = false;
	hitPlayer.speed = 0.1f;

	//initilize textures
	texture = app->tex->Load(texturePath);

	jumpSFXPath = app->configNode.child("player").child("SFXset").attribute("jumpSFXPath").as_string();
	dieSFXPath = app->configNode.child("player").child("SFXset").attribute("dieSFXPath").as_string();
	hurtSFXPath = app->configNode.child("player").child("SFXset").attribute("hurtSFXPath").as_string();
	pickLifeSFXPath = app->configNode.child("player").child("SFXset").attribute("pickLifeSFXPath").as_string();
	pickCoinSFXPath = app->configNode.child("player").child("SFXset").attribute("pickCoinSFXPath").as_string();
	levelCompletedSFXPath = app->configNode.child("player").child("SFXset").attribute("levelCompletedSFXPath").as_string();
	selectSFXPath = app->configNode.child("player").child("SFXset").attribute("selectSFXPath").as_string();
	shortRangeAttackSFXPath = app->configNode.child("player").child("SFXset").attribute("shortRangeAttackSFXPath").as_string();

	// Loading the set of SFX, BETTER HERE FOR ENABLE/DISABLE
	jumpSFX = app->audio->LoadFx(jumpSFXPath);
	dieSFX = app->audio->LoadFx(dieSFXPath);
	hurtSFX = app->audio->LoadFx(hurtSFXPath);
	pickCoinSFX = app->audio->LoadFx(pickCoinSFXPath);
	pickLifeSFX = app->audio->LoadFx(pickLifeSFXPath);
	levelCompletedSFX = app->audio->LoadFx(levelCompletedSFXPath);
	selectSFX = app->audio->LoadFx(selectSFXPath);
	shortRangeAttackSFX = app->audio->LoadFx(shortRangeAttackSFXPath);

	currentAnim = &idlePlayer;
	dead = false;
	lives = 3;

	newPos = { 0, 0 };

	//Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x, position.y, width / 3, bodyType::DYNAMIC, ColliderType::PLAYER);

	pbody->mapZone = MapZone::PLAYER;

	pbody->listener = this;

	return true;
}

bool Player::PreUpdate() {

	return true;
}

bool Player::Update()
{
	int scale = app->scaleObj->ScaleTypeToInt(app->scaleObj->GetCurrentScale());
	int speed = 6 / scale;

	currentAnim = &idlePlayer;

	if (app->scene->gamePaused != true) 
	{

		if (godMode == true) {

			velocity = { 0, 0 };
			pbody->body->SetGravityScale(0);

			// Fly around the map
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
				velocity.y = -5;
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				velocity.y = 5;
			}
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
				isFliped = true;
				velocity.x = -5;
				if (isFliped == true && fliped == SDL_FLIP_NONE) {
					fliped = SDL_FLIP_HORIZONTAL;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
				isFliped = false;
				velocity.x = 5;
				if (isFliped == false && fliped == SDL_FLIP_HORIZONTAL) {
					fliped = SDL_FLIP_NONE;
				}
			}
			pbody->body->SetLinearVelocity(velocity);

		}
		else if (godMode == false && dead == false)
		{
			velocity = { 0, 0 };
			pbody->body->SetGravityScale(0);

			// Fly around the map
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
				velocity.y = -5;
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				velocity.y = 5;
			}
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
				isFliped = true;
				velocity.x = -5;
				if (isFliped == true && fliped == SDL_FLIP_NONE) {
					fliped = SDL_FLIP_HORIZONTAL;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
				isFliped = false;
				velocity.x = 5;
				if (isFliped == false && fliped == SDL_FLIP_HORIZONTAL) {
					fliped = SDL_FLIP_NONE;
				}
			}
			
			pbody->body->SetLinearVelocity(velocity);
		}

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - (width));
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - (height/1.5));

	}
	else {
		pbody->body->SetAwake(false);
	}

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	ScaleType scaleType = app->scaleObj->GetCurrentScale();
	app->render->DrawTexture(texture, position.x, position.y, &rect, fliped, scaleType);
	currentAnim->Update();


	return true;
}

bool Player::PostUpdate() {

	
	return true;
}

bool Player::CleanUp()
{
	app->tex->UnLoad(texture);
	texture = nullptr;

	pbody->body->DestroyFixture(pbody->body->GetFixtureList());
	delete pbody;
	pbody = nullptr;

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision
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
	
	switch (physB->cType)
	{

	case ColliderType::ENEMY:
		LOG("Collision ENEMY");
		app->scene->FightKid();
		break;

	case ColliderType::ITEM:
		LOG("Collision ITEM");
		/*if (app->scene->item->iType == "life" && lives < 3)
		{
			app->audio->PlayFx(pickLifeSFX);
			lives += 0.5f;
		}*/
		break;

	case ColliderType::COIN:
		LOG("Collision COIN");
		/*coins++;
		app->audio->PlayFx(pickCoinSFX);*/
		LOG("COINS: %d", coins);
		break;
	
	case ColliderType::CHECKPOINT:
		LOG("Collision CHECKPOINT");
		/*if (app->scene->checkpointEnabled == false) {
			app->scene->checkpointEnabled = true;
			app->SaveGameRequest();
			app->audio->PlayFx(levelCompletedSFX);
		}*/
		//app->fade->FadeToBlack((Module*)app->scene, (Module*)app->titlescreen, 90);
		break;
	
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}

}


void Player::ResetPlayerPos() {

	pbody->body->SetAwake(true);
	velocity = { 0, 0 };
	pbody->body->SetTransform(PIXEL_TO_METERS(startPos), 0.0f);
	app->scene->cameraFixY = false;
	app->scene->cameraFixX = false;
	app->render->camera.x = 0;
	dead = false;
	
	LOG("--RESETING PLAYER--");
}