#include "NPC.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Log.h"
#include "Point.h"

NPC::NPC() : Entity(EntityType::NPC)
{
	name.Create("NPC");
}

NPC::~NPC() {}

bool NPC::Awake() {

	return true;
}

bool NPC::Start() {

	startPos.x = parameters.attribute("x").as_int();
	startPos.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	npcid = parameters.attribute("id").as_int();
	int character = parameters.attribute("character").as_int();

	width = 32;
	height = 32;

	idleAnim.PushBack({ (1 + character) * 32,0 * 32,32,32 });
	idleAnim.loop = false;
	idleAnim.speed = 0.0f;

	boundaries = { (position.x * 32) - 32,(position.y * 32) - 32,96,96 };

	// initilize textures
	texture = app->tex->Load(texturePath);

	currentAnim = &idleAnim;

	pbody = app->physics->CreateCircle(position.x, position.y, width / 3, bodyType::STATIC, ColliderType::NPC);

	pbody->listener = this;

	return true;
}

bool NPC::Update()
{
	currentAnim = &idleAnim;

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - (width));
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - (height / 1.5));

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x * 32, position.y * 32, &rect);
	currentAnim->Update();

	return true;
}

bool NPC::CleanUp()
{
	return true;
}

void NPC::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->cType)
	{
	
	case ColliderType::PLAYER:
		LOG("Collision UNKNOWN");
		break;
	
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}

}