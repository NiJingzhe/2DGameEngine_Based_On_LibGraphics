#include "scenes_manager.h"
#include "scene.h"

ScenesManager scenesManager;

static void addScene(SceneNode scene);
static void switchTo(char *target, bool unloadCur, bool setupNext, void *param, size_t size);
static SceneNode getScene(char *meta);
static void loadScene(SceneNode *scene, CreateSceneFunction func);

static void addScene(SceneNode scene)
{
    SceneNode currentScene = scenesManager.scenesList;
    if (currentScene == NULL)
    {
        scene->prev = NULL;
        scene->next = NULL;
        scenesManager.scenesList = scene;
        return;
    }

    while (currentScene->next)
    {
        currentScene = currentScene->next;
    }
    currentScene->next = scene;
    scene->prev = currentScene;
    scene->next = NULL;

    return;
}

void initScenesManager()
{

    scenesManager.currentScene = NULL;
    scenesManager.lastScene = NULL;
    scenesManager.scenesList = NULL;

    scenesManager.param = NULL;

    scenesManager.loadScene = loadScene;
    scenesManager.getScene = getScene;
    scenesManager.switchTo = switchTo;
}

static SceneNode getScene(char *meta)
{
    if (scenesManager.scenesList == NULL)
    {
        return NULL;
    }

    SceneNode curScene = scenesManager.scenesList;
    while (curScene->next && strcmp(curScene->getMeta(curScene), meta) != 0)
    {
        curScene = curScene->next;
    }

    if (strcmp(curScene->getMeta(curScene), meta) == 0)
    {
        return curScene;
    }
    else
    {
        return NULL;
    }
}

static void switchTo(char *target, bool unloadCur, bool setupNext, void *param, size_t size)
{
    if (target == NULL || scenesManager.getScene(target) == NULL)
    {
        printf("The scene %s have not been loaded\n", target);
        return;
    }

    if (size > 0)
    {
        void *new_param = malloc(size);
        memcpy(new_param, param, size);
        if (scenesManager.param != NULL)
        {
            free(scenesManager.param);
        }
        scenesManager.param = new_param;
    }
    else
    {
        if (scenesManager.param != NULL)
        {
            free(scenesManager.param);
        }
        scenesManager.param = NULL;
    }

    if (unloadCur)
    {
        if (scenesManager.currentScene->prev != NULL)
        {
            scenesManager.currentScene->prev->next = scenesManager.currentScene->next;
        }
        else
        {
            scenesManager.scenesList = scenesManager.currentScene->next;
        }
        if (scenesManager.currentScene->next != NULL)
        {
            scenesManager.currentScene->next->prev = scenesManager.currentScene->prev;
        }
        scenesManager.currentScene->exit(scenesManager.currentScene);
        scenesManager.currentScene = NULL;
        scenesManager.lastScene = NULL;
    }
    else
    {
        scenesManager.lastScene = scenesManager.currentScene;
    }

    printf("\nLOG:\nSuccessfully unloaded last scene!\n");
    scenesManager.currentScene = scenesManager.getScene(target);
    printf("\nLOG:\nAfter getScene, now current scene is: %p, and meta is: %s\n", scenesManager.currentScene, scenesManager.currentScene->meta);
    if (setupNext)
        scenesManager.currentScene->setup(scenesManager.currentScene, param);
}

static void loadScene(SceneNode *scene, CreateSceneFunction func)
{
    printf("\nLOG:\nstart load scene...\n");
    func(scene);
    (*scene)->setup(*scene, NULL);
    addScene(*scene);
    return;
}

void destroyScenesManager()
{
    SceneNode currentScene = scenesManager.scenesList;
    if (currentScene == NULL)
    {
        return;
    }

    while (currentScene->next)
    {
        currentScene = currentScene->next;
        destoryScene(currentScene->prev);
    }
    destoryScene(currentScene);
    free(scenesManager.scenesList);
    return;
}
