#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "scene.h"

#define scmng scenesManager

typedef struct ScenesManager{
    SceneNode currentScene;
    SceneNode scenesList;

    void (*addScene)(SceneNode);
    SceneNode (*getScene)(char*);
    void (*checkSwitch)(SceneNode);
} ScenesManager;

extern ScenesManager scenesManager;

void initScenesManager();
void destroyScenesManager();
#endif