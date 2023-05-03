#include "CollectQuest.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Log.h"
#include "Point.h"

CollectQuest::CollectQuest(pugi::xml_node node) {
	this->id = node.attribute("id").as_int();
	this->name = node.attribute("name").as_string();
	this->description = node.attribute("description").as_string();
	this->nextQuestId = node.attribute("nextQuestId").as_int();
	this->itemId = node.attribute("itemId").as_int();
	this->reward = node.attribute("reward").as_int();
	this->type = QuestType::COLLECT;
}

CollectQuest::~CollectQuest() {}

bool CollectQuest::Update() {
	bool ret = true;
	//Completion event
	return ret;
}