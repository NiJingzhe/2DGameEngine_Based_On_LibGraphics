#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "scene.h"

#define scmng scenesManager

typedef void (*CreateSceneFunction)(SceneNode* scene);

typedef struct ScenesManager{
    SceneNode currentScene;
    SceneNode lastScene;
    SceneNode scenesList;

    void* param;

    SceneNode (*getScene)(char*);
    void (*switchTo)(char*, bool, bool, void*, size_t);
    void (*loadScene)(SceneNode*, CreateSceneFunction);
    void (*unloadScene)(char* target);
} ScenesManager;

extern ScenesManager scenesManager;

void initScenesManager();
void destroyScenesManager();
#endif