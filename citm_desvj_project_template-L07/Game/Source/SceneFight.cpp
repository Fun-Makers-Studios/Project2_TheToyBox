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
#include "Debug.h"
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
	path_bg = app->configNode.child("sceneFight").child("backgroundimage").attribute("texturepath").as_string();
	path_arrow = app->configNode.child("sceneFight").child("arrowimage").attribute("texturepath").as_string();
	//musicPath = app->configNode.child("logo").child("music").attribute("musicPath").as_string();

	/*Load*/
	tex_bg = app->tex->Load(path_bg);
	tex_arrow = app->tex->Load(path_arrow);
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

		turnList.Add(member);
		enemyList.Add(member);
	}

	enemySelected = 0;

	attackButton18 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 18, "attack", 7, { 100, 600, 252, 76 }, this);
	defenseButton19 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 19, "defend", 8, { 510, 600, 252, 76 }, this);
	turnJumpButton20 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 20, "skip turn", 10, { 915, 600, 252, 76 }, this);

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
	if (turnMember->type == MemberType::ALLY)
	{
		//	choose enemy
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{			
			if (enemySelected == 0)
				enemySelected = enemyList.Count() - 1;
			else
				enemySelected--;
		}
		else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		{
			enemySelected++;
			enemySelected = enemySelected >= enemyList.Count() ? 0 : enemySelected;

			while (enemyList.At(enemySelected)->data->status == MemberStatus::DEAD)
			{
				if (enemySelected >= enemyList.Count())
					enemySelected = 0;
			}
		}
	}
	else if (turnMember->type == MemberType::ENEMY)
	{
		int randomMember = rand() % 3;

		Attack(turnMember, app->partyManager->party.At(randomMember)->data);

	}

	
	//	draw selected enemy
	app->render->DrawTexture(tex_arrow, enemyList.At(enemySelected)->data->fightPosition.x, enemyList.At(enemySelected)->data->fightPosition.y - 32, NULL);

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

	/*if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;*/

	return ret;
}

// Called before quitting
bool SceneFight::CleanUp()
{
	LOG("Freeing LOGO SCENE");

	if (attackButton18 != nullptr) attackButton18->state = GuiControlState::DISABLED;
	if (defenseButton19 != nullptr) defenseButton19->state = GuiControlState::DISABLED;
	if (turnJumpButton20 != nullptr) turnJumpButton20->state = GuiControlState::DISABLED;

	if (tex_bg != nullptr) { app->tex->UnLoad(tex_bg); }
	if (tex_arrow != nullptr) { app->tex->UnLoad(tex_arrow); }

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

	turn++;

	//godmode no damage
	if (turnMember_->type == MemberType::ENEMY && app->debug->godMode) { return; }

	//check death || godmode instakill
	if ((turnMember->attack > receiverMember_->currentHp) || 
		(turnMember_->type == MemberType::ALLY && app->debug->godMode))
	{
		receiverMember_->currentHp = 0;
		receiverMember_->status = MemberStatus::DEAD;
	}
	else
	{
		receiverMember_->currentHp -= turnMember->attack;
	}
}

void SceneFight::SkipTurn()
{
	turn++;
}