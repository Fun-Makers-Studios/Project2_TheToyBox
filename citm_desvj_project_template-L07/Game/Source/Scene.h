#ifndef __SCENE_H__
#define __SCENE_H__

#include "NPC.h"
#include "Item.h"

class GuiControl;

enum class SceneType
{
	ALWAYS_ACTIVE,
	GAME,
	FIGHT
};

enum class SceneID
{
	SCENE_LOGO,
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_FIGHT,
	SCENE_ENDING
};

class Scene
{
public:

	Scene() {}

	virtual ~Scene() {}

	// Called when scene initially created. Called once.
	virtual bool Awake(pugi::xml_node& config) = 0;

	// Called whenever a scene is transitioned into. Can be 
	// called many times in a typical game cycle.
	virtual bool Start() { return true; };

	// Called before all Updates
	virtual bool PreUpdate() { return true; };

	// Called each loop iteration
	virtual bool Update(float dt) { return true; };

	// Called before all Updates
	virtual bool PostUpdate() { return true; };

	// Called before quitting
	virtual bool CleanUp() { return true; };

	virtual bool OnGuiMouseClickEvent(GuiControl* control) { return true; }

	// Returns the scene's ID
	virtual SceneID GetID() { return id; }

	// Sets the scene's ID
	virtual void SetID(SceneID id) { this->id = id; }

public:

	SceneType sceneType;
	SceneID id;
	List<NPC*>npcs;
	List<Item*>items;
};

#endif // __SCENE_H__