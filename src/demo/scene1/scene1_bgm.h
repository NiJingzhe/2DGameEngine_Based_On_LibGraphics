#ifndef __SCENE1_BGM_H__
#define __SCENE1_BGM_H__
#include "2DEngine.h"

extern ActorNode scene1BGM;
extern Audio *bgm;

void createScene1BGM();
void setupScene1BGM(void* param);
void bgmUpdate(ActorNode scene1BGM, double delta);
#endif