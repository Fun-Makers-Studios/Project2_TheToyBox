#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__


#include "Module.h"
#include "Scene.h"
#include "List.h"

class SceneManager : public Module
{
public:

	SceneManager(bool startEnabled);

	virtual ~SceneManager();

	bool Awake(pugi::xml_node& config);
	bool Start();

	// This will call the corresponding methods of the active scene.
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();


	// Adds a scene to the scene manager.
	bool AddScene(Scene* scene, pugi::xml_node& config);

	// Transitions to a scene.
	void SwitchTo(SString id);

	// Removes scene from scene manager.
	void RemoveScene(SString id);

	ListItem<Scene*>* FindSceneByID(SString id);

	Scene* GetCurrentScene() { return currentScene; };

private:
	// Stores all of the scenes associated with this scene manager
	List<Scene*> scenes;

	// Stores a reference to the current scene. Used when drawing/updating.
	Scene* currentScene = nullptr;

	// Total scenes on list. This is incremented whenever a scene is added.
	uint numScenes = 0;
};

#endif // __SCENEMANAGER_H__