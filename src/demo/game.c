#include "2DEngine.h"
#include "scene_info.h"
#include "game_conf.h"

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
    scmng.loadScene(&STARTUP_SCENE, STARTUP_SCENE_CREATOR, NULL);
    scmng.currentScene = STARTUP_SCENE;
    scmng.currentScene->setup(scmng.currentScene, NULL);
#if ENGINE_DEBUG
    LOG("Game init finished!----------------------------------------------------------------------------------------------------------------------");
#endif
}

void CALLBACK TimerCallBack(HWND a, UINT b, UINT_PTR id, DWORD d)
{
    if (switchTimer != NULL && id == switchTimer->id)
    {
        switchTimer->callBackFunction();
    }
    if (timer != NULL && id == timer->id)
    {
        timer->callBackFunction();
    }
    if (freezSkillTimer != NULL && id == freezSkillTimer->id){
        freezSkillTimer->callBackFunction();
    }
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
