#include "QuestManager.h"
#include "App.h"
#include "Textures.h"
#include "TalkQuest.h"
#include "CollectQuest.h"

#include "Defs.h"
#include "Log.h"

QuestManager::QuestManager() : Module()
{
	name.Create("questmanager");
}

// Destructor
QuestManager::~QuestManager()
{}

// Called before render is available
bool QuestManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Quest Manager");
	bool ret = true;

	for (pugi::xml_node node = config.child("quest"); node; node = node.next_sibling("quest"))
	{
		Quest* quest;
		switch ((QuestType)node.attribute("type").as_int())
		{
		case QuestType::TALK:
			quest = new TalkQuest(node);
			break;
		case QuestType::COLLECT:
			quest = new CollectQuest(node);
			break;
		default:
			break;
		}

		quests.Add(quest);
	}

	for (pugi::xml_node node = config.child("activequests").child("quest"); node; node = node.next_sibling("quest"))
	{
		ListItem<Quest*>* qitem = quests.start;
		while (qitem != nullptr)
		{
			Quest* item = qitem->data;
			if (item->id == node.attribute("id").as_int()) {
				activeQuests.Add(item);
				break;
			}

			qitem = qitem->next;
		}
	}

	return ret;
}

bool QuestManager::Start() {

	LOG("Starting Quest Manager");
	bool ret = true;

	return ret;
}

bool QuestManager::Update(float dt)
{
	bool ret = true;

	ListItem<Quest*>* item;
	Quest* pQuest = NULL;

	for (item = activeQuests.start; item != NULL; item = item->next)
	{
		pQuest = item->data;

		if (pQuest->Update() == false) {
			activeQuests.Del(item);

			ListItem<Quest*>* qitem = quests.start;
			while (qitem != nullptr)
			{
				Quest* item = qitem->data;
				if (item->id == pQuest->nextQuestId) {
					activeQuests.Add(item);
					break;
				}

				qitem = qitem->next;
			}

			completedQuests.Add(pQuest);
		}
	}

	return ret;
}