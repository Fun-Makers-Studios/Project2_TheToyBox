#include "SceneFight.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Log.h"
#include "PartyManager.h"


SceneFight::SceneFight() : Module()
{
	name.Create("sceneFight");
}

// Destructor
SceneFight::~SceneFight()
{}

// Called before render is available
bool SceneFight::Awake(pugi::xml_node& config)
{
	LOG("Loading LogoScreen");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneFight::Start()
{
	LOG("--FORMING PARTY--");

	/*Initialize*/
	imgPath = app->configNode.child("sceneFIght").child("backgroundimage").attribute("texturepath").as_string();
	//musicPath = app->configNode.child("logo").child("music").attribute("musicPath").as_string();

	/*Load*/
	img = app->tex->Load(imgPath);
	//app->audio->PlayMusic(musicPath);


	//Create party
	for (pugi::xml_node itemNode = app->configNode.child("sceneFight").child("partymember"); itemNode; itemNode = itemNode.next_sibling("partymember"))
	{
		const char* path = itemNode.attribute("texturepath").as_string();
		SDL_Texture* tex = app->tex->Load(path);

		PartyMember* member = new PartyMember(
			itemNode.attribute("name").as_string(),
			itemNode.attribute("maxHp").as_uint(),
			itemNode.attribute("maxMana").as_uint(),
			itemNode.attribute("attack").as_uint(),
			itemNode.attribute("defense").as_uint(),
			itemNode.attribute("speed").as_uint(),
			itemNode.attribute("critRate").as_uint(),
			tex);

		
		partyManager->AddMemberToParty(member);
	}


	return true;
}

// Called each loop iteration
bool SceneFight::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneFight::Update(float dt)
{
	

	app->render->DrawTexture(img, 0, 0, NULL);

	return true;
}

// Called each loop iteration
bool SceneFight::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool SceneFight::CleanUp()
{
	LOG("Freeing LOGO SCENE");

	if (img != nullptr) {
		app->tex->UnLoad(img);
	}

	return true;
}