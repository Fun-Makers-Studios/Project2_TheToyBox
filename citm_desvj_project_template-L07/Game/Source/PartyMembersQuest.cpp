#include "PartyMembersQuest.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Log.h"
#include "Point.h"
#include "SceneManager.h"

PartyMembersQuest::PartyMembersQuest(pugi::xml_node node) {
	this->id = node.attribute("id").as_int();
	this->name = node.attribute("name").as_string();
	this->description = node.attribute("description").as_string();
	this->nextQuestId = node.attribute("nextQuestId").as_int();
	this->reward = node.attribute("reward").as_int();
	this->type = QuestType::PARTYMEMBERS;

	for (pugi::xml_node itemNode = node.child("member"); itemNode; itemNode = itemNode.next_sibling("member"))
	{
		PMember member;
		member.picked = false;
		member.name = itemNode.attribute("name").as_string();
		memberNames.Add(member);
		membersLeft++;
	}
}

PartyMembersQuest::~PartyMembersQuest() {}

bool PartyMembersQuest::Update() {
	bool ret = false;
	members = 0;
	for (ListItem<PMember>* item = memberNames.start; item != nullptr; item = item->next)
	{
		if (item->data.picked) {
			members++;
		}
	}
	if (members == membersLeft) { return true; }

	//Completion event
	if (app->sceneManager->sceneGame->dialogueManager->dialogueLoaded) {
		currentSpeaker = app->sceneManager->sceneGame->dialogueManager->GetCurrentDialogue()->currentNode->speaker;
		if (app->sceneManager->sceneGame->dialogueManager->GetCurrentDialogue()->currentNode->currentOption->questCompletionId == this->id) {
			optionSelected = true;
		}
	}
	else if (optionSelected == true) {
		for (ListItem<PMember>* item = memberNames.start; item != nullptr; item = item->next)
		{
			if (item->data.name == currentSpeaker) {
				item->data.picked = true;
			}
		}

		optionSelected = false;
	}

	return ret;
}