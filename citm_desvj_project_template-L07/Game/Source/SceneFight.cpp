#include "SceneFight.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Log.h"
#include "PartyManager.h"
#include "TitleScreen.h"
#include "UI.h"
#include "GuiManager.h"
#include "List.h"
#include <time.h>


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
		enemyList.Add(member);
	}

	enemySelected = 0;

	attackButton18 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 18, "attack", 7, { 100, 600, 252, 76 }, this);
	defenseButton19 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 19, "defense", 8, { 510, 600, 252, 76 }, this);
	turnJumpButton20 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 20, "jump turn", 10, { 915, 600, 252, 76 }, this);

	srand(time(NULL));

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
	//app->render->DrawTexture(img, 0, 0, NULL);

	//TODO: Sort by speed

	//Turn start
	uint id = turn % turnList.Count();
	turnMember = turnList.At(id)->data;

	//	check player turn
	if (turnMember->type == ALLY)
	{
		//	choose option/enemy
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			enemySelected--;
			if (enemySelected < 0)
				enemySelected = enemyList.Count();
		}
		else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		{
			enemySelected++;
			if (enemySelected > enemyList.Count())
				enemySelected = 0;
		}

	}
	else if (turnMember->type == ENEMY)
	{
		int randomMember = rand() % 3;

		Attack(turnMember, app->partyManager->party.At(randomMember)->data);

	}

	
	//	draw stats
	

	// Draw GUI
	app->guiManager->Draw();

	attackButton18->state = GuiControlState::DISABLED;
	defenseButton19->state = GuiControlState::DISABLED;
	turnJumpButton20->state = GuiControlState::DISABLED;

	if (attackButton18->state == GuiControlState::DISABLED) {
		attackButton18->state = GuiControlState::NORMAL;
	}
	if (defenseButton19->state == GuiControlState::DISABLED) {
		defenseButton19->state = GuiControlState::NORMAL;
	}
	if (turnJumpButton20->state == GuiControlState::DISABLED) {
		turnJumpButton20->state = GuiControlState::NORMAL;
	}
	

	for (size_t i = 0; i < turnList.Count(); i++)
	{
		if (turnList.At(i)->data->currentHp <= 0)
			turnList.Del(turnList.At(i));
		else
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

	if (attackButton18 != nullptr)
		attackButton18->state = GuiControlState::DISABLED;
	if (defenseButton19 != nullptr)
		defenseButton19->state = GuiControlState::DISABLED;
	if (turnJumpButton20 != nullptr)
		turnJumpButton20->state = GuiControlState::DISABLED;

	if (img != nullptr) {
		app->tex->UnLoad(img);
	}

	return true;
}

bool SceneFight::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: TODO 5: Implement the OnGuiMouseClickEvent method
	switch (control->id)
	{
	case 18: //Attack
		Attack(turnMember, enemyList.At(enemySelected)->data);
		app->audio->PlayFx(app->titlescreen->menuSelectionSFX);	
		break;

	case 19: //Defense
		
		app->audio->PlayFx(app->titlescreen->startSFX);
		break;

	case 20: //Skip turn
		SkipTurn();
		app->audio->PlayFx(app->titlescreen->startSFX);
		break;

	default:
		break;
	}

	return true;
}


void SceneFight::Attack(PartyMember* turnMember_, PartyMember* receiverMember_)
{
	//TODO: Calculate crit, def
	LOG("%c HP BEFORE: %u", receiverMember_->name,receiverMember_->currentHp);
	if (turnMember->attack > receiverMember_->currentHp)
	{
		receiverMember_->currentHp = 0;
	}
	else {
		receiverMember_->currentHp -= turnMember->attack;
	}
	LOG("%c HP AFTER: %d", receiverMember_->name,receiverMember_->currentHp);
	turn++;
}

void SceneFight::SkipTurn() {
	turn++;
}