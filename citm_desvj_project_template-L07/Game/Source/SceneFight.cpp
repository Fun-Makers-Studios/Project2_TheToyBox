#include "SceneFight.h"
#include "Scene.h"
#include "App.h"
#include "Input.h"
#include "Window.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Log.h"
#include "PartyManager.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "UI.h"
#include "GuiManager.h"
#include "Debug.h"
#include "List.h"
#include <time.h>


SceneFight::SceneFight() : Scene()
{
	id = SceneID::SCENE_FIGHT;

	/*Initialize from xml*/
	musicPath = app->configNode.child("sceneFight").child("music").attribute("musicPath").as_string();

	path_fightTownBG = app->configNode.child("sceneFight").child("fightTownBG").attribute("texturepath").as_string();
	path_fightCircusBG = app->configNode.child("sceneFight").child("fightCircusBG").attribute("texturepath").as_string();

	path_arrow = app->configNode.child("sceneFight").child("arrowimage").attribute("texturepath").as_string();

	//Create Easings
	easingPos = new Easing();
	easingPos->SetTotalTime(0.5);
	easingPos->SetDelayTime(0.5);
}

// Destructor
SceneFight::~SceneFight()
{}

// Called before render is available
bool SceneFight::Awake(pugi::xml_node& config)
{
	LOG("|| AWAKE SceneFight ||");
	bool ret = true;


	return ret;
}

// Called before the first frame
bool SceneFight::Start()
{
	LOG("--START SceneFight--");

	//Set turn step
	turnStep = TurnStep::FINISHED;

	// Set rand seed
	srand(time(NULL));

	// Set Scale
	app->scaleObj->SetCurrentScale(ScaleType::FIGHT);

	// Play level music
	app->audio->PlayMusic(musicPath, 1.0f);

	/*Load*/
	tex_fightTownBG = app->tex->Load(path_fightTownBG);
	tex_fightCircusBG = app->tex->Load(path_fightCircusBG);
	tex_arrow = app->tex->Load(path_arrow);

	// ======== CREATE TURN LIST ========

	//Add party memebers
	ListItem<PartyMember*>* member = app->partyManager->party.start;
	while (member != NULL)
	{
		turnList.Add(member->data);
		member = member->next;
	}
	
	alliesAlive = turnList.Count() + 1;

	//Add enemies
	const char* enemy = app->partyManager->enemyToFight;
	uchar amount;
	if (enemy == "enemykid") { amount = 3; }
	else if (enemy == "enemyshopkeeper") { amount = 1; }
	else if (enemy == "enemyclown") { amount = 2; }

	for (int i = 0; i < amount; i++)
	{
		pugi::xml_node itemNode = app->configNode.child("sceneFight").child(app->partyManager->enemyToFight);

		//type
		SString typeStr = itemNode.attribute("type").as_string();
		MemberType type = typeStr == "ally" ? MemberType::ALLY : MemberType::ENEMY;

		//texture
		const char* path = itemNode.attribute("texturepath").as_string();
		SDL_Texture* tex = app->tex->Load(path);

		SDL_Rect textureRect;
		SString nameStr = itemNode.attribute("name").as_string();
		if (nameStr == "enemykid")
			textureRect = { 192 + i * 32, 0, 32, 64 };
		else if (nameStr == "enemyshopkeeper")
			textureRect = { 0, 64, 64, 96 };
		else if (nameStr == "enemyclown")
			textureRect = { 128, 64, 64, 96 };

		//battle position
		int offsetX = 850;
		int offsetY = 50;

		iPoint position;
		position.x = (offsetX + 32 * i) / scale;
		position.y = (offsetY + 96 * i) / scale;

		PartyMember* member = new PartyMember(
			type, MemberStatus::NORMAL,
			itemNode.attribute("name").as_string(),
			itemNode.attribute("maxHp").as_uint(),
			itemNode.attribute("maxMana").as_uint(),
			itemNode.attribute("level").as_uint(),
			itemNode.attribute("attack").as_uint(),
			itemNode.attribute("defense").as_uint(),
			itemNode.attribute("speed").as_uint(),
			itemNode.attribute("critRate").as_uint(),
			position, tex, textureRect);

		turnList.Add(member);
		enemyList.Add(member);
	}
	
	enemiesAlive = enemyList.Count();
	enemySelected = 0;

	// Sort by speed - set first turn
	SortBySpeed();
	turn = 0;

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
	uint id = turn % turnList.Count();
	turnMember = turnList.At(id)->data;

	//Check TurnStep
	if (turnStep == TurnStep::FINISHED)
	{
		easingPos->SetFinished(false);

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
				if (enemySelected < enemyList.Count() - 1)
					enemySelected++;
				else
					enemySelected = 0;
			}
		}
		else if (turnMember->type == MemberType::ENEMY)
		{
			app->sceneManager->sceneFight->turnStep = TurnStep::IN;
		}
	}

	ExecuteTurn();	

	// Win/Lose - Back to SceneGame
	if (enemiesAlive <= 0)
	{
		app->sceneManager->sceneState = SceneState::SWITCH;
		app->sceneManager->nextScene = SceneID::SCENE_GAME;
	}
	else if (alliesAlive <= 0)
	{
		app->sceneManager->sceneState = SceneState::SWITCH;
		app->sceneManager->nextScene = SceneID::SCENE_GAME;
	}

	// Delete dead members from turnList
	for (size_t i = 0; i < turnList.Count(); i++)
	{
		if (turnList.At(i)->data->status == MemberStatus::DEAD)
		{
			if (turnList.At(i)->data->type == MemberType::ENEMY)
			{
				enemyList.Del(enemyList.At(enemySelected));

				if (enemySelected >= enemyList.Count())
					enemySelected = 0;
			}

			turnList.Del(turnList.At(i));			
		}			
	}

	return true;
}

bool SceneFight::PostUpdate()
{
	bool ret = true;

	// Draw BG
	SString map = app->sceneManager->sceneGame->mapName;
	
	if (map == "town")
	{
		app->render->DrawTexture(tex_fightTownBG, 0, 0, NULL, SDL_FLIP_NONE, ScaleType::FIGHT);
	}
	else if (map == "circus1" || map == "circus2")
	{
		app->render->DrawTexture(tex_fightCircusBG, 0, 0, NULL, SDL_FLIP_NONE, ScaleType::FIGHT);
	}

	// Draw turnList members
	for (size_t i = 0; i < turnList.Count(); i++)
	{
		app->render->DrawTexture(
			turnList.At(i)->data->texture,
			turnList.At(i)->data->pos.x,
			turnList.At(i)->data->pos.y,
			&turnList.At(i)->data->textureRect,
			SDL_FLIP_NONE, ScaleType::FIGHT);
	}

	// Draw Selected enemy
	app->render->DrawTexture(tex_arrow, enemyList.At(enemySelected)->data->pos.x, enemyList.At(enemySelected)->data->pos.y - 32, NULL, SDL_FLIP_NONE, ScaleType::FIGHT);

	// Draw GUI
	app->guiManager->Draw();

	return ret;
}

bool SceneFight::CleanUp()
{
	LOG("* CLEANUP SceneFight *");

	turnList.Clear();	
	enemyList.Clear();

	app->tex->UnLoad(tex_fightTownBG);
	app->tex->UnLoad(tex_fightCircusBG);
	app->tex->UnLoad(tex_arrow);

	//app->guiManager->guiControlsList.Clear();

	app->scaleObj->SetCurrentScale(ScaleType::WORLD);

	return true;
}


void SceneFight::ExecuteTurn()
{
	switch (turnStep)
	{
		case TurnStep::IN:
		{
			//Go to mid
			double easedTime = easingPos->TrackTime(app->GetDT());
			turnMember->pos.x = easingPos->EasingAnimation(turnMember->initPos.x, app->render->camera.w / 2 / scale, easedTime, EasingType::EASE_INOUT_SIN);
			turnMember->pos.y = easingPos->EasingAnimation(turnMember->initPos.y, app->render->camera.h / 2 / scale - turnMember->textureRect.h, easedTime, EasingType::EASE_INOUT_SIN);

			if (easingPos->GetFinished())
			{
				if (turnMember->type == MemberType::ENEMY)
				{
					int randomMember = rand() % app->partyManager->party.Count();
					Attack(turnMember, app->partyManager->party.At(randomMember)->data);
				}
				else
				{
					Attack(turnMember, app->partyManager->party.At(enemySelected)->data);
				}

				easingPos->SetFinished(false);
				turnStep = TurnStep::ACTION;
			}
		} break;

		case TurnStep::ACTION:
		{
			//Stats display
			/*double easedTime = easingPos->TrackTime(app->GetDT());
			turnMember->pos.x = easingPos->EasingAnimation(turnMember->initPos.x, app->render->camera.w / 2 / scale, easedTime, EasingType::EASE_INOUT_SIN);
			turnMember->pos.y = easingPos->EasingAnimation(turnMember->initPos.y, app->render->camera.h / 2 / scale - turnMember->textureRect.h, easedTime, EasingType::EASE_INOUT_SIN);

			if (easingPos->GetFinished())
			{
				if (turnMember->type == MemberType::ENEMY)
				{
					int randomMember = rand() % app->partyManager->party.Count();
					Attack(turnMember, app->partyManager->party.At(randomMember)->data);
				}
				else
				{
					Attack(turnMember, app->partyManager->party.At(enemySelected)->data);
				}

				turnStep = TurnStep::OUT;
			}*/

			turnStep = TurnStep::OUT;

		} break;
			
		case TurnStep::OUT:
		{
			//Back to initPos
			double easedTime = easingPos->TrackTime(app->GetDT());
			turnMember->pos.x = easingPos->EasingAnimation(app->render->camera.w / 2 / scale, turnMember->initPos.x, easedTime, EasingType::EASE_INOUT_SIN);
			turnMember->pos.y = easingPos->EasingAnimation(app->render->camera.h / 2 / scale - turnMember->textureRect.h, turnMember->initPos.y, easedTime, EasingType::EASE_INOUT_SIN);

			if (easingPos->GetFinished())
			{
				easingPos->SetFinished(false);
				turnStep = TurnStep::FINISHED;
				turn++;
			}

			
		} break;
			
		case TurnStep::FINISHED:

			break;

		default:
			break;
	}
}

bool SceneFight::Attack(PartyMember* turnMember_, PartyMember* receiverMember_)
{
	// HEKATE: Calculate crit, def

	//godmode no damage
	if (turnMember_->type == MemberType::ENEMY && app->debug->godMode) { return false; }

	//check death || godmode instakill
	if ((turnMember_->attack > receiverMember_->currentHp) || 
		(turnMember_->type == MemberType::ALLY && app->debug->godMode))
	{
		receiverMember_->currentHp = 0;
		receiverMember_->status = MemberStatus::DEAD;

		if (receiverMember_->type == MemberType::ENEMY)
			enemiesAlive--;
		else
			alliesAlive--;

		return true;
	}
	else
	{
		receiverMember_->currentHp -= turnMember->attack;

		return false;
	}
}

void SceneFight::Escape()
{
	int randomNumber = rand() % 10;
	
	if (randomNumber % 2)
	{
		app->sceneManager->SwitchTo(SceneID::SCENE_GAME);
	}
	else {
		turn++;
	}
}

void SceneFight::SortBySpeed()
{
	bool swapped = true;

	while (swapped)
	{
		swapped = false;
		ListItem<PartyMember*>* item;
		Entity* pEntity = NULL;

		for (item = turnList.start; item != NULL && item->next != NULL; item = item->next)
		{
			if (item->data->speed > item->next->data->speed)
			{
				SWAP(item->data, item->next->data);
				swapped = true;
			}
		}
	}
}