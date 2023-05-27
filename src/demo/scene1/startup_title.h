#ifndef __STARTUP_TITLE_H__
#define __STARTUP_TITLE_H__
#include "2DEngine.h"

extern Vector *titlePos;
extern ActorNode title;
extern charTexture *titlecharTexture;

void titleRender(ActorNode title);
void createStartupTitle();
void setupStartupTitle(void* param);

#endif