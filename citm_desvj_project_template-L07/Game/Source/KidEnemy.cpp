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


KidEnemy::KidEnemy(pugi::xml_node parameters) : Entity(EntityType::ENEMY_KID)
{
	name.Create("Bat");

	// Initialize parameters from xml
	this->parameters = parameters;
	startPos.x = parameters.attribute("x").as_int();
	startPos.y = parameters.attribute("y").as_int();

	texturePath = parameters.attribute("texturepath").as_string();

	stompSFXPath = app->configNode.child("scene").child("slimesfx").attribute("stompSFXPath").as_string();
	powerUpSFXPath = app->configNode.child("scene").child("slimesfx").attribute("powerUpSFXPath").as_string();
	batHitSFXPath = app->configNode.child("scene").child("batsfx").attribute("batHitSFXPath").as_string();

	// Create collider body
	body = new Body();
	body->type = ColliderType::ENEMY;
	body->shape = ColliderShape::CIRCLE;
	body->pos = { startPos.x, startPos.y };
	body->r = 14;
}

KidEnemy::~KidEnemy()
{
	app->tex->UnLoad(texture);

	delete body;
	body = nullptr;

	app->entityManager->DestroyEntity(this);
}

bool KidEnemy::Awake()
{

	return true;
}

bool KidEnemy::Start()
{
	//set var
	dead = false;

	body->pos.x = startPos.x;
	body->pos.y = startPos.y;

	//Animations
	idleAnim.PushBack({ 192, 0, 32, 64 });
	idleAnim.loop = true;
	idleAnim.speed = 0.1f;
	currentAnim = &idleAnim;

	// Load texure
	texture = app->tex->Load(texturePath);

	// Load sfx
	stompSFX = app->audio->LoadFx(stompSFXPath);
	powerUpSFX = app->audio->LoadFx(powerUpSFXPath);
	batHitSFX = app->audio->LoadFx(batHitSFXPath);


	return true;
}

bool KidEnemy::PreUpdate()
{

	return true;
}

bool KidEnemy::Update()
{
	
	currentAnim = &idleAnim;
	

	if (dead == true)
	{
		//HEKATE
		//Destroy entity
		app->entityManager->DestroyEntity(this);	
		//app->physics->world->DestroyBody(pbody->body);
		app->audio->PlayFx(powerUpSFX);
		dead = false;
	}

	if (app->scene->gamePaused != true)
	{
		//HEKATE position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - (width/2));
		//HEKATE position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - (height));
	}

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	ScaleType scale = app->scaleObj->GetCurrentScale();
	app->render->DrawTexture(texture, body->pos.x - rect.w / 2, body->pos.y - 50, &rect, fliped, scale);
	currentAnim->Update();

	return true;
}

bool KidEnemy::PostUpdate()
{


	return true;
}

bool KidEnemy::CleanUp()
{

	return true;
}

void KidEnemy::OnCollision()
{
	/*switch (physB->cType)
	{
	
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}*/

}

void KidEnemy::ResetBat()
{

	//HEKATE pbody->body->SetSleepingAllowed(false);

}