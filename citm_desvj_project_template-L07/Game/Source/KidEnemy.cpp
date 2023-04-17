#include "KidEnemy.h"
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
#include "PathFinding.h"
#include "ModuleFadeToBlack.h"
#include "EntityManager.h"


KidEnemy::KidEnemy() : Entity(EntityType::FLYING_ENEMY)
{
	name.Create("Bat");
}

KidEnemy::~KidEnemy() {

}

bool KidEnemy::Awake() {

	return true;
}

bool KidEnemy::Start() {

	//L02: DONE 1: Initialize Player parameters
	startPos.x = parameters.attribute("x").as_int();
	startPos.y = parameters.attribute("y").as_int();

	texturePath = parameters.attribute("texturepath").as_string();


	width = 32;
	height = 32;

	idleAnim.PushBack({ 192, 0, 32, 64 });
	idleAnim.loop = true;
	idleAnim.speed = 0.1f;

	//initilize textures
	texture = app->tex->Load(texturePath);

	//Initialize SFX
	stompSFXPath = app->configNode.child("scene").child("slimesfx").attribute("stompSFXPath").as_string();		// same as slime
	powerUpSFXPath = app->configNode.child("scene").child("slimesfx").attribute("powerUpSFXPath").as_string();	// same as slime
	batHitSFXPath = app->configNode.child("scene").child("batsfx").attribute("batHitSFXPath").as_string();

	// Loading the set of SFX, BETTER HERE FOR ENABLE/DISABLE
	stompSFX = app->audio->LoadFx(stompSFXPath);
	powerUpSFX = app->audio->LoadFx(powerUpSFXPath);
	batHitSFX = app->audio->LoadFx(batHitSFXPath);

	currentAnim = &idleAnim;
	dead = false;

	position.x = startPos.x;
	position.y = startPos.y;

	//Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x, position.y, width/2, bodyType::STATIC, ColliderType::ENEMY);

	pbody->listener = this;

	return true;
}

bool KidEnemy::PreUpdate() {

	return true;
}

bool KidEnemy::Update()
{
	
	currentAnim = &idleAnim;
	

	if (dead == true)
	{
		//Destroy entity
		app->entityManager->DestroyEntity(this);
		app->physics->world->DestroyBody(pbody->body);
		app->audio->PlayFx(powerUpSFX);
		dead = false;
	}

	if (app->scene->gamePaused != true)
	{
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - (width/2));
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - (height));
	}

		SDL_Rect rect = currentAnim->GetCurrentFrame();
		ScaleType scale = app->scaleObj->GetCurrentScale();
		app->render->DrawTexture(texture, position.x, position.y, &rect, fliped, scale);
		currentAnim->Update();

	return true;
}

bool KidEnemy::PostUpdate() {


	return true;
}

bool KidEnemy::CleanUp()
{

	app->tex->UnLoad(texture);
	texture = nullptr;

	app->physics->world->DestroyBody(pbody->body);
	delete pbody;
	pbody = nullptr;
	app->entityManager->DestroyEntity(this);


	return true;
}

void KidEnemy::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->cType)
	{
	
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}

}

void KidEnemy::ResetBat() {

	pbody->body->SetSleepingAllowed(false);


}