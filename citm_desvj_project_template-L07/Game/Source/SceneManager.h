#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "Module.h"
#include "List.h"
#include "SceneLogo.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneFight.h"
#include "SceneEnding.h"

enum class SceneState
{
	CONTINUE,
	SWITCH
};

class SceneManager : public Module
{
public:

	SceneManager();
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
	void SwitchTo(SceneID id);

	// Removes scene from scene manager.
	void RemoveScene(SceneID id);

	ListItem<Scene*>* FindSceneByID(SceneID id);

public:

	Scene* currentScene = nullptr;
	SceneState sceneState = SceneState::CONTINUE;
	SceneID nextScene;

	List<Scene*> scenes;

	//Scenes
	SceneLogo* sceneLogo;
	SceneTitle* sceneTitle;
	SceneGame* sceneGame;
	SceneFight* sceneFight;
	SceneEnding* sceneEnding;
};

#endif // __SCENE_MANAGER_H__