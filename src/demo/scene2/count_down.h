#ifndef __COUNT_DOWN_H__
#define __COUNT_DOWN_H__

#include "2DEngine.h"

// 倒计时UI
extern Vector *countDownPos;
extern ActorNode countDownUI;
extern Timer *timer;
extern UIText *countDownText;

extern long long int countDown;

void createCountDown();
void setupCountDown(void * param);

#endif