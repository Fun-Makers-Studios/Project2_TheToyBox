#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "NPC.h"
#include "Item.h"
#include "Chest.h"
#include "Fonts.h"
#include "QuestManager.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	LOG("Starting Scene");
	bool ret = true;

	pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child(name.GetString());

	// iterate all NPC in the scene
	for (pugi::xml_node npcNode = config.child("npc"); npcNode; npcNode = npcNode.next_sibling("npc"))
	{
		NPC* npc = (NPC*)app->entityManager->CreateEntity(EntityType::NPC);
		npc->parameters = npcNode;
		npcs.Add(npc);
	}

	// iterate all objects in the scene
	for (pugi::xml_node ringNode = config.child("ring"); ringNode; ringNode = ringNode.next_sibling("ring"))
	{
		//Item* item = Chest(ringNode.attribute("x").as_int(), ringNode.attribute("y").as_int(), ringNode.attribute("texturepath").as_string());
		//items.Add(item);
	}

	// Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");

	//IMPORTANT, ENTITY MANAGER IS DISABLED BY DEFAULT
	if (app->entityManager->state == false) { app->entityManager->Enable(); }

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	// Load music
	musicPath = config.child("music").attribute("path").as_string();
	app->audio->PlayMusic(musicPath);

	// Load map
	if (app->map->state == false) { app->map->Enable(); }
	bool retLoad = app->map->Load(name.GetString());

	font_text = app->fonts->Load(config.child("texturepaths").attribute("font").as_string(), "ABCDEFGHIJKLMNOPQRSTUWYZ0123456789-= ", 1);

	debug = false;
	questMenu = false;
	points = 0;

	return ret;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	// Draw map
	app->map->Draw();

	if (debug) {
		//Draw NPC boundaries
		ListItem<NPC*>* npcitem = npcs.start;

		while (npcitem != NULL) {
			app->render->DrawRectangle(npcitem->data->boundaries, 0, 0, 255);

			npcitem = npcitem->next;
		}

		// Draw player boundaries
		app->render->DrawRectangle({ player->tile.x * 32, player->tile.y * 32, 32, 32}, 0, 255, 0);
	}

	if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN) {
		if (questMenu) { questMenu = false; }
		else if (!questMenu) { questMenu = true; }
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (questMenu) {
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

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		if (debug) { debug = false; }
		else if (!debug) { debug = true; }
	}

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	app->audio->PlayMusic("");

	if (app->entityManager->state) { app->entityManager->Disable(); }

	return true;
}
