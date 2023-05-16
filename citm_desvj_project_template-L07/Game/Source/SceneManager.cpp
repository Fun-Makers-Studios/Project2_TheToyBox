#include "App.h"

#include "SceneManager.h"
#include "MenuManager.h"
#include "TransitionManager.h"

#include "SceneLogo.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneFight.h"
#include "SceneEnding.h"

SceneManager::SceneManager() : Module()
{
    name.Create("sceneManager");
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
    sceneState = SceneState::CONTINUE;
    currentScene = FindSceneByID(SceneID::SCENE_LOGO)->data;
    currentScene->Start();

    return true;
}

bool SceneManager::PreUpdate()
{
    if (sceneState == SceneState::SWITCH)
    {
        if (app->transitionManager->step == TransitionStep::NONE)
        {
            app->transitionManager->LoadTransition();
        }
        else if (app->transitionManager->step == TransitionStep::SWITCH)
        {
            SwitchTo(nextScene);
            app->menuManager->SetDefaultMenu();
        }
        else if (app->transitionManager->step == TransitionStep::FINISHED)
        {
            sceneState = SceneState::CONTINUE;
            app->transitionManager->step = TransitionStep::NONE;
        }       
    }

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
    scene->Awake(config);

    return true;
}

void SceneManager::SwitchTo(SceneID id)
{
    currentScene->CleanUp();
    previousScene = currentScene->id;
    
    auto scene = FindSceneByID(id);
    currentScene = scene->data;
    currentScene->Start();
}

void SceneManager::RemoveScene(SceneID id)
{
    auto scene = FindSceneByID(id);

    delete scene;

    scenes.Del(scene);
}

ListItem<Scene*>* SceneManager::FindSceneByID(SceneID id)
{
    ListItem<Scene*>* scene;

    for (scene = scenes.start; scene != NULL; scene = scene->next)
    {
        if (scene->data->GetID() == id) { return scene; }
    }
}

bool SceneManager::LoadState(pugi::xml_node& data)
{

    ListItem<Scene*>* scene;

    for (scene = scenes.start; scene != NULL; scene = scene->next)
    {
        scene->data->LoadState(data);
    }

    return true;
}

bool SceneManager::SaveState(pugi::xml_node& data)
{

    ListItem<Scene*>* scene;

    for (scene = scenes.start; scene != NULL; scene = scene->next)
    {
        scene->data->SaveState(data);
    }

    return true;
}

