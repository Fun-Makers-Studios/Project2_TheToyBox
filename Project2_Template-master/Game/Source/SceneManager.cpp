#include "App.h"
#include "SceneManager.h"
#include "SceneTest.h"

SceneManager::SceneManager(bool startEnabled) : Module(startEnabled)
{
    name.Create("scenemanager");
}

SceneManager::~SceneManager(){}

bool SceneManager::Awake(pugi::xml_node& config)
{
    Scene* sceneTest = new SceneTest();
    AddScene(sceneTest, config);

    return true;
}

bool SceneManager::Start()
{
    currentScene = FindSceneByID("sceneTest")->data;

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
    scene->OnCreate(config);

    return true;
}

void SceneManager::SwitchTo(SString id)
{
    auto scene = FindSceneByID(id);

    if (currentScene)
    {
        // If we have a current scene, we call its OnDeactivate method.
        currentScene->OnDeactivate();
    }

    // Setting new current scene
    currentScene = scene->data;

    currentScene->OnActivate();
}

void SceneManager::RemoveScene(SString id)
{
    auto scene = FindSceneByID(id);

    if (currentScene == scene->data)
    {
        // If the scene we are removing is the current scene, 
        // set it to null pointer so the scene is no longer updated.
        currentScene = nullptr;
    }

    // We make sure to call the OnDestroy method 
    // of the scene we are removing.
    scene->data->OnDestroy();

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

