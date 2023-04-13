#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "NPC.h"
#include "Item.h"
#include "List.h"

class Scene
{
public:

	Scene() {}

	// Called when scene initially created. Called once.
	virtual void OnCreate(pugi::xml_node& config) = 0;

	// Called when scene destroyed. Called at most once (if a scene 
	// is not removed from the game, this will never be called).
	virtual void OnDestroy() = 0;

	// Called whenever a scene is transitioned into. Can be 
	// called many times in a typical game cycle.
	virtual void OnActivate() {};

	// Called whenever a transition out of a scene occurs. 
	// Can be called many times in a typical game cycle.
	virtual void OnDeactivate() {};

	// Called before all Updates
	virtual void PreUpdate() {};

	// Called each loop iteration
	virtual void Update(float dt) {};

	// Called before all Updates
	virtual void PostUpdate() {};

	// Called before quitting
	virtual void CleanUp() {};

	// Returns the scene's ID
	virtual SString GetID() { return id; }

	// Sets the scene's ID
	virtual void SetID(SString id) { this->id = id; }

public:

	SString id;
	List<NPC*>npcs;
	List<Item*>items;
};

#endif // __SCENE_H__