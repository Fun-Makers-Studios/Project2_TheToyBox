#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "NPC.h"
#include "Item.h"
#include "Coin.h"
#include "SlimeEnemy.h"
#include "KidEnemy.h"
#include "GuiButton.h"
#include "DialogueManager.h"

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

	void SceneMap();
	void LoadNPC(SString mapName_);

	void SaveUI();
	void Checkpoint();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	void FightKid();

public:

	//L02: DONE 3: Declare a Player attribute 
	Player* player;
	
	NPC* npc;
	List<NPC*> npcList;

	KidEnemy* kid;

	Item* item;

	Coin* coin;

	// Set of SFX
	uint selectSFX = 0;

	bool cameraFixX = false;
	bool cameraFixY = false;
	iPoint cameraPos;

	bool playing = false;

	SDL_Texture* checkPointTex = nullptr;
	const char* checkPointTexPath;
	bool checkpointEnabled = false;

	int saveTime = 0;
	bool showSavingState = false;

	bool gamePaused = false;

	bool partyMenu = false;
	bool pauseMenu = false;
	bool settingSceneMenu = false;

	bool continueGame = false;

	List<Coin*> coinsList;

	List<Item*> livesCollectedList;
	int itemLivesCount = 0;

	SString mapName;
	bool isMapChanging = false;

	DialogueManager* dialogueManager;

private:
	SDL_Texture* img_pause;
	SDL_Rect pauseRect;

	const char* popImg_settingsPath;
	SDL_Texture* popImg_settings = nullptr;

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
	
	// Settings menu on scene game
	GuiButton* decreaseMusicButton21;
	GuiButton* increaseMusicButton22;

	GuiButton* decreaseSFXButton23;
	GuiButton* increaseSFXButton24;

	GuiButton* fullscreenButton25;
	GuiButton* vsyncButton26;


	bool exitGame = false;
};

#endif // __SCENE_H__