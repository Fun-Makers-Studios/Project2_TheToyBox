#include "MirrorPuzzle.h"
#include "App.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "Audio.h"
#include "Input.h"
#include "Log.h"
#include "Point.h"

MirrorPuzzle::MirrorPuzzle(pugi::xml_node node) {

	this->orderID = node.attribute("orderID").as_int();
	this->name = node.attribute("name").as_string();
	this->nextPuzzleId = node.attribute("nextPuzzleId").as_int();
	this->map = node.attribute("map").as_string();
	this->type = PuzzleType::MIRROR;
}

MirrorPuzzle::~MirrorPuzzle() {}

bool MirrorPuzzle::Update() {
	bool ret = false;

	ObjectTriggerCheck();

	if (isAttached)
	{
		ListItem<PuzzlePiece*>* pieceItem;

		for (pieceItem = pieces.start; pieceItem != nullptr; pieceItem = pieceItem->next)
		{
			if (pieceItem->data->isAttached)
			{
				pieceItem->data->body->pos = app->sceneManager->sceneGame->player->body->pos;
				pieceItem->data->boundaries.pos = app->sceneManager->sceneGame->player->body->pos;
			}
		}
		
	}

	CompletionEvent();

	click = app->audio->LoadFx("Assets/Audio/Fx/Puzzles/fx16.wav");
	win = app->audio->LoadFx("Assets/Audio/Fx/Puzzles/fx22.wav");
	lose = app->audio->LoadFx("Assets/Audio/Fx/Puzzles/fx7.wav");
	return ret;
}

void MirrorPuzzle::UnloadAssets()
{
	LOG("UNLOAD ASSETS P2 %d", pieces.Count());
	for (ListItem<PuzzlePiece*>* item = pieces.start; item != NULL; item = item->next)
	{
		item->data->needToDestroy = true;
	}

	pieces.Clear();
	LOG("UNLOAD ASSETS P2 %d", pieces.Count());
}

void MirrorPuzzle::LoadAssets(pugi::xml_node node)
{
	pieces.Clear();

	pugi::xml_node assetNode = node.child("puzzlemanager");

	for (pugi::xml_node itemNode = assetNode.child("puzzle"); itemNode; itemNode = itemNode.next_sibling("puzzle"))
	{
		if (itemNode.attribute("orderID").as_int() == orderID)
		{
			for (pugi::xml_node pieceNode = itemNode.child("piece"); pieceNode; pieceNode = pieceNode.next_sibling("piece"))
			{
				piece = (PuzzlePiece*)app->entityManager->CreateEntity(EntityType::PUZZLE_PIECE, pieceNode);
				piece->Start();
				pieces.Add(piece);
				if (piece->pieceType == PieceType::MOV_OBJ)
				{
					maxObj++;
				}
			}
			
		}
	}
}

void MirrorPuzzle::ObjectTriggerCheck()
{
	ListItem<PuzzlePiece*>* pieceItem;

	for (pieceItem = pieces.start; pieceItem != nullptr; pieceItem = pieceItem->next)
	{
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN &&
			app->collisions->CheckCollision(*app->sceneManager->sceneGame->player->body, pieceItem->data->boundaries))
		{
			app->audio->PlayFx(click);
			if (pieceItem->data->pieceType == PieceType::MOV_OBJ && !pieceItem->data->activated)
			{
				isAttached = !isAttached;
				pieceItem->data->isAttached = !pieceItem->data->isAttached;
				LOG("IS ATTACHED: %d", isAttached);
			}

			for (ListItem<PuzzlePiece*>* piece2Item = pieces.start; piece2Item != nullptr; piece2Item = piece2Item->next)
			{
				if (pieceItem->data->pieceType == PieceType::MOV_OBJ &&
					piece2Item->data->pieceType == PieceType::TRIGGER_PLATFORM &&
					app->collisions->CheckCollision(*pieceItem->data->body, *piece2Item->data->body) &&
					!isAttached &&
					!pieceItem->data->activated)
				{
					pieceItem->data->activated = true;
					activatedObj++;
				}
			}
		}
		/*LOG("ACTIVATED OBJECTS %d", activatedObj);
		LOG("MAX OBJECTS %d", maxObj);*/
		
	}
}

void MirrorPuzzle::ResetPuzzle()
{
	app->audio->PlayFx(lose);
	ListItem<PuzzlePiece*>* pieceItem;

	for (pieceItem = pieces.start; pieceItem != nullptr; pieceItem = pieceItem->next)
	{
		pieceItem->data->isAttached = false;
		pieceItem->data->activated = false;
		pieceItem->data->body->pos = pieceItem->data->startPos;
		pieceItem->data->boundaries.pos = pieceItem->data->startPos;
	}
	isAttached = false;
}

void MirrorPuzzle::OpenDoor()
{
	app->audio->PlayFx(win);
	ListItem<PuzzlePiece*>* pieceItem;

	for (pieceItem = pieces.start; pieceItem != nullptr; pieceItem = pieceItem->next)
	{
		if (pieceItem->data->pieceType == PieceType::DOOR)
		{
			app->entityManager->DestroyEntity(pieceItem->data);
		}
	}

}

bool MirrorPuzzle::CompletionEvent()
{
	if (activatedObj == maxObj) {
		OpenDoor();
		return true;
	}
}