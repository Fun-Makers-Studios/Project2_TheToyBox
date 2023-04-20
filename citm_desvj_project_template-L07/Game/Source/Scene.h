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

	Player* player = nullptr;
	
	NPC* npc;
	List<NPC*> npcList;

	KidEnemy* kid = nullptr;

	Item* item = nullptr;
	Coin* coin = nullptr;

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

	uint partyMemberSelected = 0;

	DialogueManager* dialogueManager;

private:
	SDL_Texture* img_pause = nullptr;
	SDL_Rect pauseRect;

	const char* popImg_settingsPath;
	SDL_Texture* popImg_settings = nullptr;

	SDL_Texture* partyMenuImg = nullptr;

	SDL_Texture* zeroImg = nullptr;
	SDL_Texture* sophieImg = nullptr;

	iPoint startPosition;
	
	// L12: Debug pathfing
	iPoint origin;
	bool originSelected = false;

	const char* imgPausePath = nullptr;
	const char* musicPath = nullptr;
	const char* selectSFXPath = nullptr;

	// Declare a GUI Button and create it using the GuiManager
	GuiButton* resumeButton14 = nullptr;
	GuiButton* backToTitleButton15 = nullptr;
	GuiButton* settingsButton16 = nullptr;
	GuiButton* closeButton17 = nullptr;
	
	// Settings menu on scene game
	GuiButton* decreaseMusicButton21 = nullptr;
	GuiButton* increaseMusicButton22 = nullptr;

	GuiButton* decreaseSFXButton23 = nullptr;
	GuiButton* increaseSFXButton24 = nullptr;

	GuiButton* fullscreenButton25 = nullptr;
	GuiButton* vsyncButton26 = nullptr;

	GuiButton* firstPMemberButton27 = nullptr;
	GuiButton* secondPMemberButton28 = nullptr;

	bool exitGame = false;
};

#endif // __SCENE_H__