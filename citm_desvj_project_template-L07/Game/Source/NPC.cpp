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

NPC::NPC() : Entity(EntityType::NPC)
{
	name.Create("NPC");
}

NPC::~NPC() {}

bool NPC::Awake()
{

	return true;
}

bool NPC::Start()
{
	startPos.x = parameters.attribute("x").as_int();
	startPos.y = parameters.attribute("y").as_int();

	texturePath = parameters.attribute("texturepath").as_string();
	shadowTexturePath = parameters.attribute("shadowtexturepath").as_string();

	npcid = parameters.attribute("id").as_int();
	dialogueid = parameters.attribute("dialogueid").as_int();
	int character = parameters.attribute("character").as_int();

	width = 32;
	height = 32;

	idleAnim.PushBack({character * 32, 0, 32, 32 });
	idleAnim.loop = true;
	idleAnim.speed = 0.1f;

	texture = app->tex->Load(texturePath);
	shadowTexture = app->tex->Load(shadowTexturePath);

	currentAnim = &idleAnim;

	//Physics
	// HEKATE
	body = new Body();
	body->type = ColliderType::NPC;
	body->shape = ColliderShape::CIRCLE;
	body->pos = { startPos.x, startPos.y };
	body->r = 16;

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
	/*position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - (width / 2));
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - (height / 2));*/

	boundaries.x = body->pos.x - ((boundaries.w / 2)-(width/2));
	boundaries.y = body->pos.y - ((boundaries.h / 2) - (height/2));

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
	app->tex->UnLoad(texture);

	// HEKATE
	/*pbody->body->DestroyFixture(pbody->body->GetFixtureList());
	app->physics->world->DestroyBody(this->pbody->body);
	delete pbody;
	pbody = nullptr;*/
	app->entityManager->DestroyEntity(this);

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
		(app->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)) {
		if (this->dialogueid != -1) {
			app->scene->dialogueManager->Load(this->dialogueid);
		}
	}
}