#include "ButtonOrderPuzzle.h"
#include "App.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "Audio.h"
#include "Input.h"
#include "Log.h"
#include "Point.h"

ButtonOrderPuzzle::ButtonOrderPuzzle(pugi::xml_node node) {

	this->id = node.attribute("id").as_int();
	this->name = node.attribute("name").as_string();
	this->nextPuzzleId = node.attribute("nextQuestId").as_int();
	this->itemId = node.attribute("itemId").as_int();
	this->type = PuzzleType::BUTTON_ORDER;
	LoadAssets(node);
}

ButtonOrderPuzzle::~ButtonOrderPuzzle() {}

bool ButtonOrderPuzzle::Start()
{
	bool ret = true;
	//Completion event

	return ret;
}


bool ButtonOrderPuzzle ::Update() {
	bool ret = false;

	//Completion event

	if (actualOrder == 6)
	{
		ret = true;
	}


	return ret;
}

void ButtonOrderPuzzle::LoadAssets(pugi::xml_node node)
{
	pieces.Clear();
	
	for (pugi::xml_node itemNode = node.child("piece"); itemNode; itemNode = itemNode.next_sibling("piece"))
	{
		piece = (PuzzlePiece*)app->entityManager->CreateEntity(EntityType::PUZZLE_PIECE, itemNode);
		pieces.Add(piece);
	}
}

void ButtonOrderPuzzle::ButtonTriggerCheck()
{
	ListItem<PuzzlePiece*>* pieceItem;

	for (pieceItem = pieces.start; pieceItem != nullptr; pieceItem = pieceItem->next)
	{
		if (app->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN &&
			app->collisions->CheckCollision(*app->sceneManager->sceneGame->player->body, pieceItem->data->boundaries))
		{
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

	ListItem<PuzzlePiece*>* pieceItem;

	for (pieceItem = pieces.start; pieceItem != nullptr; pieceItem = pieceItem->next)
	{
		pieceItem->data->activated = false;
		actualOrder = 1;
	}


}