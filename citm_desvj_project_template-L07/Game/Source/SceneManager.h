#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

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

//Scenes
class SceneLogo;
class SceneTitle;
class SceneGame;
class SceneFight;
class SceneEnding;

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
	void SwitchTo(SString id);

	// Removes scene from scene manager.
	void RemoveScene(SString id);

	ListItem<Scene*>* FindSceneByID(SString id);

public:

	Scene* currentScene = nullptr;
	SceneState sceneState = SceneState::CONTINUE;
	SString nextScene;

	List<Scene*> scenes;
	uint numScenes = 0;

	//Scenes
	SceneLogo* sceneLogo;
	SceneTitle* sceneTitle;
	SceneGame* sceneGame;
	SceneFight* sceneFight;
	SceneEnding* sceneEnding;
};

#endif // __SCENEMANAGER_H__