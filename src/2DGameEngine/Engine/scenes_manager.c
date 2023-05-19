#include "scenes_manager.h"
#include "scene.h"

ScenesManager scenesManager;

static void addScene(SceneNode scene);
static void switchTo(char *target, bool unloadCur, bool setupNext, void *param, size_t size);
static SceneNode getScene(char *meta);
static void loadScene(SceneNode *scene, CreateSceneFunction func, void* param);
static void checkSwitch();

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
    scenesManager.target = NULL;
    scenesManager.unloadCur = FALSE;
    scenesManager.setupNext = TRUE;

    scenesManager.checkSwitch = checkSwitch;
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
        scenesManager.target = NULL;
        return;
    }
    else
    {
        scenesManager.target = target;
    }

    if (size > 0 && param != NULL)
    {
        void *new_param = malloc(size);
        memcpy(new_param, param, size);
        if (scenesManager.param != NULL)
        {
            free(scenesManager.param);
#if MEM_DEBUG
            MEM_BLOCK_NUM--;
            printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
            printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
        }
        scenesManager.param = new_param;
    }
    else
    {
        if (scenesManager.param != NULL)
        {
            free(scenesManager.param);
#if MEM_DEBUG
            MEM_BLOCK_NUM--;
            printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
            printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
        }
        scenesManager.param = NULL;
    }

    scenesManager.unloadCur = unloadCur;
    scenesManager.setupNext = setupNext;
}

static void checkSwitch()
{
    if (scenesManager.target == NULL)
        return;

    if (scenesManager.unloadCur)
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

#if MEM_DEBUG
    printf("\nLOG:\nSuccessfully unloaded last scene!\n");
#endif

    scenesManager.currentScene = scenesManager.getScene(scenesManager.target);
    scenesManager.target = NULL;
#if MEM_DEBUG
    printf("\nLOG:\nAfter getScene, now current scene is: %p, and meta is: %s\n", scenesManager.currentScene, scenesManager.currentScene->meta);
#endif
    if (scenesManager.setupNext)
        scenesManager.currentScene->setup(scenesManager.currentScene, scenesManager.param);
}

static void loadScene(SceneNode *scene, CreateSceneFunction func, void* param)
{
#if MEM_DEBUG
    printf("\nLOG:\nstart load scene...\n");
#endif
    func(scene);
    (*scene)->setup(*scene, param);
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
    return;
}
