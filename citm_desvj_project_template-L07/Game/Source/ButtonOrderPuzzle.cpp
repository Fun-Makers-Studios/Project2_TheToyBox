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

	//Completion event
	//LOG("ACTIVE PUZZLE");
	if (actualOrder == maxButtons)
	{
		OpenDoor();
		ret = true;
	}

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
	
	pugi::xml_node assetNode = node.child("puzzlemanager").child("puzzle");

	for (pugi::xml_node itemNode = assetNode.child("piece"); itemNode; itemNode = itemNode.next_sibling("piece"))
	{
		piece = (PuzzlePiece*)app->entityManager->CreateEntity(EntityType::PUZZLE_PIECE, itemNode);
		piece->Start();
		pieces.Add(piece);

		if (piece->pieceType == PieceType::BUTTON)
		{
			maxButtons++;
		}
	}
}

void ButtonOrderPuzzle::ButtonTriggerCheck()
{
	ListItem<PuzzlePiece*>* pieceItem;

	for (pieceItem = pieces.start; pieceItem != nullptr; pieceItem = pieceItem->next)
	{
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN &&
			app->collisions->CheckCollision(*app->sceneManager->sceneGame->player->body, pieceItem->data->boundaries))
		{
			if (pieceItem->data->pieceType == PieceType::BUTTON)
			{
				if (actualOrder == pieceItem->data->order)
				{
					pieceItem->data->activated = true;
					actualOrder++;
					LOG("BUTTONS ORDER %d", actualOrder);
					LOG("MAX BUTTONS %d", maxButtons);
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
	ListItem<PuzzlePiece*>* pieceItem;

	for (pieceItem = pieces.start; pieceItem != nullptr; pieceItem = pieceItem->next)
	{
		pieceItem->data->activated = false;
		actualOrder = 0;
	}

}

void ButtonOrderPuzzle::OpenDoor()
{
	ListItem<PuzzlePiece*>* pieceItem;

	for (pieceItem = pieces.start; pieceItem != nullptr; pieceItem = pieceItem->next)
	{
		if (pieceItem->data->pieceType == PieceType::DOOR)
		{
			app->entityManager->DestroyEntity(pieceItem->data);
		}
	}

}