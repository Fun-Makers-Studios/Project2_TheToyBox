#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

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
	bool questMenu = false;
	bool settingSceneMenu = false;


	bool playing = false; //HEKATE not used
	bool continueGame = false;

	SString mapName;
	bool isMapChanging = false;

	uint partyMemberSelected = 0;
	uint questListSelected = 0;

	DialogueManager* dialogueManager;

private:

	iPoint startPosition;
	iPoint mousePos;
	iPoint cameraPos;

	bool isNight = false;
	bool exitGame = false;

	// Textures
	const char* partyMenuImgPath = nullptr;
	SDL_Texture* partyMenuImg = nullptr;
	
	const char* zeroImgPath = nullptr;
	SDL_Texture* zeroImg = nullptr;
	
	const char* sophieImgPath = nullptr;
	SDL_Texture* sophieImg = nullptr;
	
	// Audio
	const char* musicPath = nullptr;

	// Debug pathfinding
	iPoint origin;
	bool originSelected = false;

	//UI

	GuiButton* firstPMemberButton27 = nullptr;
	GuiButton* secondPMemberButton28 = nullptr;

	GuiButton* doneQuestsButton29 = nullptr;
	GuiButton* activeQuestsButton30 = nullptr;

	// Particle system
	ParticleSystem* smokePS;
	ParticleSystem* firefliesPS;

	//EASINGS
	Easing* easingPause = nullptr;
	Easing* easingButton = nullptr;
};

#endif // __SCENE_GAME_H__