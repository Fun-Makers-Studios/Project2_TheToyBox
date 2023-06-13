#include "MenuFight.h"

#include "App.h"
#include "Menu.h"
#include "SceneManager.h"
#include "MenuManager.h"
#include "GuiManager.h"

#include "Window.h"
#include "Audio.h"
#include "Fonts.h"
#include "Log.h"


MenuFight::MenuFight() : Menu()
{
	id = MenuID::MENU_FIGHT;

	imgPausePath = app->configNode.child("menuManager").child("imgPause").attribute("imgPausePath").as_string();
}


MenuFight::~MenuFight()
{}


bool MenuFight::Awake(pugi::xml_node& config)
{
	LOG("|| AWAKE MenuFight ||");
	bool ret = true;

	return ret;
}


bool MenuFight::Start()
{
	LOG("--START MenuFight--");

	// Load
	imgPause = app->tex->Load(imgPausePath);
	pauseRect = { 35, 69, 310, 555 };

	//UI
	attackButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::ATTACK, "attack", 7, { 50, 300, 126, 38 }, this);
	specialButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::SPECIAL, "special", 8, { 255, 300, 126, 38 }, this);
	bagButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::BAG, "bag", 4, { 457, 300, 126, 38 }, this);

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


bool MenuFight::PreUpdate()
{
	/*if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		app->audio->PlayFx(app->menuManager->closeMenuSFX);
		app->menuManager->SelectMenu();
	}*/
	return true;
}


bool MenuFight::Update(float dt)
{
	//app->render->DrawRectangle({ 0, 0, app->render->camera.w, app->render->camera.w }, 0, 0, 0, 128, true, false, true);

	app->render->DrawTexture(imgPause,
		app->render->camera.x + app->render->camera.w / 2 - pauseRect.w / 2,
		app->render->camera.y + app->render->camera.h / 2 - pauseRect.h / 2, &pauseRect);

	return true;
}


bool MenuFight::PostUpdate()
{
	bool ret = true;

	// Draw GUI
	app->guiManager->Draw();

	return ret;
}


bool MenuFight::CleanUp()
{
	LOG("Freeing TITLE SCENE");

	app->tex->UnLoad(imgPause);

	//STORE IN A LIST THIS BUTTONS AND THEN CHECK HERE IF NULLPTR TO CLEAN THEM UP
	//guiControlsList.Clear();

	return true;
}

bool MenuFight::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{
	case (uint32)ControlID::ATTACK:
		app->sceneManager->sceneFight->turnStep = TurnStep::IN;
		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case (uint32)ControlID::SPECIAL:
		
		app->audio->PlayFx(app->menuManager->startSFX);
		app->sceneManager->sceneFight->turnStep = TurnStep::IN;
		break;

	case (uint32)ControlID::BAG:
		
		app->audio->PlayFx(app->menuManager->openMenuSFX);
		break;

	default:
		break;

	}

	return true;
}