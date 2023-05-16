#include "QuestManager.h"
#include "App.h"
#include "Textures.h"
#include "TalkQuest.h"
#include "CollectQuest.h"
#include "PartyMembersQuest.h"
#include "GoToQuest.h"
#include "ImposibleQuest.h"
#include "MenuManager.h"

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
		case QuestType::PARTYMEMBERS:
			quest = new PartyMembersQuest(node);
			break;
		case QuestType::GOTO:
			quest = new GoToQuest(node);
			break;
		case QuestType::IMPOSIBLE:
			quest = new ImposibleQuest(node);
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

bool QuestManager::Start()
{
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

		if (pQuest->Update() == true)
		{
			activeQuests.Del(item);

			if (pQuest->nextQuestId != -1) {
				ListItem<Quest*>* qitem = quests.start;
				while (qitem != nullptr)
				{
					Quest* item = qitem->data;
					if (item->id == pQuest->nextQuestId)
					{
						activeQuests.Add(item);
						break;
					}

					qitem = qitem->next;
				}
			}

			completedQuests.Add(pQuest);

			//Open Quests Menu
			app->menuManager->menuState = MenuState::SWITCH;
			app->menuManager->nextMenu = MenuID::MENU_QUEST;
		}
	}

	return ret;
}

void QuestManager::TriggerQuest(int id)
{
	ListItem<Quest*>* qitem = quests.start;

	while (qitem != nullptr)
	{
		Quest* item = qitem->data;
		if (item->id == id) {
			activeQuests.Add(item);
			break;
		}

		qitem = qitem->next;
	}
}