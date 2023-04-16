#include "SceneFight.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Log.h"
#include "PartyManager.h"
#include "List.h"


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
	imgPath = app->configNode.child("sceneFight").child("backgroundimage").attribute("texturepath").as_string();
	//musicPath = app->configNode.child("logo").child("music").attribute("musicPath").as_string();

	/*Load*/
	img = app->tex->Load(imgPath);
	//app->audio->PlayMusic(musicPath);


	// ======== CREATE TURN LIST ========

	//Add party memebers
	ListItem<PartyMember*>* member = app->partyManager->party.start;
	while (member != NULL)
	{
		turnList.Add(member->data);
		member = member->next;
	}
	
	//Add enemies
	const char* enemy = app->partyManager->enemyToFight;
	uchar amount;
	if (enemy == "enemykid") { amount = 3; }
	else if (enemy == "enemyshopkeeper") { amount = 1; }
	else if (enemy == "enemyclown") { amount = 2; }

	for (size_t i = 0; i < amount; i++)
	{
		pugi::xml_node itemNode = app->configNode.child("sceneFight").child(app->partyManager->enemyToFight);

		//type
		SString typeStr = itemNode.attribute("type").as_string();
		MemberType type = typeStr == "ally" ? MemberType::ALLY : MemberType::ENEMY;

		//texture
		const char* path = itemNode.attribute("texturepath").as_string();
		SDL_Texture* tex = app->tex->Load(path);

		//battle position
		int offsetX = 800;
		int offsetY = 300;

		iPoint position;
		position.x = offsetX;
		position.y = offsetY + 96 * i;

		PartyMember* member = new PartyMember(
			type,
			itemNode.attribute("name").as_string(),
			itemNode.attribute("maxHp").as_uint(),
			itemNode.attribute("maxMana").as_uint(),
			itemNode.attribute("attack").as_uint(),
			itemNode.attribute("defense").as_uint(),
			itemNode.attribute("speed").as_uint(),
			itemNode.attribute("critRate").as_uint(),
			tex,
			position);

		turnList.Add(member);
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

	for (size_t i = 0; i < turnList.Count(); i++)
	{
		app->render->DrawTexture(turnList.At(i)->data->texture, turnList.At(i)->data->fightPosition.x, turnList.At(i)->data->fightPosition.y, NULL);
	}

	

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