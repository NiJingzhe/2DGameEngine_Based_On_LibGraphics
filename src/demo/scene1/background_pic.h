#ifndef __BACKGROUND_PIC_H__
#define __BACKGROUND_PIC_H__
#include "2DEngine.h"

extern ActorNode background_pic;
extern picTexture *background;
extern picTexture *backgroundWithText;
extern Timer *switchTimer;
void createBackgroundPic();
void setupBackgroundPic(void* param);
#endif