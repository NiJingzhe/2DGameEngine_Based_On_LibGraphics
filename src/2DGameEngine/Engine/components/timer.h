#ifndef __TIMER_H__
#define __TIMER_H__
#include "base_component.h"

typedef struct Timer
{
	Component super;
	int id;
	bool enable;
	int interval;
	TIMERPROC callBackFunction;

	void (*start)(struct Timer*);
	void (*stop)(struct Timer*);
	void (*setInterval)(struct Timer*, int interval);
	char *(*getMeta)(struct Component *);
	void (*setMeta)(struct Component *, char *meta);
} Timer;

Timer *newTimer(int id, int interval, TIMERPROC callBackFunction);
void destoryTimer(struct Timer *timer);


#endif