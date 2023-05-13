#include "App.h"
#include "Input.h"
#include "Window.h"
#include "Textures.h"
#include "Fonts.h"
#include "Audio.h"
#include "ModuleController.h"

#include "SceneManager.h"

#include "Map.h"
#include "Collisions.h"
#include "Pathfinding.h"

#include "EntityManager.h"
#include "PartyManager.h"
#include "ParticleSystemManager.h"
#include "QuestManager.h"

#include "MenuManager.h"
#include "UI.h"
#include "ParticleSystemManager.h"
#include "GuiManager.h"
#include "GuiButton.h"

#include "Debug.h"
#include "PartyManager.h"


#include "Render.h"

#include "Defs.h"
#include "Log.h"
#include "Optick/include/optick.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	input = new Input();
	win = new Window();
	tex = new Textures();
	fonts = new Fonts();
	audio = new Audio();
	controller = new ModuleController();

	sceneManager = new SceneManager();

	map = new Map();
	pathfinding = new Pathfinding();
	collisions = new Collisions();

	entityManager = new EntityManager();
	partyManager = new PartyManager();
	questManager = new QuestManager();
	particleManager = new ParticleSystemManager();

	menuManager = new MenuManager();
	ui = new UI();
	guiManager = new GuiManager();
	debug = new Debug();

	render = new Render();


	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input, true);
	AddModule(win, true);
	AddModule(tex, true);
	AddModule(fonts, true);
	AddModule(audio, true);
	AddModule(controller, true);

	AddModule(sceneManager, true);

	AddModule(map, false);
	AddModule(pathfinding, false);
	AddModule(collisions, false);

	AddModule(entityManager, false);
	AddModule(partyManager, true);
	AddModule(particleManager, true);
	AddModule(questManager, false);

	AddModule(menuManager, false);
	AddModule(ui, true);
	AddModule(guiManager, true);
	AddModule(debug, true);

	// Render last to swap buffer
	AddModule(render, true);


	//
	scaleObj = new Scale();
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module, bool activate)
{
	if(activate == true)
		module->Init();

	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	timer = Timer();

	bool ret = false;

	//Load config from XML
	ret = LoadConfig();

	if (ret == true)
	{
		title = configNode.child("app").child("title").child_value();
		
		maxFrameDuration = configNode.child("app").child("frcap").attribute("value").as_int();
	
		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			pugi::xml_node node = configNode.child(item->data->name.GetString());
			ret = item->data->Awake(node);
			item = item->next;
		}
	}

	LOG("---------------- Time Awake: %f/n", timer.ReadMSec());

	return ret;
}

// Called before the first frame
bool App::Start()
{
	timer.Start();
	startupTime.Start();
	lastSecFrameTime.Start();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->Start();
	}

	LOG("----------------- Time Start(): %f", timer.ReadMSec());

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
bool App::LoadConfig()
{
	bool ret = false;

	//Load config.xml file using load_file() method from the xml_document class
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");

	if (parseResult) {
		ret = true;
		configNode = configFile.child("config");
	}
	else {
		LOG("Error in App::LoadConfig(): %s", parseResult.description());
	}

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	OPTICK_CATEGORY("FinishUpdate", Optick::Category::GameLogic);
	
	if (loadGameRequested == true) LoadFromFile();
	if (saveGameRequested == true) SaveToFile();

	// Amount of frames since startup
	frameCount++;
	// Amount of time since game start (use a low resolution timer)
	secondsSinceStartup = startupTime.ReadSec();
	// Amount of ms took the last update
	dt = frameTime.ReadMSec();
	// Amount of frames during the last second
	lastSecFrameCount++;

	if (lastSecFrameTime.ReadMSec() > 1000) {
		lastSecFrameTime.Start();
		framesPerSecond = lastSecFrameCount;
		lastSecFrameCount = 0;
		// Average FPS for the whole game life
		averageFps = (averageFps + framesPerSecond) / 2;
	}

	float delay = float(maxFrameDuration) - dt;

	PerfTimer delayTimer = PerfTimer();
	delayTimer.Start();
	if (maxFrameDuration > 0 && delay > 0 && render->limitFPS == false) {
		SDL_Delay(delay);
		//LOG("We waited for %f milliseconds and the real delay is % f", delay, delayTimer.ReadMs());
		dt = maxFrameDuration;
	}
	else {
		//LOG("No wait");
	}

	// Shows the time measurements in the window title
	static char title[256];
	sprintf_s(title, 256, "FPS: %i | Avg.FPS: %.2f | Last dt: %.3f | V-Sync: %s",
		framesPerSecond, averageFps, dt, app->render->limitFPS ? "ON" : "OFF");
	app->win->SetTitle(title);

}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	OPTICK_CATEGORY("PreUpdate", Optick::Category::GameLogic);
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	OPTICK_CATEGORY("DoUpdate", Optick::Category::GameLogic);
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	OPTICK_CATEGORY("PostUpdate", Optick::Category::GameLogic);
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true)
	{
		if(item->data->active == true)
			ret = item->data->CleanUp();

		item = item->prev;
	}


	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// ---------------------------------------
uint App::GetFPS() 
{
	return framesPerSecond;
}

// ---------------------------------------
float App::GetAverageFPS()
{
	return averageFps;
}

// ---------------------------------------
float App::GetDT()
{
	return dt;
}

// ---------------------------------------
float App::GetTimesSinceStart()
{
	return secondsSinceStartup;
}

// ---------------------------------------
uint App::GetFrameCount()
{
	return frameCount;
}

//----------------------------------------
void App::LoadGameRequest()
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() 
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveGameRequested = true;
}


bool App::LoadFromFile()
{
	bool ret = true;

	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	if (result == NULL)
	{
		LOG("Could not load xml file savegame.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->LoadState(gameStateFile.child("save_state").child(item->data->name.GetString()));
			item = item->next;
		}
	}

	loadGameRequested = false;

	return ret;
}

// L02: DONE 7: Implement the xml save method SaveToFile() for current state
// check https://pugixml.org/docs/quickstart.html#modify
bool App::SaveToFile() 
{
	bool ret = false;

	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node saveStateNode = saveDoc->append_child("save_state");

	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL)
	{
		ret = item->data->SaveState(saveStateNode.append_child(item->data->name.GetString()));
		item = item->next;
	}

	ret = saveDoc->save_file("save_game.xml");

	saveGameRequested = false;

	return ret;
}