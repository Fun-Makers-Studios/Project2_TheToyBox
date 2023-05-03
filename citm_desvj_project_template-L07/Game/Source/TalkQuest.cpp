#include "TalkQuest.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Log.h"
#include "Point.h"
#include "Scene.h"

TalkQuest::TalkQuest(pugi::xml_node node) {
	this->id = node.attribute("id").as_int();
	this->name = node.attribute("name").as_string();
	this->description = node.attribute("description").as_string();
	this->nextQuestId = node.attribute("nextQuestId").as_int();
	this->npcId = node.attribute("npcId").as_int();
	this->reward = node.attribute("reward").as_int();
	this->type = QuestType::TALK;
}

TalkQuest::~TalkQuest() {}

bool TalkQuest::Update() {
	bool ret = true;

	//Check completion event
	ListItem<NPC*>* nitem = app->scene->npcs.start;
	while (nitem != nullptr)
	{

		if (nitem->data->npcid == this->npcId) { break; }
		nitem = nitem->next;
	}

	//Completion event: player is in npc boundaries and talks to him (G key)
	if ((app->scene->player->tile.x * 32 >= nitem->data->boundaries.x) &&
		(app->scene->player->tile.x * 32 < nitem->data->boundaries.x + nitem->data->boundaries.w) &&
		(app->scene->player->tile.y * 32 >= nitem->data->boundaries.y) &&
		(app->scene->player->tile.y * 32 < nitem->data->boundaries.y + nitem->data->boundaries.h) &&
		(app->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)) {
		ret = false;

		app->scene->points += this->reward;
	}

	return ret;
}