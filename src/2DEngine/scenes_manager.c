#include "scenes_manager.h"
#include "scene.h"

ScenesManager scenesManager;

static void addScene(SceneNode scene);
static void checkSwitch(SceneNode current);
static SceneNode getScene(char* meta);

void initScenesManager(){

    scenesManager.currentScene = NULL;
    scenesManager.scenesList = NULL;

    scenesManager.addScene = addScene;
    scenesManager.getScene = getScene;
    scenesManager.checkSwitch = checkSwitch;
}

static void addScene(SceneNode scene){
    SceneNode currentScene = scenesManager.scenesList;
    if (currentScene == NULL){
        scene->prev = NULL;
        scene->next = NULL;
        scenesManager.scenesList = scene;
        return;
    }

    while(currentScene->next){
        currentScene = currentScene->next;
    }
    currentScene->next = scene;
    scene->prev = currentScene;
    scene->next = NULL;

    return;
}

static SceneNode getScene(char* meta){
    if (scenesManager.scenesList == NULL){
		return NULL;
	}
	
	SceneNode curScene = scenesManager.scenesList;
	while(curScene->next && strcmp(curScene->getMeta(curScene), meta) != 0){
		curScene = curScene->next;
	}	

	if (strcmp(curScene->getMeta(curScene), meta) == 0){
		return curScene;
	}
	else{
		return NULL;
	}
}

static void checkSwitch(SceneNode current){
    if (strcmp(current->switchTarget, "") != 0){
        char* tempTarget = current->switchTarget;
        current->switchTarget = "";
        scenesManager.currentScene = scenesManager.getScene(tempTarget);
    }
}

void destroyScenesManager(){
    SceneNode currentScene = scenesManager.scenesList;
    if (currentScene == NULL){
        return;
    }

    while(currentScene->next){
        currentScene = currentScene->next;
        destoryScene(currentScene->prev);
    }
    destoryScene(currentScene);
    return;
}
