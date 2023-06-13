#include "MenuPause.h"

#include "App.h"
#include "Menu.h"
#include "SceneManager.h"
#include "MenuManager.h"
#include "GuiManager.h"

#include "Window.h"
#include "Audio.h"
#include "Fonts.h"
#include "Log.h"


MenuPause::MenuPause() : Menu()
{
	id = MenuID::MENU_PAUSE;

	imgPausePath = app->configNode.child("menuManager").child("menuPause").attribute("texturepath").as_string();
}


MenuPause::~MenuPause()
{}


bool MenuPause::Awake(pugi::xml_node& config)
{
	LOG("Loading MenuPause");
	bool ret = true;

	return ret;
}


bool MenuPause::Start()
{
	LOG("--STARTS PAUSE MENU--");

	// Load
	imgPause = app->tex->Load(imgPausePath);
	pauseRect = { 0, 0, 247, 330 };

	//UI
	resumeButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::RESUME, "resume", 7, { 197 + 105, 13 + 170, 44, 11 }, this);
	backToTitleButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::BACK_TO_TITLE, "back to title", 13, { 197 + 105, 13 + 191, 44, 11 }, this);
	settingsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::SETTINGS, "settings", 8, { 197 + 105, 13 + 212, 44, 11 }, this);
	saveExitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, (uint32)ControlID::SAVE_EXIT, "save and exit", 12, { 197 + 105, 13 + 233, 44, 11 }, this);
	
	return true;
}


bool MenuPause::PreUpdate()
{

	return true;
}


bool MenuPause::Update(float dt)
{

	return true;
}


bool MenuPause::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(imgPause, 197, 13, &pauseRect, SDL_FLIP_NONE, ScaleType::UI_200, false);


	return ret;
}


bool MenuPause::CleanUp()
{
	LOG("Freeing TITLE SCENE");

	app->tex->UnLoad(imgPause);

	//STORE IN A LIST THIS BUTTONS AND THEN CHECK HERE IF NULLPTR TO CLEAN THEM UP
	//guiControlsList.Clear();

	return true;
}

bool MenuPause::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{
	case (uint32)ControlID::RESUME:
		menuState = MenuState::SWITCH_OFF;
		app->audio->PlayFx(app->menuManager->selectSFX);
		break;

	case (uint32)ControlID::BACK_TO_TITLE:
		app->sceneManager->sceneState = SceneState::SWITCH;
		app->sceneManager->nextScene = SceneID::SCENE_TITLE;
		app->audio->PlayFx(app->menuManager->startSFX);
		break;

	case (uint32)ControlID::SAVE_EXIT:
		//showSavingState = true;
		app->SaveGameRequest();
		app->audio->PlayFx(app->menuManager->selectSFX);
		app->menuManager->exitGame = true;
		break;

	default:
		break;

	}

	return true;
}