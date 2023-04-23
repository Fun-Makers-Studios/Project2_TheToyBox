#include "UI.h"

#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Input.h"
#include "Player.h"
#include "Scene.h"
#include "Window.h"
#include "PartyManager.h"

#include "SDL/include/SDL.h"

#include "Defs.h"
#include "Log.h"


UI::UI():Module() 
{
	name.Create("UI_Module");
}

UI::~UI()
{}

bool UI::Awake(pugi::xml_node& config) 
{
	LOG("Loading UI_Module");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool UI::Start()
{
	LOG("--STARTS UI MODULE--");
	
	/*Initialize*/
	font1Path = app->configNode.child("ui").child("font1").attribute("texturepath").as_string();
	font2Path = app->configNode.child("ui").child("font2").attribute("texturepath").as_string();
	font2_RedPath = app->configNode.child("ui").child("font2Red").attribute("texturepath").as_string();
	livesTexPath = app->configNode.child("scene").child("life").attribute("texturepath").as_string();
	coinsTexPath = app->configNode.child("scene").child("coin").attribute("texturepath").as_string();

	//Loading font 1
	char lookupTableFont1[] = { "! @,_./0123456789$;<&?abcdefghijklmnopqrstuvwxyz" };
	font1_id = app->fonts->Load(font1Path, lookupTableFont1, 2);

	char lookupTableFont2[] = { "! %&'()*+,-./0123456789:;<=>abcdefghijklmnopqrstuvwxyz" };
	font2_id = app->fonts->Load(font2Path, lookupTableFont2, 1);
	
	char lookupTableFont2Red[] = { "! %&'()*+,-./0123456789:;<=>abcdefghijklmnopqrstuvwxyz" };
	font2Red_id = app->fonts->Load(font2_RedPath, lookupTableFont2Red, 1);

	livesTex = app->tex->Load(livesTexPath);
	coinsTex = app->tex->Load(coinsTexPath);

	return true;
}

// Called each loop iteration
bool UI::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool UI::Update(float dt)
{
	return true;
}

// Called each loop iteration
bool UI::PostUpdate()
{
	return true;
}

// Called before quitting
bool UI::CleanUp()
{
	LOG("Freeing UI_Module");

	app->fonts->UnLoad(font1_id);
	app->fonts->UnLoad(font2_id);
	app->tex->UnLoad(livesTex);
	app->tex->UnLoad(coinsTex);

	return true;
}

/* UI DEBUG INFO WITH FONT 2 */
void UI::BlitPlayerXPos()
{
	char playerXPos[25];
	sprintf_s(playerXPos, 25, "position x: %d", app->scene->player->position.x);
	app->fonts->BlitText(20, 65, font2_id, playerXPos);
}

void UI::BlitPlayerYPos()
{
	char playerYPos[25];
	sprintf_s(playerYPos, 25, "position y: %d", app->scene->player->position.y);
	app->fonts->BlitText(20, 85, font2_id, playerYPos);
}


void UI::BlitFPS()
{
	char fps[25];
	sprintf_s(fps, 25, "fps: %d", app->GetFPS());
	app->fonts->BlitText(870, 15, font2_id, fps);
}

void UI::BlitAverageFPS()
{
	char averageFps[25];
	sprintf_s(averageFps, 25, "average fps; %.2f", app->GetAverageFPS());
	app->fonts->BlitText(825, 40, font1_id, averageFps);
}

void UI::BlitDT()
{
	char dt[25];
	sprintf_s(dt, 25, "delta time; %.2f ms", app->GetDT());
	app->fonts->BlitText(825, 50, font1_id, dt);
}

void UI::BlitTimeSinceStart()
{
	char timeSinceStart[25];
	sprintf_s(timeSinceStart, 25, "time since start; %.2f", app->GetTimesSinceStart());
	app->fonts->BlitText(825, 60, font1_id, timeSinceStart);
}

void UI::BlitFrameCount()
{
	char frameCount[25];
	sprintf_s(frameCount, 25, "frame count; %d", app->GetFrameCount());
	app->fonts->BlitText(825, 70, font1_id, frameCount);
}

void UI::BlitPartyStats()
{
	
	app->fonts->BlitText(544, 225, app->ui->font1_id, app->partyManager->party.At(app->scene->partyMemberSelected)->data->name.GetString());

	char level[5];
	sprintf_s(level, 5, "%d", app->partyManager->party.At(app->scene->partyMemberSelected)->data->level);
	app->fonts->BlitText(544, 264, app->ui->font1_id, level);

	char currentHP[8];
	sprintf_s(currentHP, 8, "hp %d/", app->partyManager->party.At(app->scene->partyMemberSelected)->data->currentHp);
	char maxHP[3];
	sprintf_s(maxHP, 3, "%d", app->partyManager->party.At(app->scene->partyMemberSelected)->data->maxHp);
	app->fonts->BlitText(780, 270, app->ui->font1_id, currentHP);
	app->fonts->BlitText(828, 270, app->ui->font1_id, maxHP);

	char currentMana[10];
	sprintf_s(currentMana, 10, "mana %d/", app->partyManager->party.At(app->scene->partyMemberSelected)->data->currentMana);
	char maxMana[3];
	sprintf_s(maxMana, 3, "%d", app->partyManager->party.At(app->scene->partyMemberSelected)->data->maxMana);
	app->fonts->BlitText(875, 270, app->ui->font1_id, currentMana);
	app->fonts->BlitText(940, 270, app->ui->font1_id, maxMana);

	char attack[12];
	sprintf_s(attack, 12, "attack %d", app->partyManager->party.At(app->scene->partyMemberSelected)->data->attack);
	app->fonts->BlitText(780, 295, app->ui->font1_id, attack);

	char defense[15];
	sprintf_s(defense, 15, "defense %d", app->partyManager->party.At(app->scene->partyMemberSelected)->data->defense);
	app->fonts->BlitText(875, 295, app->ui->font1_id, defense);

	char speed[10];
	sprintf_s(speed, 10, "speed %d", app->partyManager->party.At(app->scene->partyMemberSelected)->data->speed);
	app->fonts->BlitText(780, 320, app->ui->font1_id, speed);

	char critRate[15];
	sprintf_s(critRate, 15, "crit r %d", app->partyManager->party.At(app->scene->partyMemberSelected)->data->critRate);
	app->fonts->BlitText(875, 320, app->ui->font1_id, critRate);

}