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
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();


	void ResetScene();
	void FixCamera();

	void SceneMap();
	void LoadNPC();
	void LoadItems();
	void LoadPuzzles();

	void ActiveParticles();
	void SaveUI();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	void FightKid();

	void FastDebugForSave();

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

	bool continueGame = false;
	bool newGame = true;

	SString mapName;
	bool isMapChanging = false;

	uint partyMemberSelected = 0;
	uint questListSelected = 0;

	DialogueManager* dialogueManager = nullptr;
	
	// Audio
	const char* musicPath = nullptr;
private:

	bool isCheckpointEnabled = false;

	bool isNight = false;
	bool exitGame = false;

	// Textures

	int fight, saved;

	// Particle system
	ParticleSystem* smokePS = nullptr;
	ParticleSystem* firefliesPS = nullptr;
	ParticleSystem* walkParticles = nullptr;
	ParticleSystem* fireTorchPS = nullptr;
	List<ParticleSystem*> particlesList;

	// Easings
	Easing* easingPause = nullptr;
	Easing* easingButton = nullptr;
};

#endif // __SCENEGAME_H__