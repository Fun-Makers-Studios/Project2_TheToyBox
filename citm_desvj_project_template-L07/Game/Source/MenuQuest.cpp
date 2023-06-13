#include "MenuQuest.h"

#include "App.h"
#include "Menu.h"
#include "Quest.h"
#include "SceneManager.h"
#include "MenuManager.h"
#include "QuestManager.h"
#include "DialogueManager.h"
#include "GuiManager.h"
#include "PartyMembersQuest.h"

#include "Window.h"
#include "Audio.h"
#include "Fonts.h"
#include "Log.h"


MenuQuest::MenuQuest() : Menu()
{
	id = MenuID::MENU_QUEST;

	questMenuImgPath = app->configNode.child("menuManager").child("menuQuests").attribute("texturepath").as_string();
	questsBoxImgPath = app->configNode.child("menuManager").child("menuQuests").attribute("questsBoxTexture").as_string();
		
	activeQuestBox1 =	{ 0 * 0, 32 * 0, 218, 22 };
	doneQuestBox1 =		{ 0 * 0, 32 * 1, 218, 22 };
	activeQuestBox2 =	{ 0 * 0, 32 * 2, 218, 22 };
	doneQuestBox2 =		{ 0 * 0, 32 * 3, 218, 22 };
}


MenuQuest::~MenuQuest()
{}


bool MenuQuest::Awake(pugi::xml_node& config)
{
	LOG("|| AWAKE MenuQuest ||");
	bool ret = true;

	return ret;
}


bool MenuQuest::Start()
{
	LOG("--START MenuQuest--");

	// Load
	questMenuImg = app->tex->Load(questMenuImgPath);
	rectTexture = { 528, 0, 519, 311 };
	questsBoxTexture = app->tex->Load(questsBoxImgPath);

	//UI
	SDL_Rect rect = { app->menuManager->openBookPos.x + 409, app->menuManager->openBookPos.y + 50, 41, 20 };
	quest1 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::QUEST_1, "", 1, rect, this, ButtonType::RECTANGLE_S);
	quest2 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::QUEST_2, "", 1, rect, this, ButtonType::RECTANGLE_S);
	quest3 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::QUEST_3, "", 1, rect, this, ButtonType::RECTANGLE_S);
	quest4 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::QUEST_4, "", 1, rect, this, ButtonType::RECTANGLE_S);
	quest5 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::QUEST_5, "", 1, rect, this, ButtonType::RECTANGLE_S);
	quest6 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::QUEST_6, "", 1, rect, this, ButtonType::RECTANGLE_S);
	quest7 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::QUEST_7, "", 1, rect, this, ButtonType::RECTANGLE_S);

	for (int i = 0; i < 7; i++)
		guiControlsList.At(i)->data->bounds.y += (i * 32);
	
	fontID = app->menuManager->font4_id;

	if (app->questManager->activeQuests.start != nullptr) {
		currentQuestSelectedID = app->questManager->activeQuests.start->data->id;
	}

	return true;
}


bool MenuQuest::PreUpdate()
{
	int quests = 0;

	for (ListItem<Quest*>* qitem1 = app->questManager->activeQuests.start; qitem1 != nullptr; qitem1 = qitem1->next) {
		quests++;
	}

	for (ListItem<Quest*>* qitem2 = app->questManager->completedQuests.start; qitem2 != nullptr; qitem2 = qitem2->next) {
		quests++;
	}

	switch (quests)
	{
	case 1:
		quest2->state = GuiControlState::DISABLED;
		quest3->state = GuiControlState::DISABLED;
		quest4->state = GuiControlState::DISABLED;
		quest5->state = GuiControlState::DISABLED;
		quest6->state = GuiControlState::DISABLED;
		quest7->state = GuiControlState::DISABLED;
		break;

	case 2:
		quest3->state = GuiControlState::DISABLED;
		quest4->state = GuiControlState::DISABLED;
		quest5->state = GuiControlState::DISABLED;
		quest6->state = GuiControlState::DISABLED;
		quest7->state = GuiControlState::DISABLED;
		break;

	case 3:
		quest4->state = GuiControlState::DISABLED;
		quest5->state = GuiControlState::DISABLED;
		quest6->state = GuiControlState::DISABLED;
		quest7->state = GuiControlState::DISABLED;
		break;
		break;

	case 4:
		quest5->state = GuiControlState::DISABLED;
		quest6->state = GuiControlState::DISABLED;
		quest7->state = GuiControlState::DISABLED;
		break;
		break;

	case 5:
		quest6->state = GuiControlState::DISABLED;
		quest7->state = GuiControlState::DISABLED;
		break;
		break;

	case 6:
		quest7->state = GuiControlState::DISABLED;
		break;

	case 7:

		break;

	default:
		break;
	}

	return true;
}


bool MenuQuest::Update(float dt)
{
	

	return true;
}


bool MenuQuest::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(questMenuImg, app->menuManager->openBookPos.x, app->menuManager->openBookPos.y, &rectTexture, SDL_FLIP_NONE, ScaleType::UI_200, false);

	iPoint displacement = { 284 + app->menuManager->openBookPos.x, 59 + app->menuManager->openBookPos.y };
	int lines = 0;

	int currentQuestNum = 0;

	for (ListItem<Quest*>* qitem1 = app->questManager->activeQuests.start; qitem1 != nullptr; qitem1 = qitem1->next)
	{
		Quest* item = qitem1->data;

		if (item->id == currentQuestSelectedID) {
			app->render->DrawTexture(questsBoxTexture, boxPos.x + app->menuManager->openBookPos.x, (boxPos.y + app->menuManager->openBookPos.y + (32 * currentQuestNum)), &activeQuestBox2, SDL_FLIP_NONE, ScaleType::UI_200, false);
		}
		else
		{
			app->render->DrawTexture(questsBoxTexture, boxPos.x + app->menuManager->openBookPos.x, (boxPos.y + app->menuManager->openBookPos.y + (32 * currentQuestNum)), &activeQuestBox1, SDL_FLIP_NONE, ScaleType::UI_200, false);
		}

		app->fonts->BlitText(438 + app->menuManager->openBookPos.x, (58 + app->menuManager->openBookPos.y + (32 * currentQuestNum)), fontID, std::to_string(item->id).c_str(), ScaleType::UI_200);

		lines = app->fonts->BlitText2(displacement.x, displacement.y, fontID, (const char*)item->name.GetString(), ScaleType::UI_200, 8, 120);

		if (item->id == currentQuestSelectedID)
		{
			app->fonts->BlitText(135 + app->menuManager->openBookPos.x, 47 + app->menuManager->openBookPos.y, fontID, std::to_string(item->id).c_str(), ScaleType::UI_200);
			app->fonts->BlitText( 131 + app->menuManager->openBookPos.x, 145 + app->menuManager->openBookPos.y, fontID, std::to_string(item->id).c_str(), ScaleType::UI_200);
			iPoint displacement2 = { 73 + app->menuManager->openBookPos.x, 160 + app->menuManager->openBookPos.y};

			int lines2 = app->fonts->BlitText2(displacement2.x, displacement2.y, fontID, (const char*)item->description.GetString(), ScaleType::UI_200, 7, 100);
			displacement2.y += (lines2 * (int)app->fonts->fonts[fontID].char_h) + ((lines2 - 1) * 7) + 7;

			if (item->type == QuestType::PARTYMEMBERS)
			{
				PartyMembersQuest* pQuest = dynamic_cast<PartyMembersQuest*>(item);
				for (ListItem<PMember>* pmember = pQuest->memberNames.start; pmember != nullptr; pmember = pmember->next)
				{
					std::string strInfo;
					if (pmember->data.picked) {
						strInfo = std::string(pmember->data.name.GetString()) + " - FOUND";
					}
					else {
						strInfo = std::string(pmember->data.name.GetString()) + " - LOST";
					}
					lines2 = app->fonts->BlitText2(displacement2.x, displacement2.y, fontID, (const char*)strInfo.c_str(), ScaleType::UI_200, 8, 460);
					displacement2.y += (lines2 * (int)app->fonts->fonts[fontID].char_h) + ((lines2 - 1) * 7) + 7;
				}
			}
		}

		displacement.y += 32;
		currentQuestNum++;
	}

	for (ListItem<Quest*>* qitem2 = app->questManager->completedQuests.start; qitem2 != nullptr; qitem2 = qitem2->next)
	{
		Quest* item = qitem2->data;

		if (item->id == currentQuestSelectedID) {
			app->render->DrawTexture(questsBoxTexture, boxPos.x + app->menuManager->openBookPos.x, (boxPos.y + app->menuManager->openBookPos.y + (32 * currentQuestNum)), &doneQuestBox2, SDL_FLIP_NONE, ScaleType::UI_200, false);
		}
		else
		{
			app->render->DrawTexture(questsBoxTexture, boxPos.x + app->menuManager->openBookPos.x, (boxPos.y + app->menuManager->openBookPos.y + (32 * currentQuestNum)), &doneQuestBox1, SDL_FLIP_NONE, ScaleType::UI_200, false);
		}

		app->fonts->BlitText(438 + app->menuManager->openBookPos.x, (58 + app->menuManager->openBookPos.y + (32 * currentQuestNum)), fontID, std::to_string(item->id).c_str(), ScaleType::UI_200);

		lines = app->fonts->BlitText2(displacement.x, displacement.y, fontID, (const char*)item->name.GetString(), ScaleType::UI_200, 8, 120);

		if (item->id == currentQuestSelectedID)
		{
			app->fonts->BlitText(131 + app->menuManager->openBookPos.x, 145 + app->menuManager->openBookPos.y, fontID, std::to_string(item->id).c_str(), ScaleType::UI_200);
			iPoint displacement2 = { 73 + app->menuManager->openBookPos.x, 160 + app->menuManager->openBookPos.y };

			int lines2 = app->fonts->BlitText2(displacement2.x, displacement2.y, fontID, (const char*)item->description.GetString(), ScaleType::UI_200, 7, 100);
			displacement2.y += (lines2 * (int)app->fonts->fonts[fontID].char_h) + ((lines2 - 1) * 7) + 7;

			if (item->type == QuestType::PARTYMEMBERS)
			{
				PartyMembersQuest* pQuest = dynamic_cast<PartyMembersQuest*>(item);
				for (ListItem<PMember>* pmember = pQuest->memberNames.start; pmember != nullptr; pmember = pmember->next)
				{
					std::string strInfo;
					if (pmember->data.picked) {
						strInfo = std::string(pmember->data.name.GetString()) + " - FOUND";
					}
					else {
						strInfo = std::string(pmember->data.name.GetString()) + " - LOST";
					}
					lines2 = app->fonts->BlitText2(displacement2.x, displacement2.y, fontID, (const char*)strInfo.c_str(), ScaleType::UI_200, 8, 460);
					displacement2.y += (lines2 * (int)app->fonts->fonts[fontID].char_h) + ((lines2 - 1) * 7) + 7;
				}
			}
		}

		displacement.y += 32;
		currentQuestNum++;
	}

	return ret;
}


bool MenuQuest::CleanUp()
{
	LOG("Freeing TITLE SCENE");

	app->tex->UnLoad(questMenuImg);
	app->tex->UnLoad(questsBoxTexture);

	//STORE IN A LIST THIS BUTTONS AND THEN CHECK HERE IF NULLPTR TO CLEAN THEM UP
	//guiControlsList.Clear();

	return true;
}

bool MenuQuest::OnGuiMouseClickEvent(GuiControl* control)
{
	int quests = 1;
	int questID = 0;

	switch (control->id)
	{
	case (uint32)ControlID::QUEST_1:
		quests = 1;
		
		for (ListItem<Quest*>* qitem1 = app->questManager->activeQuests.start; qitem1 != nullptr; qitem1 = qitem1->next) {
			quests--;
			if (quests == 0) {
				questID = qitem1->data->id;
			}
		}

		for (ListItem<Quest*>* qitem2 = app->questManager->completedQuests.start; qitem2 != nullptr; qitem2 = qitem2->next) {
			quests--;
			if (quests == 0) {
				questID = qitem2->data->id;
			}
		}
		currentQuestSelectedID = questID;
		
		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case (uint32)ControlID::QUEST_2:
		quests = 2;

		for (ListItem<Quest*>* qitem1 = app->questManager->activeQuests.start; qitem1 != nullptr; qitem1 = qitem1->next) {
			quests--;
			if (quests == 0) {
				questID = qitem1->data->id;
			}
		}

		for (ListItem<Quest*>* qitem2 = app->questManager->completedQuests.start; qitem2 != nullptr; qitem2 = qitem2->next) {
			quests--;
			if (quests == 0) {
				questID = qitem2->data->id;
			}
		}
		currentQuestSelectedID = questID;

		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case (uint32)ControlID::QUEST_3:
		quests = 3;

		for (ListItem<Quest*>* qitem1 = app->questManager->activeQuests.start; qitem1 != nullptr; qitem1 = qitem1->next) {
			quests--;
			if (quests == 0) {
				questID = qitem1->data->id;
			}
		}

		for (ListItem<Quest*>* qitem2 = app->questManager->completedQuests.start; qitem2 != nullptr; qitem2 = qitem2->next) {
			quests--;
			if (quests == 0) {
				questID = qitem2->data->id;
			}
		}
		currentQuestSelectedID = questID;

		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case (uint32)ControlID::QUEST_4:
		quests = 4;

		for (ListItem<Quest*>* qitem1 = app->questManager->activeQuests.start; qitem1 != nullptr; qitem1 = qitem1->next) {
			quests--;
			if (quests == 0) {
				questID = qitem1->data->id;
			}
		}

		for (ListItem<Quest*>* qitem2 = app->questManager->completedQuests.start; qitem2 != nullptr; qitem2 = qitem2->next) {
			quests--;
			if (quests == 0) {
				questID = qitem2->data->id;
			}
		}
		currentQuestSelectedID = questID;

		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case (uint32)ControlID::QUEST_5:
		quests = 5;

		for (ListItem<Quest*>* qitem1 = app->questManager->activeQuests.start; qitem1 != nullptr; qitem1 = qitem1->next) {
			quests--;
			if (quests == 0) {
				questID = qitem1->data->id;
			}
		}

		for (ListItem<Quest*>* qitem2 = app->questManager->completedQuests.start; qitem2 != nullptr; qitem2 = qitem2->next) {
			quests--;
			if (quests == 0) {
				questID = qitem2->data->id;
			}
		}
		currentQuestSelectedID = questID;

		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case (uint32)ControlID::QUEST_6:
		quests = 6;

		for (ListItem<Quest*>* qitem1 = app->questManager->activeQuests.start; qitem1 != nullptr; qitem1 = qitem1->next) {
			quests--;
			if (quests == 0) {
				questID = qitem1->data->id;
			}
		}

		for (ListItem<Quest*>* qitem2 = app->questManager->completedQuests.start; qitem2 != nullptr; qitem2 = qitem2->next) {
			quests--;
			if (quests == 0) {
				questID = qitem2->data->id;
			}
		}
		currentQuestSelectedID = questID;

		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case (uint32)ControlID::QUEST_7:
		quests = 7;

		for (ListItem<Quest*>* qitem1 = app->questManager->activeQuests.start; qitem1 != nullptr; qitem1 = qitem1->next) {
			quests--;
			if (quests == 0) {
				questID = qitem1->data->id;
			}
		}

		for (ListItem<Quest*>* qitem2 = app->questManager->completedQuests.start; qitem2 != nullptr; qitem2 = qitem2->next) {
			quests--;
			if (quests == 0) {
				questID = qitem2->data->id;
			}
		}
		currentQuestSelectedID = questID;

		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	default:
		break;

	}

	return true;
}