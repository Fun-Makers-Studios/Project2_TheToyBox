#include "MenuParty.h"

#include "App.h"
#include "Menu.h"
#include "Quest.h"
#include "SceneManager.h"
#include "MenuManager.h"
#include "QuestManager.h"
#include "DialogueManager.h"
#include "GuiManager.h"
#include "UI.h"

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
	//sophieImgPath = app->configNode.child("menuManager").child("sophieImg").attribute("sophieImgPath").as_string();
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
	app->ui->BlitPartyStats();

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