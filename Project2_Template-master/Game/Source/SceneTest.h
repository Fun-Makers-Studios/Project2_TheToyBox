#ifndef __SCENETEST_H__
#define __SCENETEST_H__

#include "Animation.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Player.h"
#include "NPC.h"
#include "Item.h"

struct SDL_Texture;

class SceneTest : public Scene
{
public:

	SceneTest();
	virtual ~SceneTest();

	void OnCreate(pugi::xml_node&) override;
	void OnDestroy() override;

	void OnActivate();
	void OnDeactivate();

	void PreUpdate();
	void Update(float dt);
	void PostUpdate();

	void CleanUp();

	virtual SString GetID() { return id; }
	virtual void SetID(SString id) { this->id = id; }

	virtual List<NPC*>* GetNPCList() { return npcs; }
	virtual void SetNPCList(List<NPC*>* npcs) { this->npcs = npcs; }

public:

	Player* player;
	//List<Chest*> rings;

	const char* musicPath;

	Animation animation;

	bool debug;

	bool questMenu;

	int points;

	SString id;
	List<NPC*>* npcs;
	SDL_Texture* img;

	int font_text;
	char pointsText[6] = { "\0" };
};

#endif // __SCENETEST_H__