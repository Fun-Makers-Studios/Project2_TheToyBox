#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "EntityManager.h"
#include "Map.h"
#include "Collisions.h"
#include "PathFinding.h"
#include "UI.h"
#include "GuiManager.h"
#include "SceneManager.h"
#include "PartyManager.h"
#include "ParticleSystemManager.h"
#include "Debug.h"
#include "SceneGame.h"

#include "Defs.h"
#include "Log.h"

SceneGame::SceneGame() : Scene()
{	
	sceneType = SceneType::GAME;
	id = SceneID::SCENE_GAME;

	/*STORE INFO FROM XML*/
	musicPath = app->configNode.child("scene").child("music").attribute("musicPath").as_string();
	selectSFXPath = app->configNode.child("scene").child("scenesfx").attribute("selectSFXPath").as_string();
	imgPausePath = app->configNode.child("scene").child("imgPause").attribute("imgPausePath").as_string();
	popImg_settingsPath = app->configNode.child("title").child("popImage").attribute("settingtexturepath").as_string();
	partyMenuImgPath = app->configNode.child("scene").child("partyMenuImg").attribute("partyMenuImgPath").as_string();
	zeroImgPath = app->configNode.child("scene").child("zeroImg").attribute("zeroImgPath").as_string();
	sophieImgPath = app->configNode.child("scene").child("sophieImg").attribute("sophieImgPath").as_string();
	saveTexPath = app->configNode.child("scene").child("saveTex").attribute("saveTexPath").as_string();
}

// Destructor
SceneGame::~SceneGame()
{}

// Called before render is available
bool SceneGame::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneGame::Start()
{
	LOG("--STARTS GAME SCENE--");

	dialogueManager = new DialogueManager(this);

	
	// Iterate all objects in the scene
	/*for (pugi::xml_node itemNode = app->configNode.child("scene").child("potionhp"); itemNode; itemNode = itemNode.next_sibling("potionhp"))
	{
		item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
		livesCollectedList.Add(item);
	}*/

	// Load Enemies
	for (pugi::xml_node itemNode = app->configNode.child("scene").child("enemykid"); itemNode; itemNode = itemNode.next_sibling("enemykid"))
	{
		kid = (KidEnemy*)app->entityManager->CreateEntity(EntityType::ENEMY_KID, itemNode);
	}

	
	//Load First Map NPCs
	mapName = "town";
	LoadNPC();
	LoadItems();

	//Instantiate and init the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER, app->configNode.child("scene").child("player"));

	/*INITIALIZE NECESSARY MODULES*/
	app->pathfinding->Enable();
	app->map->Enable();
	app->collisions->Enable();
	app->entityManager->Enable();
	app->debug->debug = false;
	exitGame = false;

	// Load map
	if (app->map->Load())
	{
		int w, h;
		uchar* buffer = NULL;

		if (app->map->CreateWalkabilityMap(w, h, &buffer))
			app->pathfinding->SetMap(w, h, buffer);

		RELEASE_ARRAY(buffer);
	}

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
	saveTex = app->tex->Load(saveTexPath);

	// UI
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
bool SceneGame::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneGame::Update(float dt)
{
	SceneMap();

	if (continueGame)
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
		if (!dialogueManager->dialogueLoaded && !settingSceneMenu && !partyMenu) { gamePaused = !gamePaused; }
		
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

	// Camera movement
	if (app->debug->debug && app->debug->freeCam)
	{
		//Free camera on debug mode
		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)	{ app->render->camera.x -= 8 * dt / 16; }
		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)	{ app->render->camera.x += 8 * dt / 16; }
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)		{ app->render->camera.y -= 8 * dt / 16; }
		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)	{ app->render->camera.y += 8 * dt / 16; }
	}
	else
	{
		// Camera movement related to player's movement
		// HEKATE FixCamera() Modified
		FixCamera();
	}

	// Draw map
	app->map->Draw();

	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		if (smokePS == nullptr) {
			smokePS = app->particleManager->CreateParticleSystem(player->body->pos, Blueprint::SAND);
		}
		else {
			smokePS->TurnOff();
			smokePS = nullptr;
		}
	}

	//Changes to Night Mode
	if(isNight)
		app->render->DrawRectangle(app->render->viewport, 0, 0, 255, 100, true, false);

	ActiveParticles();

	SaveUI();

	//Saves game if choosing the option when talkin with MAGE
	if (dialogueManager->GetCurrentDialogue() != nullptr && dialogueManager->GetCurrentDialogue()->id == 6 && dialogueManager->GetCurrentDialogue()->currentNode->id == 1 && app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		showSavingState = true;
		isNight = !isNight;
		app->SaveGameRequest();
	}

	//Blit UI
	/*app->ui->BlitFPS();

	if (app->debug->debug)
	{
		app->ui->BlitPlayerXPos();
		app->ui->BlitPlayerYPos();
		app->ui->BlitAverageFPS();
		app->ui->BlitDT();
		app->ui->BlitTimeSinceStart();
		app->ui->BlitFrameCount();
	}*/

	return true;
}

// Called each loop iteration
bool SceneGame::PostUpdate()
{
	bool ret = true;

	if (gamePaused && pauseMenu)
		app->render->DrawTexture(img_pause, app->render->camera.x + app->render->camera.w / 2 - pauseRect.w / 2, app->render->camera.y + app->render->camera.h / 2 - pauseRect.h / 2, &pauseRect);

	if(gamePaused && partyMenu)
		app->render->DrawTexture(partyMenuImg, app->render->camera.x, app->render->camera.y - 3, NULL);

	if(gamePaused && pauseMenu && settingSceneMenu)
		app->render->DrawTexture(popImg_settings, app->render->camera.x , app->render->camera.y - 3, NULL);


	if ((gamePaused && dialogueManager->dialogueLoaded) && (pauseMenu == false && partyMenu == false)) 
		dialogueManager->Update();


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

	if (pauseMenu)
	{
		if (resumeButton14->state == GuiControlState::DISABLED) 
			resumeButton14->state = GuiControlState::NORMAL;
		
		if (backToTitleButton15->state == GuiControlState::DISABLED) 
			backToTitleButton15->state = GuiControlState::NORMAL;
		
		if (settingsButton16->state == GuiControlState::DISABLED) 
			settingsButton16->state = GuiControlState::NORMAL;
		
		if (closeButton17->state == GuiControlState::DISABLED) 
			closeButton17->state = GuiControlState::NORMAL;
		
		if (settingSceneMenu)
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
				app->audio->PlayFx(app->sceneManager->sceneTitle->closemenuSFX);
			}
		}
	}

	if (partyMenu)
	{
		if (firstPMemberButton27->state == GuiControlState::DISABLED) 
			firstPMemberButton27->state = GuiControlState::NORMAL;
		
		if (secondPMemberButton28->state == GuiControlState::DISABLED) 
			secondPMemberButton28->state = GuiControlState::NORMAL;
		
		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			partyMenu = !partyMenu;
			app->audio->PlayFx(app->sceneManager->sceneTitle->closemenuSFX);
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

	if ((gamePaused && dialogueManager->dialogueLoaded) && (!pauseMenu && !partyMenu))
		dialogueManager->Draw();


	if (exitGame)
		ret = false;

	return ret;
}

// Called before quitting
bool SceneGame::CleanUp()
{
	LOG("Freeing GAME SCENE");
	app->entityManager->Disable();
	app->pathfinding->Disable();
	app->collisions->Disable();
	app->map->Disable();

	gamePaused = false;
	pauseMenu = false;
	partyMenu = false;

	dialogueManager->CleanUp();

	Mix_ResumeMusic();

	if (img_pause != nullptr)
	{
		app->tex->UnLoad(img_pause);
	}
	if (popImg_settings != nullptr)
	{
		app->tex->UnLoad(popImg_settings);
	}
	if (partyMenuImg != nullptr)
	{
		app->tex->UnLoad(partyMenuImg);
	}
	if (zeroImg != nullptr)
	{
		app->tex->UnLoad(zeroImg);
	}
	if (sophieImg != nullptr)
	{
		app->tex->UnLoad(sophieImg);
	}

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	delete player;
	player = nullptr;

	return true;
}


bool SceneGame::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{
	case 14:
		if (!dialogueManager->dialogueLoaded) { gamePaused = !gamePaused; }
		pauseMenu = !pauseMenu;
		app->audio->PlayFx(app->sceneManager->sceneTitle->menuSelectionSFX);
		break;

	case 15: // Back to title
		pauseMenu = !pauseMenu;
		app->sceneManager->sceneState = SceneState::SWITCH;
		app->sceneManager->nextScene = SceneID::SCENE_TITLE;
		app->audio->PlayFx(app->sceneManager->sceneTitle->startSFX);
		break;

	case 16:
		//Here goes settings menu
		settingSceneMenu = !settingSceneMenu;
		app->audio->PlayFx(app->sceneManager->sceneTitle->menuSelectionSFX);
		break;
	
	case 17:
		showSavingState = true;
		app->SaveGameRequest();
		//exitGame = !exitGame;
		app->audio->PlayFx(app->sceneManager->sceneTitle->menuSelectionSFX);
		break;
	
	case 21:
		// Decrease music volume
		app->musicValue = app->musicValue - 1;
		if (app->musicValue <= 0)
			app->musicValue = 0;
		if (app->musicValue >= 100)
			app->musicValue = 100;
		Mix_VolumeMusic(app->musicValue);
		app->audio->PlayFx(app->sceneManager->sceneTitle->menuSelectionSFX);
		break;

	case 22:
		// Increase music volume
		app->musicValue = app->musicValue + 1;
		if (app->musicValue <= 0)
			app->musicValue = 0;
		if (app->musicValue >= 100)
			app->musicValue = 100;
		Mix_VolumeMusic(app->musicValue);
		app->audio->PlayFx(app->sceneManager->sceneTitle->menuSelectionSFX);
		break;

	case 23:
		// Decrease SFX volume
		app->sfxValue = app->sfxValue - 1;
		if (app->sfxValue <= 0)
			app->sfxValue = 0;
		if (app->sfxValue >= 100)
			app->sfxValue = 100;
		Mix_Volume(-1, app->sfxValue);
		app->audio->PlayFx(app->sceneManager->sceneTitle->menuSelectionSFX);
		break;

	case 24:
		// Increase SFX volume
		app->sfxValue = app->sfxValue + 1;
		if (app->sfxValue <= 0)
			app->sfxValue = 0;
		if (app->sfxValue >= 100)
			app->sfxValue = 100;
		Mix_Volume(-1, app->sfxValue);
		app->audio->PlayFx(app->sceneManager->sceneTitle->menuSelectionSFX);
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
		app->audio->PlayFx(app->sceneManager->sceneTitle->menuSelectionSFX);
		break;

	case 26:
		// V-Sync button
		app->render->limitFPS = !app->render->limitFPS;
		app->audio->PlayFx(app->sceneManager->sceneTitle->menuSelectionSFX);
		break;
	
	case 27:
		// Choose First PartyMember
		partyMemberSelected = 0;
		app->audio->PlayFx(app->sceneManager->sceneTitle->menuSelectionSFX);
		break;
		
	case 28:
		// Choose Second PartyMember
		partyMemberSelected = 1;
		app->audio->PlayFx(app->sceneManager->sceneTitle->menuSelectionSFX);
		break;

	default:
		break;
	}

	return true;
}

void SceneGame::ActiveParticles()
{
	if (isNight)
	{
		if (firefliesPS == nullptr) 
		{
			firefliesPS = app->particleManager->CreateParticleSystem({0, 0}, Blueprint::FIREFLIES);
		}
		
		
		
	}
	else
	{

		if (firefliesPS != nullptr)
		{
			firefliesPS->TurnOff();
			firefliesPS = nullptr;
		}

		

	}
}

void SceneGame::SaveUI()
{
	if (showSavingState == true) {
		if (saveTime < 75) {
			app->render->DrawTexture(saveTex, app->render->camera.x + (app->render->camera.w - 100) , app->render->camera.y + (app->render->camera.h - 100), NULL);
			saveTime++;
		}
		else
		{
			showSavingState = false;
			saveTime = 0;
		}
	}
}

void SceneGame::ResetScene()
{
	app->audio->PlayMusic(musicPath, 1.0f);

	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	if (saveEnabled == false || result == NULL) {
		saveEnabled = false;
		player->ResetPlayerPos();
	}
	else if (saveEnabled == true && result != NULL) {
		app->LoadGameRequest();
	}
}

void SceneGame::SceneMap()
{
	if (isMapChanging)
	{		
		app->map->ChangeMap(mapName.GetString());
		player->body->pos = player->newPos;
		LoadNPC();
		LoadItems();

		isMapChanging = false;
	}
}

void SceneGame::LoadNPC()
{
	ListItem<NPC*>* npcItem;

	for (npcItem = npcList.start; npcItem != NULL; npcItem = npcItem->next)
	{
		npcItem->data->needToDestroy = true;
	}
	
	for (pugi::xml_node itemNode = app->configNode.child("npcs").child(mapName.GetString()).child("npc"); itemNode; itemNode = itemNode.next_sibling("npc"))
	{
		npc = (NPC*)app->entityManager->CreateEntity(EntityType::NPC, itemNode);
		npcList.Add(npc);
		npc->Start();
	}
	
}

void SceneGame::LoadItems()
{
	ListItem<Item*>* itemsItem;

	for (itemsItem = itemsList.start; itemsItem != NULL; itemsItem = itemsItem->next)
	{
		itemsItem->data->needToDestroy = true;
	}
	
	// Iterate all objects in the scene
	for (pugi::xml_node itemNode = app->configNode.child("items").child(mapName.GetString()).child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, itemNode);
		itemsList.Add(item);
		item->Start();
	}
	
}

void SceneGame::FixCamera()
{
	// HEKATE width/height in TILES (townMap 55x36)
	if (app->map->mapData.width >= 55 && app->map->mapData.height >= 36)	//SMALL MAP SIZE 1280x704
	{
		uint scale = app->scaleObj->ScaleTypeToInt(ScaleType::WORLD);

		app->render->camera.x = player->body->pos.x * scale - app->win->screenSurface->w / 2;
		app->render->camera.y = player->body->pos.y * scale - app->win->screenSurface->h / 2;

		if (app->render->camera.x < 0)
			app->render->camera.x = 0;
		if (app->render->camera.y < 0)
			app->render->camera.y = 0;
		
		if (app->render->camera.x + app->render->camera.w > app->map->mapData.width * app->map->mapData.tileWidth * scale)
			app->render->camera.x = app->map->mapData.width * app->map->mapData.tileWidth * scale - app->render->camera.w;
		if (app->render->camera.y + app->render->camera.h > app->map->mapData.height * app->map->mapData.tileHeight * scale)
			app->render->camera.y = app->map->mapData.height * app->map->mapData.tileHeight * scale - app->render->camera.h;
	}
	else
	{
		app->render->camera.x = 0;
		app->render->camera.y = 0;
	}
}

void SceneGame::FightKid()
{
	app->partyManager->enemyToFight = "enemykid";
	app->sceneManager->sceneState = SceneState::SWITCH;
	app->sceneManager->nextScene = SceneID::SCENE_FIGHT;
}

bool SceneGame::LoadState(pugi::xml_node& data)
{
	// HEKATE
	// Load previous saved player position
	//iPoint playerpos = { data.child("playerposition").attribute("x").as_float(), data.child("playerposition").attribute("y").as_float() };
	//app->scene->player->pbody->body->settransform(playerpos, 0);

	ListItem<PartyMember*>* pmemberItem;
	for (pmemberItem = app->partyManager->party.start; pmemberItem != NULL; pmemberItem = pmemberItem->next)
	{
		pugi::xml_node partyMember = data.append_child("partymember");
		pmemberItem->data->name = data.child("partymember").attribute("name").as_string();
		pmemberItem->data->maxHp = data.child("partymember").attribute("maxHp").as_uint();
		pmemberItem->data->maxMana = data.child("partymember").attribute("maxMana").as_uint();
		pmemberItem->data->currentHp = data.child("partymember").attribute("currentHp").as_uint();
		pmemberItem->data->level = data.child("partymember").attribute("level").as_uint();
		pmemberItem->data->attack = data.child("partymember").attribute("attack").as_uint();
		pmemberItem->data->defense = data.child("partymember").attribute("defense").as_uint();
		pmemberItem->data->speed = data.child("partymember").attribute("speed").as_uint();
		pmemberItem->data->critRate = data.child("partymember").attribute("critRate").as_uint();
		pmemberItem->data->fightPosition.x = data.child("partymember").attribute("fightPosX").as_int();
		pmemberItem->data->fightPosition.y = data.child("partymember").attribute("fightPosY").as_int();
	}

	//Load previous saved player number of lives
	saveEnabled = data.child("checkpointEnabled").attribute("checkpointEnabled").as_bool();

	//mapName = data.child("mapname").attribute("mapname").as_string();

	//// load previous saved bat position
	//iPoint kidpos = { data.child("kidposition").attribute("x").as_float(), data.child("kidposition").attribute("y").as_float() };
	//app->scene->kid->pbody->body->settransform(kidpos, 0);

	return true;
}

bool SceneGame::SaveState(pugi::xml_node& data)
{

	ListItem<PartyMember*>* pmemberItem;

	for (pmemberItem = app->partyManager->party.start; pmemberItem != NULL; pmemberItem = pmemberItem->next)
	{
		pugi::xml_node partyMember = data.append_child("partymember");
		partyMember.append_attribute("name") = pmemberItem->data->name.GetString();
		partyMember.append_attribute("maxHP") = pmemberItem->data->maxHp;
		partyMember.append_attribute("maxMana") = pmemberItem->data->maxMana;
		partyMember.append_attribute("currentHp") = pmemberItem->data->currentHp;
		partyMember.append_attribute("level") = pmemberItem->data->level;
		partyMember.append_attribute("attack") = pmemberItem->data->attack;
		partyMember.append_attribute("defense") = pmemberItem->data->defense;
		partyMember.append_attribute("speed") = pmemberItem->data->speed;
		partyMember.append_attribute("critRate") = pmemberItem->data->critRate;
		partyMember.append_attribute("fightPosX") = pmemberItem->data->fightPosition.x;
		partyMember.append_attribute("fightPosY") = pmemberItem->data->fightPosition.y;
	}

	// HEKATE
	// Save current player position
	//pugi::xml_node playerPos = data.append_child("playerPosition");
	//playerPos.append_attribute("x") = app->scene->player->pbody->body->GetTransform().p.x;
	//playerPos.append_attribute("y") = app->scene->player->pbody->body->GetTransform().p.y;
	//
	//// Save current player number of coins
	//pugi::xml_node itemLives = data.append_child("itemLives");
	//itemLives.append_attribute("itemLives") = itemLives;
	//
	//// Save current player number of coins
	//pugi::xml_node checkpointEnabled = data.append_child("checkpointEnabled");
	//checkpointEnabled.append_attribute("checkpointEnabled") = checkpointEnabled;

	//// Save current bat position
	//pugi::xml_node kidPos = data.append_child("kidPosition");
	//kidPos.append_attribute("x") = app->scene->kid->pbody->body->GetTransform().p.x;
	//kidPos.append_attribute("y") = app->scene->kid->pbody->body->GetTransform().p.y;
	//
	//pugi::xml_node checkPoint = data.append_child("checkPoint");
	//checkPoint.append_attribute("checkPoint") = app->scene->saveEnabled;
	//
	//pugi::xml_node actualMapName = data.append_child("mapName");
	//actualMapName.append_attribute("mapName") = mapName.GetString();

	return true;
}