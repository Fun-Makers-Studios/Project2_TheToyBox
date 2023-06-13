#include "ButtonOrderPuzzle.h"
#include "App.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "Audio.h"
#include "Input.h"
#include "Log.h"
#include "Point.h"

ButtonOrderPuzzle::ButtonOrderPuzzle(pugi::xml_node node) {

	this->orderID = node.attribute("orderID").as_int();
	this->name = node.attribute("name").as_string();
	this->nextPuzzleId = node.attribute("nextPuzzleId").as_int();
	this->map = node.attribute("map").as_string();
	this->type = PuzzleType::BUTTON_ORDER;
}

ButtonOrderPuzzle::~ButtonOrderPuzzle() {}

bool ButtonOrderPuzzle ::Update() {
	bool ret = false;

	ButtonTriggerCheck();

	CompletionEvent();

	return ret;
}

void ButtonOrderPuzzle::UnloadAssets()
{
	for (ListItem<PuzzlePiece*>* item = pieces.start; item != NULL; item = item->next)
	{
		item->data->needToDestroy = true;
	}

	pieces.Clear();

	maxButtons = 0;

}

void ButtonOrderPuzzle::LoadAssets(pugi::xml_node node)
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

				if (piece->pieceType == PieceType::BUTTON)
				{
					maxButtons++;
				}
			}
		}
	}

	
	click = app->audio->LoadFx("Assets/Audio/Fx/Puzzles/fx16.wav");
	win = app->audio->LoadFx("Assets/Audio/Fx/Puzzles/fx22.wav");
	lose = app->audio->LoadFx("Assets/Audio/Fx/Puzzles/fx7.wav");
}

void ButtonOrderPuzzle::ButtonTriggerCheck()
{
	ListItem<PuzzlePiece*>* pieceItem;
	

	for (pieceItem = pieces.start; pieceItem != nullptr; pieceItem = pieceItem->next)
	{
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN &&
			app->collisions->CheckCollision(*app->sceneManager->sceneGame->player->body, pieceItem->data->boundaries))
		{
			app->audio->PlayFx(click);
			if (pieceItem->data->pieceType == PieceType::BUTTON)
			{
				if (actualOrder == pieceItem->data->order)
				{
					pieceItem->data->activated = true;
					actualOrder++;
				}
				else
				{
					ResetPuzzle();
				}
				
			}
		}
	}
}

void ButtonOrderPuzzle::ResetPuzzle()
{
	app->audio->PlayFx(lose);
	ListItem<PuzzlePiece*>* pieceItem;

	for (pieceItem = pieces.start; pieceItem != nullptr; pieceItem = pieceItem->next)
	{
		pieceItem->data->activated = false;
		actualOrder = 1;
	}

}

void ButtonOrderPuzzle::OpenDoor()
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

bool ButtonOrderPuzzle::CompletionEvent()
{
	if (actualOrder == maxButtons)
	{
		OpenDoor();
		return true;
	}
}