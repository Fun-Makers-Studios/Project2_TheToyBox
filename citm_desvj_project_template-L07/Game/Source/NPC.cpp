#include "NPC.h"
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
#include "Debug.h"

NPC::NPC(pugi::xml_node parameters) : Entity(EntityType::NPC)
{
	name.Create("NPC");

	// Load parameters from xml
	this->parameters = parameters;
	startPos.x = parameters.attribute("x").as_int();
	startPos.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	shadowTexturePath = parameters.attribute("shadowtexturepath").as_string();
	npcid = parameters.attribute("id").as_int();
	dialogueid = parameters.attribute("dialogueid").as_int();

	// Create body
	body = new Body();	// HEKATE cleanup
	body->type = ColliderType::NPC;
	body->shape = ColliderShape::CIRCLE;
	body->pos = { startPos.x, startPos.y };
	body->r = 16;
}

NPC::~NPC()
{
	app->tex->UnLoad(texture);

	delete body;
	body = nullptr;

	app->entityManager->DestroyEntity(this);
}

bool NPC::Awake()
{

	return true;
}

bool NPC::Start()
{	
	int character = parameters.attribute("character").as_int();

	width = 32;
	height = 32;

	idleAnim.PushBack({character * 32, 0, 32, 32 });
	idleAnim.loop = true;
	idleAnim.speed = 0.1f;

	texture = app->tex->Load(texturePath);
	shadowTexture = app->tex->Load(shadowTexturePath);

	currentAnim = &idleAnim;

	boundaries = { (int)startPos.x, (int)startPos.y,96,96 };

	return true;
}

bool NPC::PreUpdate()
{

	return true;
}

bool NPC::Update()
{
	currentAnim = &idleAnim;

	// HEKATE

	boundaries.x = body->pos.x - ((boundaries.w / 2) - (width / 2));
	boundaries.y = body->pos.y - ((boundaries.h / 2) - (height / 2));

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	ScaleType scale = app->scaleObj->GetCurrentScale();
	app->render->DrawTexture(shadowTexture, body->pos.x, body->pos.y -30, NULL, fliped, scale);
	app->render->DrawTexture(texture, body->pos.x, body->pos.y, &rect, SDL_FLIP_NONE, scale);
	currentAnim->Update();

	this->DialogTriggerCheck();

	return true;
}

bool NPC::PostUpdate()
{
	if (app->debug->debug)
		app->render->DrawRectangle(boundaries, 255, 0, 0, 255U, false);
	
	return true;
}

bool NPC::CleanUp()
{

	return true;
}

void NPC::OnCollision()
{
	// HEKATE
	/*switch (physB->cType)
	{
	
	case ColliderType::PLAYER:
		LOG("Collision UNKNOWN");
		break;
	
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}*/

}

void NPC::DialogTriggerCheck()
{
	if ((app->scene->player->body->pos.x + (65 / 2) >= boundaries.x) &&
		(app->scene->player->body->pos.x + (65 / 2) < boundaries.x + boundaries.w) &&
		(app->scene->player->body->pos.y + (33 / 2) >= boundaries.y) &&
		(app->scene->player->body->pos.y + (33 / 2) < boundaries.y + boundaries.h) &&
		(app->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN))
	{
		if (this->dialogueid != -1) {
			app->scene->dialogueManager->Load(this->dialogueid);
		}
	}
}