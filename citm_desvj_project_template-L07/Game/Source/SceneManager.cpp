#include "App.h"
#include "SceneManager.h"
#include "SceneLogo.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneFight.h"
#include "SceneEnding.h"

SceneManager::SceneManager() : Module()
{
    name.Create("scenemanager");
}

SceneManager::~SceneManager() {}

bool SceneManager::Awake(pugi::xml_node& config)
{
    sceneLogo    = new SceneLogo();
    sceneTitle   = new SceneTitle();
    sceneGame    = new SceneGame();
    sceneFight   = new SceneFight();
    sceneEnding  = new SceneEnding();

    AddScene(sceneLogo, config);
    AddScene(sceneTitle, config);
    AddScene(sceneGame, config);
    AddScene(sceneFight, config);
    AddScene(sceneEnding, config);

    return true;
}

bool SceneManager::Start()
{
    currentScene = FindSceneByID("SceneLogo")->data;

    return true;
}

bool SceneManager::PreUpdate()
{
    if (currentScene)
    {
        currentScene->PreUpdate();
        return true;
    }
    return false;
}

bool SceneManager::Update(float dt)
{
    if (currentScene)
    {
        currentScene->Update(dt);
        return true;
    }
    return false;
}

bool SceneManager::PostUpdate()
{
    if (currentScene)
    {
        currentScene->PostUpdate();
        return true;
    }
    return false;
}

bool SceneManager::AddScene(Scene* scene, pugi::xml_node& config)
{
    if (scene == nullptr) return false;

    scenes.Add(scene);
    numScenes++;
    scene->Awake(config);

    return true;
}

void SceneManager::SwitchTo(SString id)
{
    auto scene = FindSceneByID(id);

    switch (currentScene->sceneType)
    {
    case SceneType::ALWAYS_ACTIVE:
        currentScene = scene->data;
        break;
    case SceneType::GAME:
        delete currentScene;

        break;
    case SceneType::FIGHT:
        break;
    default:
        break;
    }

    currentScene->Start();
}

void SceneManager::RemoveScene(SString id)
{
    auto scene = FindSceneByID(id);

    delete scene;

    scenes.Del(scene);
}

ListItem<Scene*>* SceneManager::FindSceneByID(SString id)
{
    ListItem<Scene*>* scene;

    for (scene = scenes.start; scene != NULL; scene = scene->next)
    {
        if (scene->data->GetID() == id) { return scene; }
    }
}