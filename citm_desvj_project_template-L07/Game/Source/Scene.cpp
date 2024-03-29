#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "Physics.h"
#include "PathFinding.h"
#include "ModuleFadeToBlack.h"
#include "EndingScreen.h"
#include "UI.h"
#include "GuiManager.h"
#include "TitleScreen.h"
#include "PartyManager.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	
	name.Create("scene");
	
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{

	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	dialogueManager = new DialogueManager(this);

	/*STORE INFO FROM XML*/
	musicPath = app->configNode.child("scene").child("music").attribute("musicPath").as_string();
	selectSFXPath = app->configNode.child("scene").child("scenesfx").attribute("selectSFXPath").as_string();
	imgPausePath = app->configNode.child("scene").child("imgPause").attribute("imgPausePath").as_string();
	popImg_settingsPath = app->configNode.child("title").child("popImage").attribute("settingtexturepath").as_string();
	partyMenuImgPath = app->configNode.child("scene").child("partyMenuImg").attribute("partyMenuImgPath").as_string();
	zeroImgPath = app->configNode.child("scene").child("zeroImg").attribute("zeroImgPath").as_string();
	sophieImgPath = app->configNode.child("scene").child("sophieImg").attribute("sophieImgPath").as_string();

	// Iterate all objects in the scene
	
	
	/*for (pugi::xml_node itemNode = app->configNode.child("scene").child("potionhp"); itemNode; itemNode = itemNode.next_sibling("potionhp"))
	{
		item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
		livesCollectedList.Add(item);
	}*/

	for (pugi::xml_node itemNode = app->configNode.child("scene").child("enemykid"); itemNode; itemNode = itemNode.next_sibling("enemykid"))
	{
		kid = (KidEnemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		kid->parameters = itemNode;
	}

	/*INITIALIZE NECESSARY MODULES*/
	app->physics->Enable();
	app->pathfinding->Enable();
	app->entityManager->Enable();
	app->map->Enable();
	LOG("--STARTS GAME SCENE--");
	app->physics->debug = false;
	exitGame = false;

	//Load First Map NPCs
	mapName = "town";
	LoadNPC(mapName);

	//Instantiate and init the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = app->configNode.child("scene").child("player");
	player->Start();

	// L03: DONE: Load map
	app->map->Load();
	
	// L12 Create walkability map
	/*if (app->map->Load()) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}*/

	// Play level music
	app->audio->PlayMusic(musicPath, 1.0f);

	// Loading set of SFX
	selectSFX = app->audio->LoadFx(selectSFXPath);

	img_pause = app->tex->Load(imgPausePath);
	pauseRect = {35, 69, 310, 555};
	popImg_settings = app->tex->Load(popImg_settingsPath);
	partyMenuImg = app->tex->Load(partyMenuImgPath);
	zeroImg = app->tex->Load(zeroImgPath);
	sophieImg = app->tex->Load(sophieImgPath);


	// L15: TODO 2: Declare a GUI Button and create it using the GuiManager
	uint w, h;
	app->win->GetWindowSize(w, h);
	resumeButton14 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 14, "resume", 7, { 515, 295, 252, 76 }, this);
	backToTitleButton15 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 15, "back to title", 13, { 515, 375, 252, 76 }, this);
	settingsButton16 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 16, "settings", 8, { 515, 455, 252, 76 }, this);
	closeButton17 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 17, "save and exit", 12, { 515, 535, 252, 76 }, this);

	decreaseMusicButton21 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 21, "decrease", 9, { 325, 200, 252, 76 }, this);
	increaseMusicButton22 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 22, "increase", 9, { 700, 200, 252, 76 }, this);

	decreaseSFXButton23 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 23, "decrease", 9, { 325, 315, 252, 76 }, this);
	increaseSFXButton24 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 24, "increase", 9, { 700, 315, 252, 76 }, this);

	fullscreenButton25 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 25, "", 1, { 520, 424, 252, 76 }, this);

	vsyncButton26 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 26, "", 1, { 520, 532, 252, 76 }, this);

	firstPMemberButton27 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 27, "z", 2, { 176, 140, 65, 76 }, this, ButtonType::SMALL);
	secondPMemberButton28 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 28, "s", 2, { 176, 216, 64, 76 }, this, ButtonType::SMALL);

	ResetScene();

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	SceneMap();

	if (continueGame == true)
	{
		app->LoadGameRequest();
		app->audio->PlayFx(selectSFX);
		continueGame = false;
	}

	//TEST FIGHT SCENE
	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
		LOG("SWITCHING TO SCENEFIGHT");
		FightKid();
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		if (!dialogueManager->dialogueLoaded && settingSceneMenu == false && partyMenu == false) { gamePaused = !gamePaused; }
		
		if(settingSceneMenu == false)
			pauseMenu = !pauseMenu;
	
		Mix_PauseMusic();
	}

	if (gamePaused != true)
	{
		Mix_ResumeMusic();

		// God Mode key
		/*if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			player->godMode = !player->godMode;
			app->audio->PlayFx(selectSFX);
		}*/
	}
	
	if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN && !dialogueManager->dialogueLoaded)
	{
		
			gamePaused = !gamePaused;
			partyMenu = !partyMenu;
			app->audio->PlayFx(selectSFX);
		
	}

	// Camera movement related to player's movement
	FixCamera();

	// Draw map
	app->map->Draw();

	Checkpoint();
		
	SaveUI();

	//Blit UI
	app->ui->BlitFPS();

	if (app->physics->debug)
	{
		app->ui->BlitPlayerXPos();
		app->ui->BlitPlayerYPos();
		app->ui->BlitAverageFPS();
		app->ui->BlitDT();
		app->ui->BlitTimeSinceStart();
		app->ui->BlitFrameCount();
	}
	
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (gamePaused == true && pauseMenu == true)
		app->render->DrawTexture(img_pause, app->render->camera.x + app->render->camera.w / 2 - pauseRect.w / 2, app->render->camera.y + app->render->camera.h / 2 - pauseRect.h / 2, &pauseRect);

	if(gamePaused == true && partyMenu == true)
		app->render->DrawTexture(partyMenuImg, app->render->camera.x, app->render->camera.y - 3, NULL);

	if(gamePaused == true && pauseMenu == true && settingSceneMenu == true)
		app->render->DrawTexture(popImg_settings, app->render->camera.x , app->render->camera.y - 3, NULL);


	if ((gamePaused && dialogueManager->dialogueLoaded) && (pauseMenu == false && partyMenu == false)) {
		dialogueManager->Update();
	}

	// Draw GUI
	app->guiManager->Draw();

	resumeButton14->state = GuiControlState::DISABLED;
	backToTitleButton15->state = GuiControlState::DISABLED;
	settingsButton16->state = GuiControlState::DISABLED;
	closeButton17->state = GuiControlState::DISABLED;

	// Setting Menu
	decreaseMusicButton21->state = GuiControlState::DISABLED;
	increaseMusicButton22->state = GuiControlState::DISABLED;
	decreaseSFXButton23->state = GuiControlState::DISABLED;
	increaseSFXButton24->state = GuiControlState::DISABLED;
	fullscreenButton25->state = GuiControlState::DISABLED;
	vsyncButton26->state = GuiControlState::DISABLED;

	firstPMemberButton27->state = GuiControlState::DISABLED;
	secondPMemberButton28->state = GuiControlState::DISABLED;

	if (pauseMenu == true)
	{

		if (resumeButton14->state == GuiControlState::DISABLED) {
			resumeButton14->state = GuiControlState::NORMAL;
		}
		if (backToTitleButton15->state == GuiControlState::DISABLED) {
			backToTitleButton15->state = GuiControlState::NORMAL;
		}
		if (settingsButton16->state == GuiControlState::DISABLED) {
			settingsButton16->state = GuiControlState::NORMAL;
		}
		if (closeButton17->state == GuiControlState::DISABLED) {
			closeButton17->state = GuiControlState::NORMAL;
		}

		if (settingSceneMenu == true)
		{
			resumeButton14->state = GuiControlState::DISABLED;
			backToTitleButton15->state = GuiControlState::DISABLED;
			settingsButton16->state = GuiControlState::DISABLED;
			closeButton17->state = GuiControlState::DISABLED;

		
			decreaseMusicButton21->state = GuiControlState::NORMAL;
			increaseMusicButton22->state = GuiControlState::NORMAL;
			decreaseSFXButton23->state = GuiControlState::NORMAL;
			increaseSFXButton24->state = GuiControlState::NORMAL;
			fullscreenButton25->state = GuiControlState::NORMAL;
			vsyncButton26->state = GuiControlState::NORMAL;

			char music[10];
			sprintf_s(music, 10, "%d", app->musicValue);
			app->fonts->BlitText(630, 245, app->ui->font1_id, music);

			char sfx[10];
			sprintf_s(sfx, 10, "%d", app->sfxValue);
			app->fonts->BlitText(630, 362, app->ui->font1_id, sfx);

			char fullscreen[10];
			sprintf_s(fullscreen, 10, "%s", app->win->fullscreenMode ? "on" : "off");
			app->fonts->BlitText(632, 458, app->ui->font1_id, fullscreen);

			char vsync[10];
			sprintf_s(vsync, 10, "%s", app->render->limitFPS ? "on" : "off");
			app->fonts->BlitText(632, 565, app->ui->font1_id, vsync);


			if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
			{
				settingSceneMenu = !settingSceneMenu;
				app->audio->PlayFx(app->titlescreen->closemenuSFX);
			}
		}


	}

	if (partyMenu == true)
	{

		if (firstPMemberButton27->state == GuiControlState::DISABLED) {
			firstPMemberButton27->state = GuiControlState::NORMAL;
		}
		if (secondPMemberButton28->state == GuiControlState::DISABLED) {
			secondPMemberButton28->state = GuiControlState::NORMAL;
		}

		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			partyMenu = !partyMenu;
			app->audio->PlayFx(app->titlescreen->closemenuSFX);
		}

		switch (partyMemberSelected)
		{
		case 0:
			// Zero
			app->render->DrawTexture(zeroImg, app->render->camera.x + 290, app->render->camera.y + 207, NULL);
			break;

		case 1:
			// Sophie
			app->render->DrawTexture(sophieImg, app->render->camera.x + 290, app->render->camera.y + 207, NULL);
			
			break;
		default:
			break;
		}
		app->ui->BlitPartyStats();

	}

	if ((gamePaused && dialogueManager->dialogueLoaded) && (pauseMenu == false && partyMenu == false)) {
		dialogueManager->Draw();
	}

	if (exitGame == true)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing GAME SCENE");
	app->entityManager->Disable();
	app->pathfinding->Disable();
	app->physics->Disable();
	app->map->Disable();
	
	//app->guiManager->guiControlsList.Clear();
	gamePaused = false;
	pauseMenu = false;
	partyMenu = false;

	dialogueManager->CleanUp();

	Mix_ResumeMusic();

	app->tex->UnLoad(img_pause);
	app->tex->UnLoad(popImg_settings);
	app->tex->UnLoad(partyMenuImg);
	app->tex->UnLoad(zeroImg);
	app->tex->UnLoad(sophieImg);

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	if(resumeButton14 != nullptr)
		resumeButton14->state = GuiControlState::DISABLED;
	if(backToTitleButton15 != nullptr)
		backToTitleButton15->state = GuiControlState::DISABLED;
	if(settingsButton16 != nullptr)
		settingsButton16->state = GuiControlState::DISABLED;
	if(closeButton17 != nullptr)
		closeButton17->state = GuiControlState::DISABLED;

	if (decreaseMusicButton21 != nullptr)
		decreaseMusicButton21->state = GuiControlState::DISABLED;
	if (increaseMusicButton22 != nullptr)
		increaseMusicButton22->state = GuiControlState::DISABLED;
	if (decreaseSFXButton23 != nullptr)
		decreaseSFXButton23->state = GuiControlState::DISABLED;
	if (increaseSFXButton24 != nullptr)
		increaseSFXButton24->state = GuiControlState::DISABLED;
	if (fullscreenButton25 != nullptr)
		fullscreenButton25->state = GuiControlState::DISABLED;
	if (vsyncButton26 != nullptr)
		vsyncButton26->state = GuiControlState::DISABLED;

	if (firstPMemberButton27 != nullptr)
		firstPMemberButton27->state = GuiControlState::DISABLED;
	if (secondPMemberButton28 != nullptr)
		secondPMemberButton28->state = GuiControlState::DISABLED;

	return true;
}


bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: TODO 5: Implement the OnGuiMouseClickEvent method
	switch (control->id)
	{
	case 14:
		if (!dialogueManager->dialogueLoaded) { gamePaused = !gamePaused; }
		pauseMenu = !pauseMenu;
		app->audio->PlayFx(app->titlescreen->menuSelectionSFX);
		break;

	case 15:
		app->fade->FadeToBlack(this, (Module*)app->titlescreen, 0);
		app->audio->PlayFx(app->titlescreen->startSFX);
		break;

	case 16:
		//Here goes settings menu
		settingSceneMenu = !settingSceneMenu;
		app->audio->PlayFx(app->titlescreen->menuSelectionSFX);
		break;
	
	case 17:
		app->SaveGameRequest();
		exitGame = !exitGame;
		app->audio->PlayFx(app->titlescreen->menuSelectionSFX);
		break;
	
	case 21:
		// Decrease music volume
		app->musicValue = app->musicValue - 1;
		if (app->musicValue <= 0)
			app->musicValue = 0;
		if (app->musicValue >= 100)
			app->musicValue = 100;
		Mix_VolumeMusic(app->musicValue);
		app->audio->PlayFx(app->titlescreen->menuSelectionSFX);
		break;

	case 22:
		// Increase music volume
		app->musicValue = app->musicValue + 1;
		if (app->musicValue <= 0)
			app->musicValue = 0;
		if (app->musicValue >= 100)
			app->musicValue = 100;
		Mix_VolumeMusic(app->musicValue);
		app->audio->PlayFx(app->titlescreen->menuSelectionSFX);
		break;

	case 23:
		// Decrease SFX volume
		app->sfxValue = app->sfxValue - 1;
		if (app->sfxValue <= 0)
			app->sfxValue = 0;
		if (app->sfxValue >= 100)
			app->sfxValue = 100;
		Mix_Volume(-1, app->sfxValue);
		app->audio->PlayFx(app->titlescreen->menuSelectionSFX);
		break;

	case 24:
		// Increase SFX volume
		app->sfxValue = app->sfxValue + 1;
		if (app->sfxValue <= 0)
			app->sfxValue = 0;
		if (app->sfxValue >= 100)
			app->sfxValue = 100;
		Mix_Volume(-1, app->sfxValue);
		app->audio->PlayFx(app->titlescreen->menuSelectionSFX);
		break;

	case 25:
		// Fullscreen button
		app->win->fullscreenMode = !app->win->fullscreenMode;
		if (app->win->fullscreenMode == true)
		{
			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
		}
		else if (app->win->fullscreenMode == false)
		{
			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_SHOWN);
		}
		app->audio->PlayFx(app->titlescreen->menuSelectionSFX);
		break;

	case 26:
		// V-Sync button
		app->render->limitFPS = !app->render->limitFPS;
		app->audio->PlayFx(app->titlescreen->menuSelectionSFX);
		break;
	
	case 27:
		// Choose First PartyMember
		partyMemberSelected = 0;
		app->audio->PlayFx(app->titlescreen->menuSelectionSFX);
		break;
		
	case 28:
		// Choose Second PartyMember
		partyMemberSelected = 1;
		app->audio->PlayFx(app->titlescreen->menuSelectionSFX);
		break;

	default:
		break;
	}

	return true;
}

void Scene::SaveUI()
{
	if (app->saveGameRequested == true) {
		showSavingState = true;
	}
	if (showSavingState == true) {
		if (saveTime < 50) {
			uint w, h;
			SDL_Rect rect_2 = { 64, 0, 32, 32 };
			app->win->GetWindowSize(w, h);
			app->render->DrawTexture(checkPointTex, w - 100, h - 100, &rect_2, SDL_FLIP_HORIZONTAL, ScaleType::NORMAL, 0);
			saveTime++;
			LOG("SAVETIME: %d", saveTime);
		}
		else {
			showSavingState = false;
			saveTime = 0;
		}

	}
}

void Scene::Checkpoint()
{
	//Checkpoint
	if (checkpointEnabled == false) {
		SDL_Rect rect_1 = { 32, 0, 32, 32 };
		app->render->DrawTexture(checkPointTex, 62 * 32, 10 * 32, &rect_1);
	}
	else
	{
		SDL_Rect rect_1 = { 0, 0, 32, 32 };
		app->render->DrawTexture(checkPointTex, 62 * 32, 10 * 32, &rect_1);
	}
}

void Scene::ResetScene()
{
	app->audio->PlayMusic(musicPath, 1.0f);

	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	if (checkpointEnabled == false || result == NULL) {
		checkpointEnabled = false;
		player->ResetPlayerPos();
	}
	else if (checkpointEnabled == true && result != NULL) {
		app->LoadGameRequest();
	}
}

void Scene::SceneMap()
{
	if (isMapChanging == true)
	{
		
		app->map->ChangeMap(mapName.GetString());
		player->pbody->body->SetTransform(PIXEL_TO_METERS(player->newPos), 0.0f);
		LoadNPC(mapName.GetString());

		isMapChanging = false;
	}

}

void Scene::LoadNPC(SString mapName_)
{
	ListItem<NPC*>* npcItem;

	for (npcItem = npcList.start; npcItem != NULL; npcItem = npcItem->next)
	{
		npcItem->data->needToDestroy = true;
	}
	
	for (pugi::xml_node itemNode = app->configNode.child("npcs").child(mapName_.GetString()).child("npc"); itemNode; itemNode = itemNode.next_sibling("npc"))
	{
		npc = (NPC*)app->entityManager->CreateEntity(EntityType::NPC);
		npc->parameters = itemNode;
		npcList.Add(npc);
		npc->Start();
	}
	
}

void Scene::FixCamera()
{
	if (METERS_TO_PIXELS(app->map->mapData.width) > 1280 && METERS_TO_PIXELS(app->map->mapData.height) > 704)	//SMALL MAP SIZE 1280x704
	{
		uint scale = app->scaleObj->ScaleTypeToInt(ScaleType::WORLD);

		app->render->camera.x = (player->position.x * scale + 16) - ((app->win->screenSurface->w) / 2);
		app->render->camera.y = (player->position.y * scale + 16) - ((app->win->screenSurface->h) / 2);

		if (app->render->camera.x < 0)
			app->render->camera.x = 0;
		if (app->render->camera.y < 0)
			app->render->camera.y = 0;

		if (app->render->camera.x > METERS_TO_PIXELS(app->map->mapData.width) * scale - app->render->camera.w)
			app->render->camera.x = METERS_TO_PIXELS(app->map->mapData.width) * scale - app->render->camera.w;
		if (app->render->camera.y > METERS_TO_PIXELS(app->map->mapData.height) * scale - app->render->camera.h)
			app->render->camera.y = METERS_TO_PIXELS(app->map->mapData.height) * scale - app->render->camera.h;
	}
	else {
		app->render->camera.x = 0;
		app->render->camera.y = 0;
	}
}

void Scene::FightKid()
{
	app->partyManager->enemyToFight = "enemykid";
	app->fade->FadeToBlack(this, (Module*)app->sceneFight, 0);
}

bool Scene::LoadState(pugi::xml_node& data)
{
	// Load previous saved player position
	b2Vec2 playerPos = { data.child("playerPosition").attribute("x").as_float(), data.child("playerPosition").attribute("y").as_float() };
	app->scene->player->pbody->body->SetTransform(playerPos, 0);

	//Load previous saved player number of lives
	checkpointEnabled = data.child("checkpointEnabled").attribute("checkpointEnabled").as_bool();

	mapName = data.child("mapName").attribute("mapName").as_string();

	// Load previous saved bat position
	b2Vec2 kidPos = { data.child("kidPosition").attribute("x").as_float(), data.child("kidPosition").attribute("y").as_float() };
	app->scene->kid->pbody->body->SetTransform(kidPos, 0);

	return true;
}

bool Scene::SaveState(pugi::xml_node& data)
{
	// Save current player position
	pugi::xml_node playerPos = data.append_child("playerPosition");
	playerPos.append_attribute("x") = app->scene->player->pbody->body->GetTransform().p.x;
	playerPos.append_attribute("y") = app->scene->player->pbody->body->GetTransform().p.y;
	
	// Save current player number of coins
	pugi::xml_node itemLives = data.append_child("itemLives");
	itemLives.append_attribute("itemLives") = itemLives;
	
	// Save current player number of coins
	pugi::xml_node checkpointEnabled = data.append_child("checkpointEnabled");
	checkpointEnabled.append_attribute("checkpointEnabled") = checkpointEnabled;

	// Save current bat position
	pugi::xml_node kidPos = data.append_child("kidPosition");
	kidPos.append_attribute("x") = app->scene->kid->pbody->body->GetTransform().p.x;
	kidPos.append_attribute("y") = app->scene->kid->pbody->body->GetTransform().p.y;
	
	pugi::xml_node checkPoint = data.append_child("checkPoint");
	checkPoint.append_attribute("checkPoint") = app->scene->checkpointEnabled;
	
	pugi::xml_node actualMapName = data.append_child("mapName");
	actualMapName.append_attribute("mapName") = mapName.GetString();

	return true;
}


