#include "2DEngine.h"
#include "scene_info.h"

void Main() {
	InitGraphics();
    //InitConsole();
    initInputManager();
    initScenesManager();
    scmng.loadScene(&scene1, createScene1);
    scmng.currentScene = scene1;
    scmng.currentScene->setup(scmng.currentScene, NULL);
    LOG("Game init finished!");
}

void EngineUpdate(double delta) {
    scmng.currentScene->update(scmng.currentScene, delta);
    clearEvent();
}

void Render() {
    scmng.currentScene->render(scmng.currentScene);
}

void Free() {
    destroyScenesManager();
}
