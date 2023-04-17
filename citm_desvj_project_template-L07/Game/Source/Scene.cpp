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
	origintexturePath = app->configNode.child("scene").child("originTexture").attribute("origintexturePath").as_string();
	checkPointTexPath = app->configNode.child("scene").child("checkpoint").attribute("checkpointPath").as_string();
	slimeTilePath = app->configNode.child("scene").child("pathfinding").attribute("slimePathTile").as_string();
	musicPath = app->configNode.child("scene").child("music").attribute("musicPath").as_string();
	selectSFXPath = app->configNode.child("scene").child("scenesfx").attribute("selectSFXPath").as_string();
	imgPausePath = app->configNode.child("scene").child("imgPause").attribute("imgPausePath").as_string();

	// Iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = app->configNode.child("scene").child("coin"); itemNode; itemNode = itemNode.next_sibling("coin"))
	{
		coin = (Coin*)app->entityManager->CreateEntity(EntityType::COIN);
		coin->parameters = itemNode;
		coinsList.Add(coin);
	}
	
	for (pugi::xml_node itemNode = app->configNode.child("scene").child("life"); itemNode; itemNode = itemNode.next_sibling("life"))
	{
		item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
		livesCollectedList.Add(item);
	}

	//L02: DONE 3: Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = app->configNode.child("scene").child("player");

	mapName = "mapfile";
	LoadNPC(mapName);
	
	for (pugi::xml_node itemNode = app->configNode.child("scene").child("bat"); itemNode; itemNode = itemNode.next_sibling("bat"))
	{
		bat = (BatEnemy*)app->entityManager->CreateEntity(EntityType::FLYING_ENEMY);
		bat->parameters = itemNode;
		//bat->lives = 2;
	}

	//Create party
	uchar partyCount = 0;
	for (pugi::xml_node itemNode = app->configNode.child("sceneFight").child("partymember"); itemNode; itemNode = itemNode.next_sibling("partymember"))
	{
		//type
		SString typeStr = itemNode.attribute("type").as_string();
		MemberType type = typeStr == "ally" ? MemberType::ALLY : MemberType::ENEMY;

		//texture
		const char* path = itemNode.attribute("texturepath").as_string();
		SDL_Texture* tex = app->tex->Load(path);

		//battle position
		int offsetX = 200;
		int offsetY = 300;

		iPoint position;
		position.x = offsetX;
		position.y = offsetY + 96 * partyCount;
		partyCount++;

		PartyMember* member = new PartyMember(
			type,
			MemberStatus::NORMAL,
			itemNode.attribute("name").as_string(),
			itemNode.attribute("maxHp").as_uint(),
			itemNode.attribute("maxMana").as_uint(),
			itemNode.attribute("attack").as_uint(),
			itemNode.attribute("defense").as_uint(),
			itemNode.attribute("speed").as_uint(),
			itemNode.attribute("critRate").as_uint(),
			tex,
			position);

		app->partyManager->AddMemberToParty(member);
	}

	/*INITIALIZE NECESSARY MODULES*/
	app->physics->Enable();
	app->pathfinding->Enable();
	app->entityManager->Enable();
	app->map->Enable();
	LOG("--STARTS GAME SCENE--");
	app->physics->debug = false;
	exitGame = false;

	// L03: DONE: Load map
	app->map->Load();
	
	// L12 Create walkability map
	if (app->map->Load()) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	// Play level music
	app->audio->PlayMusic(musicPath, 1.0f);

	// Loading set of SFX
	selectSFX = app->audio->LoadFx(selectSFXPath);
	
	// Texture to highligh mouse position 
	slimeTilePathTex = app->tex->Load(slimeTilePath);
	// Texture to show path origin 
	originTex = app->tex->Load(origintexturePath);

	checkPointTex = app->tex->Load(checkPointTexPath);

	img_pause = app->tex->Load(imgPausePath);
	pauseRect = {35, 69, 310, 555};
	
	// L15: TODO 2: Declare a GUI Button and create it using the GuiManager
	uint w, h;
	app->win->GetWindowSize(w, h);
	resumeButton14 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 14, "resume", 7, { 515, 295, 252, 76 }, this);
	backToTitleButton15 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 15, "back to title", 13, { 515, 375, 252, 76 }, this);
	settingsButton16 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 16, "settings", 8, { 515, 455, 252, 76 }, this);
	closeButton17 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 17, "save and exit", 12, { 515, 535, 252, 76 }, this);


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
		app->partyManager->enemyToFight = "enemykid";
		app->fade->FadeToBlack(this, (Module*)app->sceneFight, 0);
	}

	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		app->render->viewGUIbounds = !app->render->viewGUIbounds;
		app->audio->PlayFx(selectSFX);
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		if (!dialogueManager->dialogueLoaded) { gamePaused = !gamePaused; }
		pauseMenu = !pauseMenu;
		
		Mix_PauseMusic();
	}

	if (gamePaused != true)
	{
		Mix_ResumeMusic();

		// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
		if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		{
			app->SaveGameRequest();
			app->audio->PlayFx(selectSFX);
		}
		if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		{
			app->LoadGameRequest();
			app->audio->PlayFx(selectSFX);
		}

		// God Mode key
		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			player->godMode = !player->godMode;
			app->audio->PlayFx(selectSFX);
		}
	}
	
	// Camera movement related to player's movement
	FixCamera();

	
	if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN && !dialogueManager->dialogueLoaded)
	{
		gamePaused = !gamePaused;
		partyMenu = !partyMenu;
		app->audio->PlayFx(selectSFX);
	}

	// Draw map
	app->map->Draw();

	Checkpoint();
		
	SaveUI();

	//Blit UI
	app->ui->BlitCoins();
	app->ui->BlitTimer();
	app->ui->BlitLives();
	app->ui->BlitFPS();

	if (app->physics->debug) {
		app->ui->BlitPlayerXPos();
		app->ui->BlitPlayerYPos();
		app->ui->BlitSlimeLives();
		app->ui->BlitBatLives();
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

	if (gamePaused == true && (pauseMenu == true || partyMenu == true))
		app->render->DrawTexture(img_pause, app->render->camera.x + app->render->camera.w / 2 - pauseRect.w / 2, app->render->camera.y + app->render->camera.h / 2 - pauseRect.h / 2, &pauseRect);

	if ((gamePaused && dialogueManager->dialogueLoaded) && (pauseMenu == false && partyMenu == false)) {
		dialogueManager->Update();
	}

	// Draw GUI
	app->guiManager->Draw();

	resumeButton14->state = GuiControlState::DISABLED;
	backToTitleButton15->state = GuiControlState::DISABLED;
	settingsButton16->state = GuiControlState::DISABLED;
	closeButton17->state = GuiControlState::DISABLED;

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

	}

	if (partyMenu == true)
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
	//app->map->Disable();
	
	//app->guiManager->guiControlsList.Clear();
	gamePaused = false;
	pauseMenu = false;
	partyMenu = false;

	Mix_ResumeMusic();

	app->tex->UnLoad(img_pause);

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

	return true;
}


bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: TODO 5: Implement the OnGuiMouseClickEvent method
	switch (control->id)
	{
	case 14:
		gamePaused = !gamePaused;
		pauseMenu = !pauseMenu;
		app->audio->PlayFx(app->titlescreen->menuSelectionSFX);
		break;

	case 15:
		app->fade->FadeToBlack(this, (Module*)app->titlescreen, 0);
		app->audio->PlayFx(app->titlescreen->startSFX);
		break;

	case 16:
		//Here goes settings menu
		break;
	
	case 17:
		app->SaveGameRequest();
		exitGame = !exitGame;
		app->audio->PlayFx(app->titlescreen->menuSelectionSFX);
		break;

	default:
		break;
	}

	return true;
}

void Scene::SaveUI() {
	if (app->saveGameRequested == true) {
		showSavingState = true;
	}
	if (showSavingState == true) {
		if (saveTime < 50) {
			uint w, h;
			SDL_Rect rect_2 = { 64, 0, 32, 32 };
			app->win->GetWindowSize(w, h);
			app->render->DrawTexture(checkPointTex, w - 100, h - 100, &rect_2, SDL_FLIP_HORIZONTAL, 0);
			saveTime++;
			LOG("SAVETIME: %d", saveTime);
		}
		else {
			showSavingState = false;
			saveTime = 0;
		}

	}
}

void Scene::Checkpoint() {
	
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

void Scene::ResetScene() {
	
	app->audio->PlayMusic("Assets/Audio/Music/song1.ogg", 1.0f);

	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	if (checkpointEnabled == false || result == NULL) {
		checkpointEnabled = false;
		player->ResetPlayerPos();
		player->lives = 3;
	}
	else if (checkpointEnabled == true && result != NULL) {
		app->LoadGameRequest();
	}
}

void Scene::SceneMap() {

	if (isMapChanging == true)
	{
		
		app->map->ChangeMap(mapName.GetString());
		player->pbody->body->SetTransform(PIXEL_TO_METERS(player->newPos), 0.0f);
		//LoadNPC(mapName.GetString());

		isMapChanging = false;
	}

}

void Scene::LoadNPC(SString mapName_)
{
	
	if (mapName_ == "house")
	{
		
	}
	else if (mapName_ == "mapfile")
	{
		for (pugi::xml_node itemNode = app->configNode.child("scene").child("npc"); itemNode; itemNode = itemNode.next_sibling("npc"))
		{
			npc = (NPC*)app->entityManager->CreateEntity(EntityType::NPC);
			npc->parameters = itemNode;
			npcList.Add(npc);
		}
	}
	
}

void Scene::FixCamera() {

	app->render->camera.x = (player->position.x) - (app->win->screenSurface->w) / 2;
	app->render->camera.y = (player->position.y) - (app->win->screenSurface->h) / 2;
	if (app->render->camera.x < 0)
		app->render->camera.x = 0;
	if (app->render->camera.y < 0)
		app->render->camera.y = 0;
	if (app->render->camera.x + app->render->camera.w > METERS_TO_PIXELS(app->map->mapData.width)) {
		if (cameraFixX == false)
		{
			cameraFixX = true;
			cameraPos.x = app->render->camera.x;
		}
		app->render->camera.x = cameraPos.x;
	}
	else
	{
		if (cameraFixX == true)
		{
			cameraFixX = false;
		}
	}
	if (app->render->camera.y + app->render->camera.h > METERS_TO_PIXELS(app->map->mapData.height)) {
		if (cameraFixY == false)
		{
			cameraFixY = true;
			LOG("CAMERA Y FIX: %d", cameraFixY);
			cameraPos.y = app->render->camera.y;
		}
		app->render->camera.y = cameraPos.y;
	}
	else
	{
		if (cameraFixY == true)
		{
			cameraFixY = false;
		}
	}
}

bool Scene::LoadState(pugi::xml_node& data)
{
	// Load previous saved player position
	b2Vec2 playerPos = { data.child("playerPosition").attribute("x").as_float(), data.child("playerPosition").attribute("y").as_float() };
	app->scene->player->pbody->body->SetTransform(playerPos, 0);
	
	// Load previous saved cameraFix & cameraFix2 parameters
	app->scene->cameraFixX = data.child("cameraIsFix").attribute("value").as_bool();
	app->scene->cameraFixY = data.child("cameraIsFix2").attribute("value").as_bool();

	//Load previous saved player number of lives
	app->scene->player->lives = data.child("playerLives").attribute("playerLives").as_float();

	app->scene->player->coins = data.child("coins").attribute("coins").as_int();
	ListItem<Coin*>* coinsCollected;
	coinsCollected = coinsList.start;
	int countCoins = app->scene->player->coins;
	while(countCoins >= 0 && coinsCollected != NULL){
		
		if (coinsCollected->data->isPicked == false) {
			coinsCollected->data->isPicked = true;
			countCoins--;
		}
			

		coinsCollected = coinsCollected->next;
	}
	
	itemLivesCount = data.child("itemLives").attribute("itemLives").as_int();
	ListItem<Item*>* livesCollected;
	livesCollected = livesCollectedList.start;
	int countLives = itemLivesCount;
	while(countLives >= 0 && livesCollected != NULL){
		
		if (livesCollected->data->isPicked == false) {
			livesCollected->data->isPicked = true;
			countLives--;
		}
			

		livesCollected = livesCollected->next;
	}

	//Load previous saved player number of lives
	checkpointEnabled = data.child("checkpointEnabled").attribute("checkpointEnabled").as_bool();

	// Load previous saved bat position
	b2Vec2 batPos = { data.child("batPosition").attribute("x").as_float(), data.child("batPosition").attribute("y").as_float() };
	app->scene->bat->pbody->body->SetTransform(batPos, 0);

	//Load previous saved bat number of lives
	app->scene->bat->lives = data.child("batLives").attribute("batLives").as_int();

	return true;
}

bool Scene::SaveState(pugi::xml_node& data)
{
	// Save current player position
	pugi::xml_node playerPos = data.append_child("playerPosition");
	playerPos.append_attribute("x") = app->scene->player->pbody->body->GetTransform().p.x;
	playerPos.append_attribute("y") = app->scene->player->pbody->body->GetTransform().p.y;

	// Save cameraFix parameter
	pugi::xml_node cameraIsFix = data.append_child("cameraIsFix");
	cameraIsFix.append_attribute("value") = app->scene->cameraFixX;

	// Save cameraFix2 parameter
	pugi::xml_node cameraIsFix2 = data.append_child("cameraIsFix2");
	cameraIsFix2.append_attribute("value") = app->scene->cameraFixY;

	// Save current player number of lives
	pugi::xml_node playerLives = data.append_child("playerLives");
	playerLives.append_attribute("playerLives") = app->scene->player->lives;
	
	// Save current player number of coins
	pugi::xml_node playerCoins = data.append_child("coins");
	playerCoins.append_attribute("coins") = app->scene->player->coins;
	
	// Save current player number of coins
	pugi::xml_node itemLives = data.append_child("itemLives");
	itemLives.append_attribute("itemLives") = itemLives;
	
	// Save current player number of coins
	pugi::xml_node checkpointEnabled = data.append_child("checkpointEnabled");
	checkpointEnabled.append_attribute("checkpointEnabled") = checkpointEnabled;

	// Save current bat position
	pugi::xml_node batPos = data.append_child("batPosition");
	batPos.append_attribute("x") = app->scene->bat->pbody->body->GetTransform().p.x;
	batPos.append_attribute("y") = app->scene->bat->pbody->body->GetTransform().p.y;

	// Save current bat number of lives
	pugi::xml_node batLives = data.append_child("batLives");
	batLives.append_attribute("batLives") = app->scene->bat->lives;
	
	pugi::xml_node checkPoint = data.append_child("checkPoint");
	checkPoint.append_attribute("checkPoint") = app->scene->checkpointEnabled;

	return true;
}