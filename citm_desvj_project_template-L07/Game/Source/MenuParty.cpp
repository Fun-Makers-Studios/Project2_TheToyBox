#include "MenuParty.h"

#include "App.h"
#include "Menu.h"
#include "Quest.h"
#include "SceneManager.h"
#include "MenuManager.h"
#include "QuestManager.h"
#include "DialogueManager.h"
#include "GuiManager.h"

#include "Window.h"
#include "Audio.h"
#include "Fonts.h"
#include "Log.h"


MenuParty::MenuParty() : Menu()
{
	id = MenuID::MENU_PARTY;

	partyMenuImgPath = app->configNode.child("menuManager").child("partyMenuImg").attribute("partyMenuImgPath").as_string();
	zeroImgPath = app->configNode.child("menuManager").child("zeroImg").attribute("zeroImgPath").as_string();
	sophieImgPath = app->configNode.child("menuManager").child("sophieImg").attribute("sophieImgPath").as_string();
}


MenuParty::~MenuParty()
{}


bool MenuParty::Awake(pugi::xml_node& config)
{
	LOG("|| AWAKE MenuParty ||");
	bool ret = true;

	return ret;
}


bool MenuParty::Start()
{
	LOG("--START MenuParty--");

	partyMemberSelected = ControlID::PARTY_1;

	// Load
	partyMenuImg = app->tex->Load(partyMenuImgPath);
	zeroImg = app->tex->Load(zeroImgPath);
	sophieImg = app->tex->Load(sophieImgPath);

	//UI
	partyMember1Button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::PARTY_1, "z", 2, { 176, 140, 65, 76 }, this, ButtonType::SMALL);
	partyMember2Button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::PARTY_2, "s", 2, { 176, 216, 64, 76 }, this, ButtonType::SMALL);

	for (int i = 0; i < MAX_INVENTORY_SLOTS_ROWS; i++)
	{
		for (int j = 0; j < MAX_INVENTORY_SLOTS_COLS; j++)
		{
			GuiInventorySlot* slot = (GuiInventorySlot*)app->guiManager->CreateGuiControl(GuiControlType::INVENTORY_SLOT, (uint32)ControlID::I_SLOT, "", 1, { 341 + ((41 + margin) * j), 423 + ((38 + marginDown) * i), 40, 38 }, this);
			slot->slotID = slotNumP;
			inventorySlotsList.Add(slot);
			slotNumP++;
		}
	}

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

	return true;
}


bool MenuParty::PreUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		app->audio->PlayFx(app->menuManager->closeMenuSFX);
		app->menuManager->SelectMenu();
	}
	return true;
}


bool MenuParty::Update(float dt)
{
	
	return true;
}


bool MenuParty::PostUpdate()
{
	bool ret = true;

	app->render->DrawRectangle({ 0, 0, app->render->camera.w, app->render->camera.w }, 0, 0, 0, 128, true, false, true);

	app->render->DrawTexture(partyMenuImg, app->render->camera.x, app->render->camera.y - 3, NULL);

	switch (partyMemberSelected)
	{
	case ControlID::PARTY_1:
		// Zero
		app->render->DrawTexture(zeroImg, app->render->camera.x + 312, app->render->camera.y + 220, NULL);
		app->sceneManager->sceneGame->partyMemberSelected = 0;
		break;

	case ControlID::PARTY_2:
		// Sophie
		app->render->DrawTexture(sophieImg, app->render->camera.x + 312, app->render->camera.y + 220, NULL);
		app->sceneManager->sceneGame->partyMemberSelected = 1;
		break;
	default:
		break;
	}

	BlitPartyStats();

	return ret;
}


bool MenuParty::CleanUp()
{
	LOG("Freeing TITLE SCENE");

	app->tex->UnLoad(partyMenuImg);
	app->tex->UnLoad(zeroImg);
	app->tex->UnLoad(sophieImg);

	//STORE IN A LIST THIS BUTTONS AND THEN CHECK HERE IF NULLPTR TO CLEAN THEM UP
	//guiControlsList.Clear();

	return true;
}

bool MenuParty::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{
	case (uint32)ControlID::PARTY_1:
		partyMemberSelected = ControlID::PARTY_1;
		app->audio->PlayFx(app->menuManager->startSFX);
		break;

	case (uint32)ControlID::PARTY_2:
		partyMemberSelected = ControlID::PARTY_2;
		app->audio->PlayFx(app->menuManager->startSFX);
		break;
	
	case (uint32)ControlID::I_SLOT:
	{
		//control->state = GuiControlState::SELECTED;
		app->audio->PlayFx(app->menuManager->closeMenuSFX);

	}break;

	default:
		break;

	}

	return true;
}

void MenuParty::SwapItems()
{

}

void MenuParty::BlitPartyStats()
{
	app->fonts->BlitText(465, 230, app->menuManager->font1_id, app->partyManager->party.At(app->sceneManager->sceneGame->partyMemberSelected)->data->name.GetString());

	char level[5];
	sprintf_s(level, 5, "%d", app->partyManager->party.At(app->sceneManager->sceneGame->partyMemberSelected)->data->level);
	app->fonts->BlitText(465, 260, app->menuManager->font1_id, level);

	char currentHP[8];
	sprintf_s(currentHP, 8, "hp %d/", app->partyManager->party.At(app->sceneManager->sceneGame->partyMemberSelected)->data->currentHp);
	app->fonts->BlitText(776, 265, app->menuManager->font1_id, currentHP);

	char maxHP[3];
	sprintf_s(maxHP, 3, "%d", app->partyManager->party.At(app->sceneManager->sceneGame->partyMemberSelected)->data->maxHp);
	app->fonts->BlitText(824, 265, app->menuManager->font1_id, maxHP);

	char currentMana[10];
	sprintf_s(currentMana, 10, "mana %d/", app->partyManager->party.At(app->sceneManager->sceneGame->partyMemberSelected)->data->currentMana);
	app->fonts->BlitText(871, 265, app->menuManager->font1_id, currentMana);

	char maxMana[3];
	sprintf_s(maxMana, 3, "%d", app->partyManager->party.At(app->sceneManager->sceneGame->partyMemberSelected)->data->maxMana);
	app->fonts->BlitText(936, 265, app->menuManager->font1_id, maxMana);

	char attack[12];
	sprintf_s(attack, 12, "attack %d", app->partyManager->party.At(app->sceneManager->sceneGame->partyMemberSelected)->data->attack);
	app->fonts->BlitText(776, 290, app->menuManager->font1_id, attack);

	char defense[15];
	sprintf_s(defense, 15, "defense %d", app->partyManager->party.At(app->sceneManager->sceneGame->partyMemberSelected)->data->defense);
	app->fonts->BlitText(871, 290, app->menuManager->font1_id, defense);

	char speed[10];
	sprintf_s(speed, 10, "speed %d", app->partyManager->party.At(app->sceneManager->sceneGame->partyMemberSelected)->data->speed);
	app->fonts->BlitText(776, 315, app->menuManager->font1_id, speed);

	char critRate[15];
	sprintf_s(critRate, 15, "crit r %d", app->partyManager->party.At(app->sceneManager->sceneGame->partyMemberSelected)->data->critRate);
	app->fonts->BlitText(871, 315, app->menuManager->font1_id, critRate);
}