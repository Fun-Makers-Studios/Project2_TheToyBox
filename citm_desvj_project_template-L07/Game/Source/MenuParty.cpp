#include "MenuParty.h"

#include "App.h"
#include "Menu.h"
#include "Quest.h"
#include "SceneManager.h"
#include "MenuManager.h"
#include "QuestManager.h"
#include "DialogueManager.h"
#include "GuiManager.h"
#include "PartyManager.h"

#include "Window.h"
#include "Audio.h"
#include "Fonts.h"
#include "Log.h"


MenuParty::MenuParty() : Menu()
{
	id = MenuID::MENU_PARTY;

	partyMenuImgPath = app->configNode.child("menuManager").child("menuParty").attribute("texturepath").as_string();
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
			InventorySlot* slot = new InventorySlot();
			SDL_Rect rect = { app->menuManager->openBookPos.x + 288 + 26 * j, app->menuManager->openBookPos.y + 48 + 28 * i, 22, 22 };
			slot->button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::I_SLOT, "", 1, rect, this, ButtonType::SMALL);
			slot->item = new Item(app->configNode);
						
			inventorySlotList.Add(slot);
		}
	}

	return true;
}


bool MenuParty::PreUpdate()
{
	ListItem<PartyMember*>* item = app->partyManager->party.start;
	bool active = false;
	while (item != nullptr)
	{
		if (item->data->name.GetString() == "sophie") {
			active = true;
		}
		item = item->next;
	}
	if (!active) { partyMember2Button->state = GuiControlState::DISABLED; }

	return true;
}


bool MenuParty::Update(float dt)
{
	
	return true;
}


bool MenuParty::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = { 0, 0, 519, 311 };
	app->render->DrawTexture(partyMenuImg, app->menuManager->openBookPos.x, app->menuManager->openBookPos.y, &rect, SDL_FLIP_NONE, ScaleType::UI_200, false);

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
		//control->state = ;

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
	uint font = app->menuManager->font4_id;
	PartyMember* memberData = app->partyManager->party.At(app->sceneManager->sceneGame->partyMemberSelected)->data;

	app->fonts->BlitText(465, 230, font, memberData->name.GetString(), ScaleType::UI_200);

	char level[5];
	sprintf_s(level, 5, "%d", memberData->level);
	app->fonts->BlitText(465, 260, font, level, ScaleType::UI_200);

	char currentHP[8];
	sprintf_s(currentHP, 8, "hp %d/", memberData->currentHp);
	app->fonts->BlitText(776, 265, font, currentHP, ScaleType::UI_200);

	char maxHP[3];
	sprintf_s(maxHP, 3, "%d", memberData->maxHp);
	app->fonts->BlitText(824, 265, font, maxHP, ScaleType::UI_200);

	char currentMana[10];
	sprintf_s(currentMana, 10, "mana %d/", memberData->currentMana);
	app->fonts->BlitText(871, 265, font, currentMana, ScaleType::UI_200);

	char maxMana[3];
	sprintf_s(maxMana, 3, "%d", memberData->maxMana);
	app->fonts->BlitText(936, 265, font, maxMana, ScaleType::UI_200);

	char attack[12];
	sprintf_s(attack, 12, "attack %d", memberData->attack);
	app->fonts->BlitText(776, 290, font, attack, ScaleType::UI_200);

	char defense[15];
	sprintf_s(defense, 15, "defense %d", memberData->defense);
	app->fonts->BlitText(871, 290, font, defense, ScaleType::UI_200);

	char speed[10];
	sprintf_s(speed, 10, "speed %d", memberData->speed);
	app->fonts->BlitText(776, 315, font, speed, ScaleType::UI_200);

	char critRate[15];
	sprintf_s(critRate, 15, "crit r %d", memberData->critRate);
	app->fonts->BlitText(871, 315, font, critRate, ScaleType::UI_200);
}