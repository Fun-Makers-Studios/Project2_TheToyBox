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


	if (newGame) {
		if (app->sceneManager->sceneGame->dialogueManager->GetCurrentDialogue() == nullptr)
			app->sceneManager->sceneGame->dialogueManager->Load(0);
	}

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
		isCheckpointEnabled = true;
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

	FastDebugForSave();
	SaveUI();

	//Saves game if choosing the option when talkin with MAGE
	/*if (dialogueManager->GetCurrentDialogue() != nullptr 
		&& dialogueManager->GetCurrentDialogue()->id == 6 
		&& dialogueManager->GetCurrentDialogue()->currentNode->id == 1 
		&& app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		Item* addItem = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, app->configNode.child("itemsMap").child(mapName.GetString()).child("item"));
		addItem->takeItem = true;
		showSavingState = true;
		isNight = !isNight;
		app->SaveGameRequest();
	}*/
	


	// Lower music volume - HEKATE
	/*if (app->menuManager->currentMenu == app->menuManager->menuPause) {

		if (app->musicValue >= 20)
			Mix_VolumeMusic(20);
	}
	else
	{
		Mix_VolumeMusic(app->musicValue);
	}*/

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
			
		if (!player->isIdle)
		{
			if (walkParticles == nullptr)
			{
				dPoint pos;

				if (player->currentAnim == &player->walkUp)
				{
					pos = { player->body->pos.x, player->body->pos.y + 10 };
				}
				else if (player->currentAnim == &player->walkDown)
				{
					pos = { player->body->pos.x, player->body->pos.y - 10 };

				}
				else if (player->currentAnim == &player->walkLeft)
				{
					pos = { player->body->pos.x + 10, player->body->pos.y + 4 };

				}
				else if (player->currentAnim == &player->walkRight)
				{
					pos = { player->body->pos.x - 10, player->body->pos.y + 4 };

				}

				walkParticles = app->particleManager->CreateParticleSystem(pos, Blueprint::SAND);
			}
			else
			{
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
		app->saveGameRequested = true;
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

	for (ListItem<Puzzle*>* puzzlesItem = app->puzzleManager->puzzles.start; puzzlesItem != NULL; puzzlesItem = puzzlesItem->next)
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
	pugi::xml_node sceneGame = data.child("scene");
	
	mapName = sceneGame.child("mapName").attribute("actualMap").as_string();

	isNight = sceneGame.child("isNight").attribute("night").as_bool();

	partyMemberSelected = sceneGame.child("pMmeberSelected").attribute("selected").as_uint();

	player->body->pos.x = sceneGame.child("player").attribute("x").as_double();
	player->body->pos.y = sceneGame.child("player").attribute("y").as_double();

	isMapChanging = true;

	return true;
}

bool SceneGame::SaveState(pugi::xml_node& data)
{
	pugi::xml_node sceneGame = data.append_child("scene");

	pugi::xml_node mapname = sceneGame.append_child("mapName");
	mapname.append_attribute("actualMap") = mapName.GetString();

	pugi::xml_node isnight = sceneGame.append_child("isNight");
	isnight.append_attribute("night") = isNight;
	
	pugi::xml_node pMemberSelected = sceneGame.append_child("pMmeberSelected");
	pMemberSelected.append_attribute("selected") = partyMemberSelected;
	
	pugi::xml_node playerNode = sceneGame.append_child("player");
	playerNode.append_attribute("x") = player->body->pos.x;
	playerNode.append_attribute("y") = player->body->pos.y;
	
	pugi::xml_node itemsListNode = sceneGame.append_child("itemsList");

	for (ListItem<Item*>* itemsItem = itemsList.start; itemsItem != NULL; itemsItem = itemsItem->next)
	{
		pugi::xml_node itemsNode = itemsListNode.append_child("item");
		
		itemsNode.append_attribute("x") = itemsItem->data->body->pos.x;
		itemsNode.append_attribute("y") = itemsItem->data->body->pos.y;
		itemsNode.append_attribute("name") = itemsItem->data->itemData.name.GetString();
		itemsNode.append_attribute("itemType") = itemsItem->data->itemData.itemType.GetString();
		itemsNode.append_attribute("animable") = itemsItem->data->itemData.animable;
		itemsNode.append_attribute("frames") = itemsItem->data->itemData.frames;
		itemsNode.append_attribute("isStackable") = itemsItem->data->itemData.isStackable;
		itemsNode.append_attribute("texturepath") = itemsItem->data->itemData.texturePath;
	}

	return true;
}

void SceneGame::FastDebugForSave()
{

	if (app->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN) {
		LOG("SAVED");
		showSavingState = true;
		app->SaveGameRequest();
	}

	if (app->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN) {
		LOG("NIGHT CHANGE");
		isNight = !isNight;
	}

}