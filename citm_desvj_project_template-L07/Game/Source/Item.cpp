#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Collisions.h"
#include "EntityManager.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");
}

Item::~Item() {}

bool Item::Awake()
{
	return true;
}

bool Item::Start()
{
	double posX = parameters.attribute("x").as_int();
	double posY = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	iType = parameters.attribute("iType").as_string();

	width = 32;
	height = 32;

	//initilize textures
	texture = app->tex->Load(texturePath);
	
	if (iType == "life")
		lifeRect = {0, 0, 32, 32};
	
	if (iType == "potionhp")
		lifeRect = {0, 0, 16, 16};

	// Add a physics to an item - initialize the physics body
	body->type = ColliderType::ITEM;
	body->shape = ColliderShape::CIRCLE;
	body->pos = { posX, posY };
	body->r = 16;

	return true;
}

bool Item::Update()
{
	// Link item's texture with pbody when moving

	if (isPicked == true)
	{
		// HEKATE app->physics->world->DestroyBody(pbody->body);
		app->entityManager->DestroyEntity(this);
		
		//CleanUp();
	}

	if (app->scene->gamePaused != true)
	{
		// HEKATE position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - (width / 4));
		// HEKATE position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - (height / 4));
	}

	if (isPicked == false) {

		app->render->DrawTexture(texture, body->pos.x, body->pos.y, &lifeRect);

	}

	return true;
}

bool Item::PostUpdate()
{

	return true;
}

bool Item::CleanUp()
{
	app->tex->UnLoad(texture);
	texture = nullptr;

	// HEKATE app->physics->world->DestroyBody(pbody->body);
	// HEKATE delete pbody;
	// HEKATE pbody = nullptr;
	app->entityManager->DestroyEntity(this);

	return true;
}

void Item::OnCollision()
{
	//switch (physB->cType)
	//{
	//case ColliderType::PLAYER:
	//	LOG("Collision PLAYER");
	//	
	//	/*pbody->body->SetActive(false);
	//	this->Disable();*/
	//	isPicked = true;
	//	break;
	//}

}

void Item::ResetItem()
{
	// HEKATE
	/*SDL_SetTextureAlphaMod(texture, 1);
	pbody->body->SetActive(true);
	isPicked = false;*/
}