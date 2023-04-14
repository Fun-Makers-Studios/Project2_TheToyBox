#include "TalkQuest.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Log.h"
#include "Point.h"

TalkQuest::TalkQuest(pugi::xml_node node)
{
	this->id = node.attribute("id").as_int();
	this->name = node.attribute("name").as_string();
	this->description = node.attribute("description").as_string();
	this->nextQuestId = node.attribute("nextQuestId").as_int();
	this->npcId = node.attribute("npcId").as_int();
	this->reward = node.attribute("reward").as_int();
	this->type = QuestType::TALK;
}

TalkQuest::~TalkQuest() {}

bool TalkQuest::Update()
{
	bool ret = true;

	List<NPC*>* npcList = app->sceneManager->GetCurrentScene()->GetNPCList();
	SString id = app->sceneManager->GetCurrentScene()->id;
	ListItem<NPC*>* npcItem = npcList->start;
	NPC* npc = npcItem->data;

	while (npcItem != nullptr)
	{
		if (npc->npcid == this->npcId) { break; }
		npcItem = npcItem->next;
		npc = npcItem->data;
	}


	//Check completion event
	/*ListItem<NPC*>* nitem = app->sceneManager->GetCurrentScene()->GetNPCList()->start;
	NPC* npc = nitem->data;
	
	while (nitem != nullptr)
	{
		if (npc->npcid == this->npcId) { break; }
		nitem = nitem->next;
		npc = nitem->data;
	}*/

	//Completion event: player is in npc boundaries and talks to him (G key)
	Entity* player = nullptr;
	for (int i = 0; i < app->entityManager->entities.Count(); i++)
	{
		if (app->entityManager->entities.At(i)->data->type == EntityType::PLAYER)
		{
			player = app->entityManager->entities.At(i)->data;
			break;
		}
	}

	if ((player->position.x * 32 >= npc->boundaries.x) && 
		(player->position.x * 32 < npc->boundaries.x + npc->boundaries.w) &&
		(player->position.y * 32 >= npc->boundaries.y) &&
		(player->position.y * 32 < npc->boundaries.y + npc->boundaries.h) &&
		(app->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)) {
		ret = false;

		//app->scene->points += this->reward;
	}

	return ret;
}