#include "MirrorPuzzle.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Log.h"
#include "Point.h"

MirrorPuzzle::MirrorPuzzle(pugi::xml_node node) {
	this->orderID = node.attribute("orderID").as_int();
	this->name = node.attribute("name").as_string();
	this->nextPuzzleId = node.attribute("nextPuzzleId").as_int();
	this->itemId = node.attribute("itemId").as_int();
	this->type = PuzzleType::MIRROR;
}

MirrorPuzzle::~MirrorPuzzle() {}

bool MirrorPuzzle::Update() {
	bool ret = true;
	//Completion event
	return ret;
}