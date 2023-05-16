#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__
#include "public_headers.h"

#define inmng inputManager

typedef struct InputManager {
    double mouseX;
    double mouseY;
    int mouseButtons[3];
    int keyStates[256];
    int keyBoardEventType;
    int mouseEventType;
} InputManager;

extern InputManager inputManager;

void initInputManager();
void clearEvent();


#endif