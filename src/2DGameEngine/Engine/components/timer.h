#ifndef __TIMER_H__
#define __TIMER_H__
#include "base_component.h"

typedef void (*TimerProcFunc)();
typedef struct Timer
{
	Component super;
	int id;
	bool enable;
	int interval;

	void (*callBackFunction)();
	void (*start)(struct Timer*);
	void (*stop)(struct Timer*);
	void (*setInterval)(struct Timer*, int interval);
	char *(*getMeta)(struct Component *);
	void (*setMeta)(struct Component *, char *meta);
} Timer;

Timer *newTimer(int id, int interval, TimerProcFunc callBackFunction);
void destoryTimer(struct Timer *timer);

extern ComponentNode GLOBAL_TIMER_LIST;

#endif