#include "CollectQuest.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Log.h"
#include "Point.h"
#include "MenuParty.h"
#include "MenuManager.h"

CollectQuest::CollectQuest(pugi::xml_node node)
{
	this->id = node.attribute("id").as_int();
	this->name = node.attribute("name").as_string();
	this->description = node.attribute("description").as_string();
	this->nextQuestId = node.attribute("nextQuestId").as_int();
	this->itemName = node.attribute("itemName").as_string();
	this->reward = node.attribute("reward").as_int();
	this->type = QuestType::COLLECT;
}

CollectQuest::~CollectQuest() {}

bool CollectQuest::Update()
{
	bool ret = false;

	//Completion event
	for (ListItem<InventorySlot*>* sitem = app->menuManager->menuParty->inventorySlotList.start; sitem != nullptr; sitem = sitem->next)
	{
		if (sitem->data->item == nullptr)
			continue;

		if (sitem->data->item->itemData.name == itemName)
			return true;
	}

	return ret;
}