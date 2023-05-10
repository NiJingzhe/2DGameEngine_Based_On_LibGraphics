#include "2DEngine.h"
#include "scene1.h"
#include "scene2.h"

void Main() {
	InitGraphics();
    //InitConsole();
    initInputManager();
    initScenesManager();
    setupScene_scene1();
    setupScene_scene2();

    scmng.addScene(scene1);
    scmng.addScene(scene2);
    scmng.currentScene = scene1;
}

void EngineUpdate(double delta) {
    scmng.checkSwitch(scmng.currentScene);
    scmng.currentScene->update(scmng.currentScene, delta);
    clearEvent();
}

void Render() {
    scmng.currentScene->render(scmng.currentScene);
}

void Free() {
    destroyScenesManager();
}
