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
	order = parameters.attribute("id").as_int();
	pieceType = (PieceType)parameters.attribute("pieceType").as_int();

	switch (pieceType)
	{
	case PieceType::BUTTON:

		// Create body
		body = new Body();	// HEKATE cleanup
		body->type = ColliderType::BUTTON;
		body->shape = ColliderShape::CIRCLE;
		body->pos = { startPos.x, startPos.y };
		body->r = 8;

		boundaries = *this->body;
		boundaries.r = PIECE_BOUNDARY;

		break;

	case PieceType::MOV_OBJ:

		// Create body
		body = new Body();	// HEKATE cleanup
		body->type = ColliderType::MOVABLEOBJ;
		body->shape = ColliderShape::RECTANGLE;
		body->pos = { startPos.x, startPos.y };
		body->w = 32;
		body->h = 32;

		break;

	case PieceType::DOOR:

		// Create body
		body = new Body();	// HEKATE cleanup
		body->type = ColliderType::WALL;
		body->shape = ColliderShape::RECTANGLE;
		body->pos = { startPos.x, startPos.y };
		body->w = 32;
		body->h = 96;

		break;

	case PieceType::UNKNOWN:

		break;

	default:
		break;
	}

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

	if (pieceType == PieceType::DOOR)
	{
		idleAnim.PushBack({ 0, 0, 32, 96 });
		idleAnim.loop = true;
		idleAnim.speed = 0.1f;
	}
	else
	{
		idleAnim.PushBack({ 0, 0, 32, 32 });
		idleAnim.loop = true;
		idleAnim.speed = 0.1f;
	}
	

	if (pieceType != PieceType::DOOR)
	{
		activeAnim.PushBack({ 0 + (32 * order), 0, 32, 32 });
		activeAnim.loop = true;
		activeAnim.speed = 0.1f;
	}

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

	if (activated && pieceType == PieceType::BUTTON)
	{
		
		currentAnim = &activeAnim;
	
	}

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	ScaleType scale = app->scaleObj->GetCurrentScale();
	app->render->DrawTexture(texture, body->pos.x - rect.w / 2, body->pos.y - rect.h / 2, &rect, SDL_FLIP_NONE, scale);

	currentAnim->Update();

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
