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
	this->type = PuzzleType::BUTTON_ORDER;
}

MirrorPuzzle::~MirrorPuzzle() {}

bool MirrorPuzzle::Update() {
	bool ret = false;

	ObjectTriggerCheck();

	//Completion event
	LOG("ACTIVE PUZZLE 2");

	return ret;
}

void MirrorPuzzle::LoadAssets(pugi::xml_node node)
{
	pieces.Clear();

	pugi::xml_node assetNode = node.child("puzzlemanager").child("puzzle");

	for (pugi::xml_node itemNode = assetNode.child("piece"); itemNode; itemNode = itemNode.next_sibling("piece"))
	{
		piece = (PuzzlePiece*)app->entityManager->CreateEntity(EntityType::PUZZLE_PIECE, itemNode);
		piece->Start();
		pieces.Add(piece);
	}
}

void MirrorPuzzle::ObjectTriggerCheck()
{
	ListItem<PuzzlePiece*>* pieceItem;

	for (pieceItem = pieces.start; pieceItem != nullptr; pieceItem = pieceItem->next)
	{
		if (app->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN &&
			app->collisions->CheckCollision(*app->sceneManager->sceneGame->player->body, pieceItem->data->boundaries))
		{
			if (pieceItem->data->pieceType == PieceType::MOV_OBJ)
			{
				
			}
		}
	}
}

void MirrorPuzzle::ResetPuzzle()
{
	ListItem<PuzzlePiece*>* pieceItem;

	for (pieceItem = pieces.start; pieceItem != nullptr; pieceItem = pieceItem->next)
	{
		pieceItem->data->activated = false;
	}

}

void MirrorPuzzle::OpenDoor()
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