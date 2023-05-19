#ifndef __SCENE_INFO_H__
#define __SCENE_INFO_H__
#include "2DEngine.h"
extern SceneNode scene1;
extern ActorNode scene1Background;
extern ActorNode title;
extern ActorNode startButton;
extern ActorNode scene1BGM;

extern SceneNode scene2;
extern ActorNode scene2Backgound;
extern ActorNode pauseButton;
extern ActorNode player;
extern ActorNode player2;
extern ActorNode countDownUI;

extern SceneNode scene3;

void createScene1(SceneNode* scene1);
void createScene2(SceneNode* scene2);
void createScene3(SceneNode* scene3);
#endif