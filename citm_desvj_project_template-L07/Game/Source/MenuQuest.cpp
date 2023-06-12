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

	//UI
	SDL_Rect rect = { app->menuManager->openBookPos.x + 410, app->menuManager->openBookPos.y + 48, 41, 20 };
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
	return true;
}


bool MenuQuest::Update(float dt)
{
	
	return true;
}


bool MenuQuest::PostUpdate()
{
	bool ret = true;

	//app->render->DrawRectangle({ 0, 0, app->render->camera.w, app->render->camera.w }, 0, 0, 0, 128, true, false, true);

	app->render->DrawTexture(questMenuImg, app->menuManager->openBookPos.x, app->menuManager->openBookPos.y, &rectTexture, SDL_FLIP_NONE, ScaleType::UI_200, false);

	iPoint displacement = { 270, 120 };
	int lines = 0;



	for (ListItem<Quest*>* qitem1 = app->questManager->activeQuests.start; qitem1 != nullptr; qitem1 = qitem1->next)
	{
		Quest* item = qitem1->data;

		lines = app->fonts->BlitText2(displacement.x, displacement.y, fontID, (const char*)item->name.GetString(), ScaleType::UI_200, 8, 215);

		if (item->id == currentQuestSelectedID)
		{
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

			app->render->DrawRectangle({ (displacement.x - 2), (displacement.y - 2), (240 - 12), ((displacement.y + (lines * (int)app->fonts->fonts[fontID].char_h) + ((lines - 1) * 8)) - displacement.y) }, 218, 141, 85, 96, true, false, ScaleType::UI_100);
			app->render->DrawRectangle({ (displacement.x - 2), (displacement.y - 2), (240 - 12), ((displacement.y + (lines * (int)app->fonts->fonts[fontID].char_h) + ((lines - 1) * 8)) - displacement.y) }, 55, 55, 80, 196, false, false, ScaleType::UI_100);
		}

		displacement.y += (lines * (int)app->fonts->fonts[fontID].char_h) + ((lines - 1) * 8) + 16;
	}

	for (ListItem<Quest*>* qitem2 = app->questManager->completedQuests.start; qitem2 != nullptr; qitem2 = qitem2->next)
	{
		Quest* item = qitem2->data;

		lines = app->fonts->BlitText2(displacement.x, displacement.y, fontID, (const char*)item->name.GetString(), ScaleType::UI_200, 8, 215);

		if (item->id == currentQuestSelectedID)
		{
			iPoint displacement2 = { 540, 130 };

			int lines2 = app->fonts->BlitText2(displacement2.x, displacement2.y, fontID, (const char*)item->description.GetString(), ScaleType::UI_200, 8, 460);
			displacement2.y += (lines2 * (int)app->fonts->fonts[fontID].char_h) + ((lines2 - 1) * 8) + 16;

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
					displacement2.y += (lines2 * (int)app->fonts->fonts[fontID].char_h) + ((lines2 - 1) * 8) + 16;
				}
			}

			app->render->DrawRectangle({ (displacement.x - 2), (displacement.y - 2), (240 - 12), ((displacement.y + (lines * (int)app->fonts->fonts[fontID].char_h) + ((lines - 1) * 8)) - displacement.y) }, 218, 141, 85, 96, true, false, ScaleType::UI_100);
			app->render->DrawRectangle({ (displacement.x - 2), (displacement.y - 2), (240 - 12), ((displacement.y + (lines * (int)app->fonts->fonts[fontID].char_h) + ((lines - 1) * 8)) - displacement.y) }, 55, 55, 80, 196, false, false, ScaleType::UI_100);
		}

		displacement.y += (lines * (int)app->fonts->fonts[fontID].char_h) + 16;
	}

	return ret;
}


bool MenuQuest::CleanUp()
{
	LOG("Freeing TITLE SCENE");

	app->tex->UnLoad(questMenuImg);

	//STORE IN A LIST THIS BUTTONS AND THEN CHECK HERE IF NULLPTR TO CLEAN THEM UP
	//guiControlsList.Clear();

	return true;
}

bool MenuQuest::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{
	case (uint32)ControlID::QUEST_1:
		
		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case (uint32)ControlID::QUEST_2:

		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case (uint32)ControlID::QUEST_3:

		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case (uint32)ControlID::QUEST_4:

		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case (uint32)ControlID::QUEST_5:

		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case (uint32)ControlID::QUEST_6:

		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case (uint32)ControlID::QUEST_7:

		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	default:
		break;

	}

	return true;
}