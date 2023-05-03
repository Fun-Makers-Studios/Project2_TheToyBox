#ifndef __SCENEGAME_H__
#define __SCENEGAME_H__

#include "Scene.h"
#include "Player.h"
#include "NPC.h"
#include "Item.h"
#include "KidEnemy.h"
#include "GuiButton.h"
#include "ParticleSystem.h"
#include "DialogueManager.h"
#include "Spline.h"
#include "Easing.h"

struct SDL_Texture;

class SceneGame : public Scene
{
public:

	SceneGame();

	// Destructor
	virtual ~SceneGame();

	bool Awake(pugi::xml_node&) override;


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
	void LoadNPC();
	void LoadItems();

	void ActiveParticles();
	void SaveUI();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	void FightKid();


public:

	// Entities
	Player* player = nullptr;	
	NPC* npc;
	List<NPC*> npcList;
	KidEnemy* kid = nullptr;
	Item* item = nullptr;
	List<Item*> itemsList;

	// Set of SFX
	uint selectSFX = 0;

	// Save Game
	SDL_Texture* saveTex = nullptr;
	const char* saveTexPath;
	bool saveEnabled = false;
	int saveTime = 0;
	bool showSavingState = false;

	// Menu states
	bool gamePaused = false;
	bool partyMenu = false;
	bool pauseMenu = false;
	bool settingSceneMenu = false;


	bool playing = false; //HEKATE not used
	bool continueGame = false;

	SString mapName;
	bool isMapChanging = false;

	uint partyMemberSelected = 0;

	DialogueManager* dialogueManager;

	/*Particles FX Vars*/
	ParticleSystem* takeItemPS = nullptr;
	bool takeItem = false;

private:

	iPoint startPosition;
	iPoint mousePos;
	iPoint cameraPos;


	// Textures
	SDL_Texture* img_pause = nullptr;
	SDL_Rect pauseRect;

	const char* popImg_settingsPath = nullptr;
	SDL_Texture* popImg_settings = nullptr;

	const char* partyMenuImgPath = nullptr;
	SDL_Texture* partyMenuImg = nullptr;
	
	const char* zeroImgPath = nullptr;
	SDL_Texture* zeroImg = nullptr;
	
	const char* sophieImgPath = nullptr;
	SDL_Texture* sophieImg = nullptr;
	
	// Debug pathfinding
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

	// Particle system
	ParticleSystem* smokePS;

	bool isNight = false;

	//EASINGS
	Easing* easingPause = nullptr;
	Easing* easingButton = nullptr;


	bool exitGame = false;
};

#endif // __SCENEGAME_H__