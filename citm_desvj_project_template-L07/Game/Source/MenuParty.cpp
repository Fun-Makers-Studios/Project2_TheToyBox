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

	rectTexture = { 0, 0, 519, 311 };

	//UI
	SDL_Rect rect = { app->menuManager->openBookPos.x + 43, app->menuManager->openBookPos.y + 227, 33, 34 };
	partyMember1Button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::PARTY_1, "", 1, rect, this, ButtonType::SQUARE_M);
	partyMember2Button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::PARTY_2, "", 1, rect, this, ButtonType::SQUARE_M);
	partyMember3Button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::PARTY_3, "", 1, rect, this, ButtonType::SQUARE_M);
	partyMember4Button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::PARTY_4, "", 1, rect, this, ButtonType::SQUARE_M);

	for (int i = 0; i < 4; i++)
		guiControlsList.At(i)->data->bounds.x += i * 43;


	for (int i = 0; i < MAX_INVENTORY_SLOTS_ROWS; i++)
	{
		for (int j = 0; j < MAX_INVENTORY_SLOTS_COLS; j++)
		{
			InventorySlot* slot = new InventorySlot();
			SDL_Rect rect = { app->menuManager->openBookPos.x + 288 + 26 * j, app->menuManager->openBookPos.y + 48 + 28 * i, 22, 22 };
			slot->button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::I_SLOT, "", 1, rect, this, ButtonType::SQUARE_S);
			slot->item = nullptr;
						
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
	
	app->render->DrawTexture(partyMenuImg, app->menuManager->openBookPos.x, app->menuManager->openBookPos.y, &rectTexture, SDL_FLIP_NONE, ScaleType::UI_200, false);

	// PARTY ==========
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


	//INVENTORY ==========
	SDL_Rect rectTexture2 = { 0, 0, 16, 16 };
	ListItem<InventorySlot*>* slot = inventorySlotList.start;

	while (slot != nullptr)
	{
		if (slot->data->item == nullptr)
		{
			slot = slot->next;
			continue;
		}

		if (slot->data->item->texture == nullptr)
		{
			slot = slot->next;
			continue;
		}

		if (slot->data == selectedSlot)
		{
			int mouseX, mouseY;
			app->input->GetMousePosition(mouseX, mouseY);

			app->render->DrawTexture(slot->data->item->texture, mouseX/2, mouseY/2, &rectTexture2, SDL_FLIP_NONE, ScaleType::UI_200, false);
		}
		else
		{
			app->render->DrawTexture(slot->data->item->texture, slot->data->button->bounds.x + 3, slot->data->button->bounds.y + 3, &rectTexture2, SDL_FLIP_NONE, ScaleType::UI_200, false);
		}
		

		slot = slot->next;
	}

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
		ListItem<InventorySlot*>* slot = inventorySlotList.start;

		while (slot != nullptr)
		{
			if (slot->data->button->state == GuiControlState::FOCUSED)
			{
				if (selectedSlot == nullptr)		
				{
					//Select
					selectedSlot = slot->data;
				}				
				else if (selectedSlot != nullptr)
				{
					//Place or Swap
					SWAP(selectedSlot->item, slot->data->item);
					selectedSlot = nullptr;
				}
			}
			
			slot = slot->next;
		}

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
	PartyMember* memberData = app->partyManager->party.At(app->sceneManager->sceneGame->partyMemberSelected)->data;
	int lineNum = 0;

	BlitTextStats(memberData->name.GetString(), lineNum);

	// Header stats

	/*char level[5];
	sprintf_s(level, 5, "%d", memberData->level);
	app->fonts->BlitText(776, 265, font, level, ScaleType::UI_200);

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

	*/


	// Detailed stats
	char maxHP[3];
	sprintf_s(maxHP, 3, "%d", memberData->maxHp);
	lineNum = BlitTextStats(maxHP, lineNum);

	char maxMana[3];
	sprintf_s(maxMana, 3, "%d", memberData->maxMana);
	lineNum = BlitTextStats(maxMana, lineNum);

	char attack[12];
	sprintf_s(attack, 12, "%d", memberData->attack);
	lineNum = BlitTextStats(attack, lineNum);

	char defense[15];
	sprintf_s(defense, 15, "%d", memberData->defense);
	lineNum = BlitTextStats(defense, lineNum);

	char speed[10];
	sprintf_s(speed, 10, "%d", memberData->speed);
	lineNum = BlitTextStats(speed, lineNum);

	char critRate[15];
	sprintf_s(critRate, 15, "%d", memberData->critRate);
	lineNum = BlitTextStats(critRate, lineNum);
}

int MenuParty::BlitTextStats(const char* text, int lineNum)
{
	app->fonts->BlitText(app->menuManager->openBookPos.x + 96, app->menuManager->openBookPos.y + 122 + 11 * lineNum, app->menuManager->font4_id, text, ScaleType::UI_200);
	return ++lineNum;
}