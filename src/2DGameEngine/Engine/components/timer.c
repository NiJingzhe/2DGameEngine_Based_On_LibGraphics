#include "timer.h"
#include "base_component.h"

static void initTimer(struct Timer *timer, int id, int interval, TimerProcFunc callbackFunction);
static void renderTimer(Component *c);
static void updateTimer(Component *c, ...);
static void start(struct Timer *timer);
static void stop(struct Timer *timer);
static void setInterval(struct Timer *timer, int interval);
static const int returnTimerType(ComponentNode c);

Timer *newTimer(int id, int interval, TimerProcFunc callBackFunction)
{
	Timer *timer = (Timer *)malloc(sizeof(Timer));
	initTimer(timer, id, interval, callBackFunction);
	return timer;
}

static void initTimer(struct Timer *timer, int id, int interval, TimerProcFunc callbackFunction)
{
	Component *super = newComponent(renderTimer, updateTimer);
	memcpy(&(timer->super), super, sizeof(Component));
	timer->super.vptr = NULL;
	timer->super.vptr = (componentvTable *)calloc(1, sizeof(componentvTable));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	if (timer->super.vptr == NULL)
	{
		printf("Cannot allocate memory for vptr of audio component\n");
		return;
	}
	memcpy(timer->super.vptr, super->vptr, sizeof(componentvTable));
	destoryComponent(super);

	timer->super.vptr->getComponentType = returnTimerType;
	timer->callBackFunction = callbackFunction;
	timer->interval = interval;
	timer->id = id;
	timer->enable = FALSE;

	timer->start = start;
	timer->stop = stop;
	timer->setInterval = setInterval;
	timer->getMeta = timer->super.getMeta;
	timer->setMeta = timer->super.setMeta;
}

static void renderTimer(Component *c)
{
	return;
}

static void updateTimer(Component *c, ...)
{
	return;
}

static void start(struct Timer *timer)
{
	if (!timer->enable)
	{
		timer->enable = TRUE;
		startTimer(timer->id, timer->interval, TimerCallBack);
	}
}

static void stop(struct Timer *timer)
{
	if (timer->enable)
	{
		timer->enable = FALSE;
		cancelTimer(timer->id);
	}
}

static void setInterval(struct Timer *timer, int interval)
{
	if (timer->enable = TRUE)
	{
		cancelTimer(timer->id);
		timer->interval = interval;
		startTimer(timer->id, timer->interval, TimerCallBack);
	}
	else
	{
		timer->interval = interval;
	}
}

static const int returnTimerType(ComponentNode c)
{
	return TIMER;
}

void destoryTimer(struct Timer *timer)
{
	if (timer->enable)
	{
		timer->enable = FALSE;
		cancelTimer(timer->id);
	}
	free(timer->super.vptr);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	free(timer);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
}
