#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "EntityManager.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");
}

Item::~Item() {}

bool Item::Awake() {

	return true;
}

bool Item::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
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

	// L07 TODO 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircleSensor(position.x, position.y, width/3, bodyType::KINEMATIC, ColliderType::ITEM);

	pbody->listener = this;

	return true;
}

bool Item::Update()
{
	// Link item's texture with pbody when moving

	if (isPicked == true)
	{
		app->physics->world->DestroyBody(pbody->body);
		app->entityManager->DestroyEntity(this);
		
		//CleanUp();
	}

	if (app->scene->gamePaused != true)
	{
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - (width / 4));
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - (height / 4));
	}

	if (isPicked == false) {

		app->render->DrawTexture(texture, position.x, position.y, &lifeRect);

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

	app->physics->world->DestroyBody(pbody->body);
	delete pbody;
	pbody = nullptr;
	app->entityManager->DestroyEntity(this);

	return true;
}

void Item::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->cType)
	{
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		
		/*pbody->body->SetActive(false);
		this->Disable();*/
		isPicked = true;
		break;
	}

}

void Item::ResetItem() {
	SDL_SetTextureAlphaMod(texture, 1);
	pbody->body->SetActive(true);
	isPicked = false;
}