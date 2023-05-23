#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"

#include "EntityManager.h"
#include "PuzzleManager.h"
#include "QuestManager.h"
#include "MenuManager.h"
#include "TransitionManager.h"
#include "Map.h"
#include "Collisions.h"
#include "PathFinding.h"
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
	id = SceneID::SCENE_GAME;

	/*STORE INFO FROM XML*/
	musicPath = app->configNode.child("scene").child("music").attribute("musicPath").as_string();
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
	LoadPuzzles();

	//Instantiate and init the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER, app->configNode.child("scene").child("player"));

	/*INITIALIZE NECESSARY MODULES*/
	app->pathfinding->Enable();
	app->map->Enable();
	app->collisions->Enable();
	app->entityManager->Enable();
	app->puzzleManager->Enable();
	app->questManager->Enable();
	app->particleManager->Enable();
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

	// Load tex
	saveTex = app->tex->Load(saveTexPath);


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

	//TEST FIGHT SCENE - HEKATE!!!
	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
		LOG("SWITCHING TO SCENEFIGHT");
		FightKid();
	}

	// HEKATE
	/*if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		if (!dialogueManager->dialogueLoaded && !settingSceneMenu && !partyMenu) { gamePaused != gamePaused; }
		
		if(!settingSceneMenu)
			pauseMenu = !pauseMenu;
	
		Mix_PauseMusic();
	}

	if (!gamePaused)
	{
		Mix_ResumeMusic();
	}
	
	if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN && !dialogueManager->dialogueLoaded)
	{
		gamePaused != gamePaused;
		partyMenu != partyMenu;
		app->audio->PlayFx(selectSFX);
	}*/
	/*if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN && !dialogueManager->dialogueLoaded)
	{
		gamePaused != gamePaused;
		questMenu != questMenu;
		app->audio->PlayFx(selectSFX);
	}*/

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
		FixCamera();
	}

	// Draw map
	app->map->Draw(false);

	ActiveParticles();

	SaveUI();

	//Saves game if choosing the option when talkin with MAGE
	if (dialogueManager->GetCurrentDialogue() != nullptr 
		&& dialogueManager->GetCurrentDialogue()->id == 6 
		&& dialogueManager->GetCurrentDialogue()->currentNode->id == 1 
		&& app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		Item* addItem = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, app->configNode.child("itemsMap").child(mapName.GetString()).child("item"));
		addItem->takeItem = true;
		showSavingState = true;
		isNight = !isNight;
		app->SaveGameRequest();
	}

	// Lower music volume
	if (app->menuManager->currentMenu == app->menuManager->menuPause) {

		if (app->musicValue >= 20)
			Mix_VolumeMusic(20);
	}
	else
	{
		Mix_VolumeMusic(app->musicValue);
	}

	return true;
}

// Called each loop iteration
bool SceneGame::PostUpdate()
{
	bool ret = true;	

	app->map->Draw(true);

	//Changes to Night Mode
	if (mapName == "town" && isNight)
		app->render->DrawRectangle(app->render->viewport, 0, 0, 255, 100, true, false);

	// HEKATE
	if (/*gamePaused &&*/ dialogueManager->dialogueLoaded)
	{
		if (dialogueManager->Update())
			dialogueManager->Draw();
	}


	if (exitGame == true)
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
	app->questManager->Disable();
	app->puzzleManager->Disable();
	app->map->Disable();

	//HEKATE
	gamePaused = false;
	pauseMenu = false;
	partyMenu = false;
	questMenu = false;

	dialogueManager->CleanUp();

	Mix_ResumeMusic();

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	delete player;
	player = nullptr;

	return true;
}

bool SceneGame::OnGuiMouseClickEvent(GuiControl* control)
{
	// HEKATE MUST DELETE	

	return true;
}

void SceneGame::ActiveParticles()
{
	if (mapName == "town")
	{
		if (isNight)
		{
			if (firefliesPS == nullptr)
			{
				firefliesPS = app->particleManager->CreateParticleSystem({ 0, 0 }, Blueprint::FIREFLIES);
			}
		}
			
		if (player->currentAnim != &player->idle)
		{
			if (walkParticles == nullptr) {
				dPoint pos = { player->body->pos.x, player->body->pos.y + 10 };
				walkParticles = app->particleManager->CreateParticleSystem(pos, Blueprint::SAND);
			}
			else {
				walkParticles->TurnOff();
				walkParticles = nullptr;
			}
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
	if (showSavingState == true)
	{
		if (saveTime < 75)
		{
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
		if (app->transitionManager->step == TransitionStep::NONE)
        {
            app->transitionManager->LoadTransition();
        }
        else if (app->transitionManager->step == TransitionStep::SWITCH)
        {
			app->map->ChangeMap(mapName.GetString());
			player->body->pos = player->newPos;
			LoadNPC();
			LoadItems();
			LoadPuzzles();
			app->scaleObj->SetCurrentScale(app->collisions->scaleMap);
        }
        else if (app->transitionManager->step == TransitionStep::FINISHED)
        {
            app->transitionManager->step = TransitionStep::NONE;
			isMapChanging = false;
        }  
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
	for (pugi::xml_node itemNode = app->configNode.child("itemsMap").child(mapName.GetString()).child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, itemNode);
		item->Start();
		itemsList.Add(item);
	}	
}

void SceneGame::LoadPuzzles()
{
	for (ListItem<Puzzle*>* item = app->puzzleManager->activePuzzles.start; item != NULL; item = item->next)
	{
		item->data->UnloadAssets();
	}

	app->puzzleManager->activePuzzles.Clear();

	for (ListItem<Puzzle*>* puzzlesItem = app->puzzleManager->puzzles.start; puzzlesItem != nullptr; puzzlesItem = puzzlesItem->next)
	{
		if (puzzlesItem->data->map == mapName)
		{
			app->puzzleManager->TriggerPuzzle(puzzlesItem->data->orderID);
		}
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
		pmemberItem->data->initPos.x = data.child("partymember").attribute("fightPosX").as_int();
		pmemberItem->data->initPos.y = data.child("partymember").attribute("fightPosY").as_int();
	}

	//Load previous saved player number of lives
	saveEnabled = data.child("checkpointEnabled").attribute("checkpointEnabled").as_bool();

	//mapName = data.child("mapname").attribute("mapname").as_string();

	//// load previous saved bat position
	//iPoint kidpos = { data.child("kidposition").attribute("x").as_float(), data.child("kidposition").attribute("y").as_float() };
	//app->scene->kid->pbody->body->settransform(kidpos, 0);

	//Load NPC States
	/*for (pugi::xml_node itemNode = app->configNode.child("npcs").child(mapName.GetString()).child("npc"); itemNode; itemNode = itemNode.next_sibling("npc"))
	{
		npc = (NPC*)app->entityManager->CreateEntity(EntityType::NPC, itemNode);
		npcList.Add(npc);
		npc->Start();
	}

	ListItem<NPC*>*npcItem;
	for (npcItem = npcList.start; npcItem != NULL; npcItem = npcItem->next)
	{
		pugi::xml_node partyMember = data.append_child("npc");
		npcItem->data->name = data.child("partymember").attribute("name").as_string();
		npcItem->data->maxHp = data.child("partymember").attribute("maxHp").as_uint();
		npcItem->data->maxMana = data.child("partymember").attribute("maxMana").as_uint();
		npcItem->data->currentHp = data.child("partymember").attribute("currentHp").as_uint();
		npcItem->data->level = data.child("partymember").attribute("level").as_uint();
		npcItem->data->attack = data.child("partymember").attribute("attack").as_uint();
		npcItem->data->defense = data.child("partymember").attribute("defense").as_uint();
		npcItem->data->speed = data.child("partymember").attribute("speed").as_uint();
		npcItem->data->critRate = data.child("partymember").attribute("critRate").as_uint();
		npcItem->data->initPos.x = data.child("partymember").attribute("fightPosX").as_int();
		npcItem->data->initPos.y = data.child("partymember").attribute("fightPosY").as_int();
	}*/

	return true;
}

bool SceneGame::SaveState(pugi::xml_node& data)
{
	pugi::xml_node sceneGame = data.append_child("scene");

	pugi::xml_node mapname = sceneGame.append_child("mapName");
	mapname.append_attribute("actualMap") = mapName.GetString();

	
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