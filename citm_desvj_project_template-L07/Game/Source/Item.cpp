#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "SceneManager.h"
#include "Log.h"
#include "Point.h"
#include "Collisions.h"
#include "EntityManager.h"
#include "ParticleSystemManager.h"

Item::Item(pugi::xml_node parameters) : Entity(EntityType::ITEM)
{
	name.Create("item");

	// Initialize parameters from xml
	this->parameters = parameters;

	double posX = parameters.attribute("x").as_int();
	double posY = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	itemType = parameters.attribute("itemType").as_string();
	animable = parameters.attribute("animable").as_bool();
	frames = parameters.attribute("frames").as_uint();
	// Create collider body
	body = new Body();	// HEKATE cleanup
	body->type = ColliderType::ITEM;
	body->shape = ColliderShape::CIRCLE;
	body->pos = { posX, posY };
	body->r = 8;
}

Item::~Item()
{
	app->tex->UnLoad(texture);
	texture = nullptr;

	delete body;
	body = nullptr;

	app->entityManager->DestroyEntity(this);
}

bool Item::Awake()
{
	return true;
}

bool Item::Start()
{

	//initilize textures
	texture = app->tex->Load(texturePath);
	
	rect = {0, 0, 16, 16};

	if (animable) {
		for (int i = 0; i < frames; i++)
		{
			idle.PushBack({ i * 16, 0, 16, 16 });
		}
		idle.loop = true;
		idle.speed = 0.15f;
		currentAnim = &idle;
	}

	return true;
}

bool Item::Update()
{
	if (takeItem)
	{
		app->partyManager->inventory.Add(this);
		if (takeItemPS == nullptr) {
			dPoint pos = { body->pos.x, body->pos.y };
			takeItemPS = app->particleManager->CreateParticleSystem(pos, Blueprint::TAKE_ITEM);

			app->entityManager->DestroyEntity(this);
		}
		else {
			takeItemPS->TurnOff();
			takeItemPS = nullptr;
		}
	}
	else 
	{
		if (animable)
		{
			SDL_Rect rect = currentAnim->GetCurrentFrame();
			ScaleType scale = app->scaleObj->GetCurrentScale();
			app->render->DrawTexture(texture, body->pos.x - body->r, body->pos.y - body->r, &rect, SDL_FLIP_NONE, scale);
			currentAnim->Update();
		}
		else {
			ScaleType scale = app->scaleObj->GetCurrentScale();
			app->render->DrawTexture(texture, body->pos.x - body->r, body->pos.y - body->r, &rect, SDL_FLIP_NONE, scale);
		}
		
	}

	return true;
}

bool Item::PostUpdate()
{

	return true;
}

bool Item::CleanUp()
{

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