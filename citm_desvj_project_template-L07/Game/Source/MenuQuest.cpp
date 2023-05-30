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

	questMenuImgPath = app->configNode.child("menuManager").child("questMenuImg").attribute("questMenuImgPath").as_string();
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

	questListSelected = ControlID::ACTIVE;

	// Load
	questMenuImg = app->tex->Load(questMenuImgPath);
	pauseRect = { 35, 69, 310, 555 };

	//UI
	doneQuestsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::DONE, "done", 4, { 176, 140, 65, 76 }, this, ButtonType::SMALL);
	activeQuestsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::ACTIVE, "active", 6, { 176, 216, 64, 76 }, this, ButtonType::SMALL);


	// Set easing finished on title buttons
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		if (control->data->id < 5)
		{
			control->data->easing->SetFinished(false);
			control->data->easing->SetTotalTime(1 + 0.2 * control->data->id);
		}

		control = control->next;
	}

	if (app->questManager->activeQuests.start != nullptr) {
		currentQuestSelectedActive = app->questManager->activeQuests.start->data->id;
	}

	if (app->questManager->completedQuests.start != nullptr) {
		currentQuestSelectedDone = app->questManager->completedQuests.start->data->id;
	}

	return true;
}


bool MenuQuest::PreUpdate()
{
	/*if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		app->audio->PlayFx(app->menuManager->closeMenuSFX);
		app->menuManager->SelectMenu();
	}*/

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		if (questListSelected == ControlID::ACTIVE) {
			for (ListItem<Quest*>* qitem1 = app->questManager->activeQuests.start; qitem1 != nullptr; qitem1 = qitem1->next)
			{
				if (qitem1->next != nullptr) {
					if (qitem1->next->data->id == currentQuestSelectedActive) {
						currentQuestSelectedActive = qitem1->data->id;
						break;
					}
				}
			}
		}
		else if (questListSelected == ControlID::DONE) {
			for (ListItem<Quest*>* qitem1 = app->questManager->completedQuests.start; qitem1 != nullptr; qitem1 = qitem1->next)
			{
				if (qitem1->next != nullptr) {
					if (qitem1->next->data->id == currentQuestSelectedDone) {
						currentQuestSelectedDone = qitem1->data->id;
						break;
					}
				}
			}
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		if (questListSelected == ControlID::ACTIVE) {
			for (ListItem<Quest*>* qitem1 = app->questManager->activeQuests.start; qitem1 != nullptr; qitem1 = qitem1->next)
			{
				if (qitem1->data->id == currentQuestSelectedActive) {
					if (qitem1->next != nullptr) {
						currentQuestSelectedActive = qitem1->next->data->id;
					}
					break;
				}
			}
		}
		else if (questListSelected == ControlID::DONE) {
			for (ListItem<Quest*>* qitem1 = app->questManager->completedQuests.start; qitem1 != nullptr; qitem1 = qitem1->next)
			{
				if (qitem1->data->id == currentQuestSelectedDone) {
					if (qitem1->next != nullptr) {
						currentQuestSelectedDone = qitem1->next->data->id;
					}
					break;
				}
			}
		}
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

	//app->render->DrawRectangle({ 0, 0, app->render->camera.w, app->render->camera.w }, 0, 0, 0, 128, true, false, true);

	app->render->DrawTexture(questMenuImg, app->render->camera.x, app->render->camera.y - 3, NULL);

	iPoint displacement = { 270, 120 };
	int lines = 0;
	uint fontID = app->sceneManager->sceneGame->dialogueManager->dialogueFontId;

	switch (questListSelected)
	{
	case ControlID::ACTIVE:

		for (ListItem<Quest*>* qitem1 = app->questManager->activeQuests.start; qitem1 != nullptr; qitem1 = qitem1->next)
		{
			Quest* item = qitem1->data;

			lines = app->fonts->BlitText2(displacement.x, displacement.y, fontID, (const char*)item->name.GetString(), 8, 215);

			if (item->id == currentQuestSelectedActive) {
				iPoint displacement2 = { 540, 130 };

				int lines2 = app->fonts->BlitText2(displacement2.x, displacement2.y, fontID, (const char*)item->description.GetString(), 8, 460);
				displacement2.y += (lines2 * (int)app->fonts->fonts[fontID].char_h) + ((lines2 - 1) * 8) + 16;

				if (item->type == QuestType::PARTYMEMBERS) {
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
						lines2 = app->fonts->BlitText2(displacement2.x, displacement2.y, fontID, (const char*)strInfo.c_str(), 8, 460);
						displacement2.y += (lines2 * (int)app->fonts->fonts[fontID].char_h) + ((lines2 - 1) * 8) + 16;
					}
				}

				app->render->DrawRectangle({ (displacement.x - 2), (displacement.y - 2), (240 - 12), ((displacement.y + (lines * (int)app->fonts->fonts[fontID].char_h) + ((lines - 1) * 8)) - displacement.y) }, 218, 141, 85, 96, true, false, ScaleType::UI_100);
				app->render->DrawRectangle({ (displacement.x - 2), (displacement.y - 2), (240 - 12), ((displacement.y + (lines * (int)app->fonts->fonts[fontID].char_h) + ((lines - 1) * 8)) - displacement.y) }, 55, 55, 80, 196, false, false, ScaleType::UI_100);
			}

			displacement.y += (lines * (int)app->fonts->fonts[fontID].char_h) + ((lines - 1) * 8) + 16;
		}
		break;

	case ControlID::DONE:

		for (ListItem<Quest*>* qitem2 = app->questManager->completedQuests.start; qitem2 != nullptr; qitem2 = qitem2->next)
		{
			Quest* item = qitem2->data;

			lines = app->fonts->BlitText2(displacement.x, displacement.y, fontID, (const char*)item->name.GetString(), 8, 215);

			if (item->id == currentQuestSelectedDone) {
				iPoint displacement2 = { 540, 130 };

				int lines2 = app->fonts->BlitText2(displacement2.x, displacement2.y, fontID, (const char*)item->description.GetString(), 8, 460);
				displacement2.y += (lines2 * (int)app->fonts->fonts[fontID].char_h) + ((lines2 - 1) * 8) + 16;

				if (item->type == QuestType::PARTYMEMBERS) {
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
						lines2 = app->fonts->BlitText2(displacement2.x, displacement2.y, fontID, (const char*)strInfo.c_str(), 8, 460);
						displacement2.y += (lines2 * (int)app->fonts->fonts[fontID].char_h) + ((lines2 - 1) * 8) + 16;
					}
				}

				app->render->DrawRectangle({ (displacement.x - 2), (displacement.y - 2), (240 - 12), ((displacement.y + (lines * (int)app->fonts->fonts[fontID].char_h) + ((lines - 1) * 8)) - displacement.y) }, 218, 141, 85, 96, true, false, ScaleType::UI_100);
				app->render->DrawRectangle({ (displacement.x - 2), (displacement.y - 2), (240 - 12), ((displacement.y + (lines * (int)app->fonts->fonts[fontID].char_h) + ((lines - 1) * 8)) - displacement.y) }, 55, 55, 80, 196, false, false, ScaleType::UI_100);
			}

			displacement.y += (lines * (int)app->fonts->fonts[fontID].char_h) + 16;
		}
		break;

	default:
		break;
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
	case (uint32)ControlID::DONE:
		questListSelected = ControlID::DONE;
		if (app->questManager->completedQuests.start != nullptr) {
			currentQuestSelectedDone = app->questManager->completedQuests.start->data->id;
		}

		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case (uint32)ControlID::ACTIVE:
		questListSelected = ControlID::ACTIVE;
		if (app->questManager->activeQuests.start != nullptr) {
			currentQuestSelectedActive = app->questManager->activeQuests.start->data->id;
		}

		app->audio->PlayFx(app->menuManager->startSFX);
		break;

	default:
		break;

	}

	return true;
}