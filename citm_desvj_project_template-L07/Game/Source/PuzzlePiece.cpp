#include "PuzzlePiece.h"
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
#include "PathFinding.h"
#include "EntityManager.h"
#include "Debug.h"

PuzzlePiece::PuzzlePiece(pugi::xml_node parameters) : Entity(EntityType::PUZZLE_PIECE)
{
	name.Create("puzzlepiece");

	// Load parameters from xml
	this->parameters = parameters;
	startPos.x = parameters.attribute("x").as_int();
	startPos.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	pieceID = parameters.attribute("id").as_int();
	pieceType = (PieceType)parameters.attribute("pieceType").as_int();

	// Create body
	body = new Body();	// HEKATE cleanup
	body->type = ColliderType::BUTTON;
	body->shape = ColliderShape::CIRCLE;
	body->pos = { startPos.x, startPos.y };
	body->r = 8;

	boundaries = *this->body;
	boundaries.r = PIECE_BOUNDARY;

}

PuzzlePiece::~PuzzlePiece()
{
	app->tex->UnLoad(texture);

	delete body;
	body = nullptr;

	app->entityManager->DestroyEntity(this);
}

bool PuzzlePiece::Awake()
{

	return true;
}

bool PuzzlePiece::Start()
{

	width = 32;
	height = 32;

	idleAnim.PushBack({ 0, 0, 32, 32 });
	idleAnim.loop = true;
	idleAnim.speed = 0.1f;
	
	activeAnim.PushBack({ 0 + (32 * pieceID), 0, 32, 32 });
	activeAnim.loop = true;
	activeAnim.speed = 0.1f;

	currentAnim = &idleAnim;

	texture = app->tex->Load(texturePath);

	return true;
}

bool PuzzlePiece::PreUpdate()
{

	return true;
}

bool PuzzlePiece::Update()
{
	currentAnim = &idleAnim;

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	ScaleType scale = app->scaleObj->GetCurrentScale();
	//app->render->DrawTexture(shadowTexture, body->pos.x - rect.w / 2, body->pos.y - 54, NULL, fliped, scale);

	if (activated) 
	{
		
		currentAnim = &activeAnim;
	
	}

	app->render->DrawTexture(texture, body->pos.x - rect.w / 2, body->pos.y - 24, &rect, SDL_FLIP_NONE, scale);

	currentAnim->Update();

	//TriggerCheck();

	return true;
}

bool PuzzlePiece::PostUpdate()
{
	if (app->debug->debug)
		app->render->DrawCircle(body->pos.x, body->pos.y, boundaries.r, 0, 0, 255, 255, false);

	return true;
}

bool PuzzlePiece::CleanUp()
{
	return true;
}

//void PuzzlePiece::TriggerCheck()
//{
//	/*boundaries = *this->body;
//	boundaries.r = 30;
//
//	if (app->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN &&
//		app->collisions->CheckCollision(*app->sceneManager->sceneGame->player->body, boundaries))
//	{
//		activated = true;
//	}*/
//}