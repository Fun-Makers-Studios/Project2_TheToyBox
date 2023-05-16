#include "TalkQuest.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Log.h"
#include "Point.h"
#include "SceneManager.h"

TalkQuest::TalkQuest(pugi::xml_node node) {
	this->id = node.attribute("id").as_int();
	this->name = node.attribute("name").as_string();
	this->description = node.attribute("description").as_string();
	this->nextQuestId = node.attribute("nextQuestId").as_int();
	this->reward = node.attribute("reward").as_int();
	this->type = QuestType::TALK;
}

TalkQuest::~TalkQuest() {}

bool TalkQuest::Update() {
	bool ret = false;

	//Check completion event
	if (app->sceneManager->sceneGame->dialogueManager->dialogueLoaded) {
		if (app->sceneManager->sceneGame->dialogueManager->GetCurrentDialogue()->currentNode->currentOption->questCompletionId == this->id) {
			optionSelected = true;
		}
	}
	else if (optionSelected == true) {
		optionSelected = false;
		ret = true;
	}

	return ret;
}