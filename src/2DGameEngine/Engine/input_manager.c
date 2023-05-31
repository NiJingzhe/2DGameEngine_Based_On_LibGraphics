#include "input_manager.h"

InputManager inputManager;

void KeyboardEvent(int key, int event);
void MouseEvent(int x, int y, int button, int event);


void initInputManager() {
	inputManager.mouseButtons[0] = 0;
	inputManager.mouseButtons[1] = 0;
	inputManager.mouseButtons[2] = 0;

	memset(inputManager.keyStates, 0, sizeof(inputManager.keyStates));

	inputManager.mouseX = 0;
	inputManager.mouseY = 0;
	inputManager.keyBoardEventType = -9;
	inputManager.mouseEventType = -9;

	registerKeyboardEvent(KeyboardEvent);
	registerMouseEvent(MouseEvent);

}
void clearEvent(){
	int i = 0;
	while((GetAsyncKeyState(i) & 0x8000 ? 1 : 0) && i < 256){
		i++;
	}
	if (i == 255)
		inputManager.keyBoardEventType = -9;
	
	if(!((GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0) || 
	   (GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0) ||
	   (GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0))) 
		inputManager.mouseEventType = -9;
}

void KeyboardEvent(int key, int event)
{
    for (int i = 0; i < 256; i++) {
		inputManager.keyStates[i] = GetAsyncKeyState(i) & 0x8000 ? 1 : 0;
	}
	inputManager.keyBoardEventType = event;
}

void MouseEvent(int x, int y, int button, int event) {
	inputManager.mouseX = ScaleXInches(x);
	inputManager.mouseY = ScaleYInches(y);

	inputManager.mouseButtons[0] = button == VK_LBUTTON;
	inputManager.mouseButtons[1] = button == VK_MBUTTON;
	inputManager.mouseButtons[2] = button == VK_RBUTTON;

	inputManager.mouseEventType = event;

	#if MOUSE_DEBUG
		printf("\nLOG:\nMouse Event: %d, mouse position: (%lf, %lf)", event, inputManager.mouseX, inputManager.mouseY);
	#endif
}