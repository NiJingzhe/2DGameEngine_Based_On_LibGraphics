#include "2DEngine.h"
#include "scene_info.h"

void Main()
{
    InitGraphics();
#if ENGINE_DEBUG
    InitConsole();
#endif
#if ENGINE_DEBUG
    LOG("Enter Game Init--------------------------------------------------------------------------------------------------------------------------");
#endif
    srand(time(0));
    initInputManager();
    initScenesManager();
    scmng.loadScene(&scene1, createScene1);
    scmng.currentScene = scene1;
    scmng.currentScene->setup(scmng.currentScene, NULL);
#if ENGINE_DEBUG
    LOG("Game init finished!----------------------------------------------------------------------------------------------------------------------");
#endif
}

void EngineUpdate(double delta)
{
    scmng.checkSwitch();
    scmng.currentScene->update(scmng.currentScene, delta);
    clearEvent();
}

void Render()
{
    scmng.currentScene->render(scmng.currentScene);
}

void Free()
{
#if ENGINE_DEBUG
    LOG("Enter Free-------------------------------------------------------------------------------------------------------------------------------");
#endif
    destroyScenesManager();
#if ENGINE_DEBUG
    LOG("Finish Free------------------------------------------------------------------------------------------------------------------------------");
#endif
}
