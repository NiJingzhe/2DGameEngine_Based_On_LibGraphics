#include "scene1_bgm.h"

ActorNode scene1BGM;
Audio *bgm;

void createScene1BGM()
{
    Vector* bgmPos = newVector(0, 0);
    scene1BGM = newActor("scene1_bgm", bgmPos);
    bgm = newAudio("./res/scene1/bgm.mp3", TRUE);
    scene1BGM->addComponent(scene1BGM, (ComponentNode)bgm);
    destoryVector(bgmPos);
}

void setupScene1BGM(void *param)
{
    bgm->setMeta((ComponentNode)bgm, "scene1_bgm");
    scene1BGM->vptr->update = bgmUpdate;
}

void bgmUpdate(ActorNode scene1BGM, double delta)
{
    Audio *bgm = (Audio *)(scene1BGM->getComponent(scene1BGM, "scene1_bgm"));
    if (!bgm->playing)
    {
        bgm->play(bgm);
    }
}
