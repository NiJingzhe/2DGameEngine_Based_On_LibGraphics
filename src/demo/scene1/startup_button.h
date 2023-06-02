#ifndef __STARTUP_BUTTON_H__
#define __STARTUP_BUTTON_H__
#include "2DEngine.h"

extern Vector *startButtonPos;
extern ActorNode startButton;
extern picTexture *startButtonTexture;

void startButtonUpdate(ActorNode button, double delta);
void createStartupButton();
void setupStartupButton(void* param);

#endif