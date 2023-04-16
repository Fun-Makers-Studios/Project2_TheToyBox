#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "NPC.h"
#include "Item.h"
#include "Coin.h"
#include "SlimeEnemy.h"
#include "BatEnemy.h"
#include "GuiButton.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	void ResetScene();
	void FixCamera();

	void SaveUI();
	void Checkpoint();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

public:

	//L02: DONE 3: Declare a Player attribute 
	Player* player;
	
	NPC* npc;
	List<NPC*> npcList;

	BatEnemy* bat;

	Item* item;

	Coin* coin;

	// Set of SFX
	uint selectSFX = 0;

	bool cameraFixX = false;
	bool cameraFixY = false;
	iPoint cameraPos;

	bool playing = false;

	SDL_Texture* originTex = nullptr;
	const char* origintexturePath;

	SDL_Texture* slimeTilePathTex = nullptr;
	const char* slimeTilePath;
	
	SDL_Texture* checkPointTex = nullptr;
	const char* checkPointTexPath;
	bool checkpointEnabled = false;

	int saveTime = 0;
	bool showSavingState = false;

	bool gamePaused = false;

	bool continueGame = false;

	List<Coin*> coinsList;

	List<Item*> livesCollectedList;
	int itemLivesCount = 0;

	bool isTeleporting = false;

private:
	SDL_Texture* img_pause;
	SDL_Rect pauseRect;

	iPoint startPosition;
	
	// L12: Debug pathfing
	iPoint origin;
	bool originSelected = false;

	const char* imgPausePath;
	const char* musicPath;
	const char* selectSFXPath;

	// Declare a GUI Button and create it using the GuiManager
	GuiButton* resumeButton14;
	GuiButton* backToTitleButton15;
	GuiButton* settingsButton16;
	GuiButton* closeButton17;

	bool partyMenu = false;

	bool exitGame = false;
};

#endif // __SCENE_H__