#ifndef __PAUSE_BUTTON_H__
#define __PAUSE_BUTTON_H__

#include "2DEngine.h"

extern Vector *pauseButtonPos;
extern ActorNode pauseButton;
extern charTexture *pauseButtonTexture;

void createPauseButton();
void setupPauseButton(void *param);


#endif