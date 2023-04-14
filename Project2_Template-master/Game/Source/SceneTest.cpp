#include "App.h"
#include "Input.h"
#include "Fonts.h"
#include "Textures.h"
#include "Audio.h"
#include "Window.h"
#include "Scene.h"
#include "SceneTest.h"
#include "EntityManager.h"
#include "QuestManager.h"
#include "Map.h"
#include "PathFinding.h"
#include "Render.h"


#include "Defs.h"
#include "Log.h"

SceneTest::SceneTest() : Scene()
{
	id.Create("sceneTest");
}

// Destructor
SceneTest::~SceneTest()
{}

// Called before render is available
void SceneTest::OnCreate(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	// Instantiate the player using the entity manager
	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}

	List<NPC*>* npcList = new List<NPC*>;
	// Iterate all NPC in the scene
	for (pugi::xml_node npcNode = config.child("npc"); npcNode; npcNode = npcNode.next_sibling("npc"))
	{
		NPC* npc = (NPC*)app->entityManager->CreateEntity(EntityType::NPC);
		npc->parameters = npcNode;
		npcList->Add(npc);
	}
	SetNPCList(npcList);
	npcList->Clear();
}

void SceneTest::OnDestroy() {}

// Called before the first frame
void SceneTest::OnActivate()
{
	LOG("Starting Scene");
	bool ret = true;

	
	pugi::xml_node configNode = app->GetNode();
	pugi::xml_node config = configNode.child(id.GetString());

	// iterate all objects in the scene
	for (pugi::xml_node ringNode = config.child("ring"); ringNode; ringNode = ringNode.next_sibling("ring"))
	{
		//Item* item = Chest(ringNode.attribute("x").as_int(), ringNode.attribute("y").as_int(), ringNode.attribute("texturepath").as_string());
		//items.Add(item);
	}

	//IMPORTANT, ENTITY MANAGER IS DISABLED BY DEFAULT (NOT)
	//if (app->entityManager->active == false) { app->entityManager->Enable(); }

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	// Load music
	musicPath = config.child("music").attribute("path").as_string();
	app->audio->PlayMusic(musicPath);

	// Load map
	if (app->map->active == false) { app->map->Enable(); }
	bool retLoad = app->map->Load(id.GetString());

	font_text = app->fonts->Load(config.child("texturepaths").attribute("font").as_string(), "ABCDEFGHIJKLMNOPQRSTUWYZ0123456789-= ", 1);

	debug = false;
	questMenu = false;
	points = 0;
}

void SceneTest::OnDeactivate() {}

// Called each loop iteration
void SceneTest::PreUpdate()
{
}

// Called each loop iteration
void SceneTest::Update(float dt)
{
	// Draw map
	app->map->Draw();

	if (debug) {
		//Draw NPC boundaries
		ListItem<NPC*>* npcitem = npcs->start;

		while (npcitem != NULL) {
			app->render->DrawRectangle(npcitem->data->boundaries, 0, 0, 255);

			npcitem = npcitem->next;
		}

		// Draw player boundaries
		app->render->DrawRectangle({ player->position.x * 32, player->position.y * 32, 32, 32 }, 0, 255, 0);
	}

	if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN) {
		if (questMenu) { questMenu = false; }
		else if (!questMenu) { questMenu = true; }
	}
}

// Called each loop iteration
void SceneTest::PostUpdate()
{
	if (questMenu)
	{
		app->render->DrawRectangle({ 128, 64, 960, 576 }, 0, 0, 0);

		sprintf_s(pointsText, 6, "%d", points);
		app->fonts->BlitText(544, 576, font_text, pointsText, false);

		app->fonts->BlitText(672, 576, font_text, "POINTS", false);

		app->fonts->BlitText(960, 96, font_text, "- IN PROGRESS -", false);
		ListItem<Quest*>* qitem = app->questManager->activeQuests.start;
		int posY = 128;
		while (qitem != nullptr)
		{
			if (posY >= 300) { break; }
			Quest* item = qitem->data;
			app->fonts->BlitText(960, posY, font_text, (const char*)item->name.GetString(), false);

			qitem = qitem->next;
			posY += 32;
		}

		app->fonts->BlitText(960, 320, font_text, "- COMPLETED -", false);
		qitem = app->questManager->completedQuests.start;
		posY = 352;
		while (qitem != nullptr)
		{
			if (posY >= 700) { break; }
			Quest* item = qitem->data;
			app->fonts->BlitText(960, posY, font_text, (const char*)item->name.GetString(), false);

			qitem = qitem->next;
			posY += 32;
		}
	}

	/*if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;*/

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		if (debug) { debug = false; }
		else if (!debug) { debug = true; }
	}
}

// Called before quitting
void SceneTest::CleanUp()
{
	LOG("Freeing scene");

	app->audio->PlayMusic("");

	if (app->entityManager->active) { app->entityManager->Disable(); }

}