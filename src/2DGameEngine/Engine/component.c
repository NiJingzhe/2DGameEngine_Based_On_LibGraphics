#include "component.h"
#include "public_headers.h"

/*--------------------------------Function Declaration Part------------------------------------*/
static void initComponent(Component *c, ComponentRender render, ComponentUpdate update);
static char *getComponentMeta(Component *c);
static void setComponentMeta(Component *c, char *meta);
static const int returnEmptyType();

static void initCollisionShape(CollisionShape *c, Shape *shape);
static void renderCollisionShape(Component *c);
static void updateCollisionShape(Component *c, ...);
static bool isCollideWith(CollisionShape *s1, CollisionShape *s2);
static const int returnCollisionShape();
static void setCollisionShapePos(CollisionShape *c, Vector *pos);
static Vector *getCollisionShapePos(CollisionShape *c);
static void destoryCollisionShape(CollisionShape *cs);

static void initTexture(Texture *t, char *resPath, Vector pos, char *color, int pointSize);
static void renderTexture(Component *c);
static void updateTexture(Component *c, ...); // ... should contain and only contain a Vector pointer represents the position of texture
static void setTexturePos(Texture *t, Vector *pos);
static Vector *getTexturePos(Texture *t);
static double getWidth(Texture *t);
static double getHeight(Texture *t);
static const int returnTexture();
static void destoryTexture(Texture *t);

static void initAudio(struct Audio *audio, char *filePath, bool loop);
static void renderAudio(Component *c);
static void updateAudio(Component *c, ...);
static const int returnAudioType(ComponentNode c);
static void Audio_play(struct Audio *audio);
static void Audio_pause(struct Audio *audio);
static void Audio_stop(struct Audio *audio);
static void Audio_setVolume(struct Audio *audio, int volume);
static char *Audio_getFilePath(struct Audio *audio);
static void destoryAudio(struct Audio *audio);

static void initTimer(struct Timer *timer, int id, int interval, TIMERPROC callbackFunction);
static void renderTimer(Component *c);
static void updateTimer(Component *c, ...);
static void start(struct Timer *timer);
static void stop(struct Timer *timer);
static void setInterval(struct Timer *timer, int interval);
static const int returnTimerType(ComponentNode c);
static void destoryTimer(struct Timer *timer);

static void initUIText(struct UIText *uiText, char *content, Vector pos, char *color, char *font, int style, int pointSize);
static void renderUIText(Component *c);
static void updateUIText(Component *c, ...);
static void setUITextPos(UIText *t, Vector *pos);
static Vector *getUITextPos(UIText *t);
static void setUITextContent(UIText *t, char *content);
static char *getUITextContent(UIText *t);
static double getUITextWidth(UIText *t);
static const int returnUITextType(ComponentNode c);
static void destoryUIText(struct UIText *uiText);

/*------------------------------Empty Component Part---------------------------------*/

Component *newComponent(ComponentRender render, ComponentUpdate update)
{
	Component *component = (Component *)calloc(1, sizeof(Component));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	initComponent(component, render, update);
	return component;
}

static void initComponent(Component *c, ComponentRender render, ComponentUpdate update)
{
	c->vptr = (componentvTable *)calloc(1, sizeof(componentvTable));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	if (c->vptr == NULL)
	{
		printf("Cannot allocate memory for component vptr");
		return;
	}
	c->vptr->render = render;
	c->vptr->update = update;
	c->vptr->getComponentType = returnEmptyType;
	c->getMeta = getComponentMeta;
	c->setMeta = setComponentMeta;

	c->next = NULL;
	c->prev = NULL;

	c->meta = "ComponentBaseClass";
}

static char *getComponentMeta(Component *c)
{
	return c->meta;
}

static void setComponentMeta(Component *c, char *meta)
{
	c->meta = meta;
}

static const int returnEmptyType()
{
	return EMPTY_COMPONENT;
}

void destoryComponent(Component *c)
{
#if MEM_DEBUG
	printf("\nLOG:\n		Enter destoryComponent, destorying component: %s\n", c->meta);
#endif
	switch (c->vptr->getComponentType())
	{
	case EMPTY_COMPONENT:
		free(c->vptr);
#if MEM_DEBUG
		MEM_BLOCK_NUM--;
		printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
		free(c);
#if MEM_DEBUG
		MEM_BLOCK_NUM--;
		printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
		c = NULL;
		break;

	case COLLISION_SHAPE:
		destoryCollisionShape((CollisionShape *)c);
		break;

	case TEXTURE:
		destoryTexture((Texture *)c);
		break;

	case AUDIO:
		destoryAudio((Audio *)c);
		break;

	case TIMER:
		destoryTimer((Timer *)c);
		break;

	case UITEXT:
		destoryUIText((UIText *)c);
		break;
	}
}

/*--------------------------------CollisionShape Part------------------------------------*/

CollisionShape *newCollisionShape(Shape *shape)
{
	CollisionShape *collisionShape = (CollisionShape *)calloc(1, sizeof(CollisionShape));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	initCollisionShape(collisionShape, shape);
	return collisionShape;
}

static void initCollisionShape(CollisionShape *c, Shape *shape)
{
	switch (shape->vptr->getShape())
	{
	case RECT:
		c->shape = (Shape *)calloc(1, sizeof(Rect));
#if MEM_DEBUG
		MEM_BLOCK_NUM++;
		printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
		if (c->shape == NULL)
		{
			printf("Cannot allocate memory for shape of collionShape component\n");
			return;
		}
		memcpy(c->shape, (Rect *)shape, sizeof(Rect));
		c->shape->vptr = NULL;
		c->shape->vptr = (shapevTable *)calloc(1, sizeof(shapevTable));
#if MEM_DEBUG
		MEM_BLOCK_NUM++;
		printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
		memcpy(c->shape->vptr, shape->vptr, sizeof(shapevTable));
		break;

	case CIRCLE:
		c->shape = (Shape *)calloc(1, sizeof(Circle));
#if MEM_DEBUG
		MEM_BLOCK_NUM++;
		printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
		if (c->shape == NULL)
		{
			printf("Cannot allocate memory for shape of collionShape component\n");
			return;
		}
		memcpy(c->shape, (Circle *)shape, sizeof(Circle));
		c->shape->vptr = NULL;
		c->shape->vptr = (shapevTable *)calloc(1, sizeof(shapevTable));
#if MEM_DEBUG
		MEM_BLOCK_NUM++;
		printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
		memcpy(c->shape->vptr, shape->vptr, sizeof(shapevTable));
		break;

	default:
		printf("YOU SHOULD SPECIFIC A TYPE OF SHAPE!!!\n");
		return;
	}

	Component *super = newComponent(renderCollisionShape, updateCollisionShape);
	memcpy(&(c->super), super, sizeof(Component));
	c->super.vptr = NULL;
	c->super.vptr = (componentvTable *)calloc(1, sizeof(componentvTable));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	if (c->super.vptr == NULL)
	{
		printf("Cannot allocate memory for vptr of collionShape component\n");
		return;
	}
	memcpy(c->super.vptr, super->vptr, sizeof(componentvTable));
	c->super.vptr->getComponentType = returnCollisionShape;
	destoryComponent(super);

	c->enable = TRUE;
	c->pos.x = c->shape->pos.x;
	c->pos.y = c->shape->pos.y;
	c->getMeta = c->super.getMeta;
	c->setMeta = c->super.setMeta;
	c->getPos = getCollisionShapePos;
	c->setPos = setCollisionShapePos;
	c->visible = FALSE;

	c->isCollideWith = isCollideWith;
}

static void renderCollisionShape(Component *c)
{
	CollisionShape *cs = (CollisionShape *)c;
	if (cs->visible)
		cs->shape->vptr->render(cs->shape);
	else
		return;
}

static void updateCollisionShape(Component *c, ...)
{
	CollisionShape *cs = (CollisionShape *)c;
	Vector *pos = NULL;
	va_list argList;
	va_start(argList, c);
	pos = va_arg(argList, Vector *);
	cs->shape->setPos(cs->shape, pos);
	cs->setPos(cs, pos);
	va_end(argList);
}

static bool isCollideWith(CollisionShape *s1, CollisionShape *s2)
{
	if (s1->enable && s2->enable)
	{
		if (s1->shape->isCollideWith(s1->shape, s2->shape))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

static void setCollisionShapePos(CollisionShape *c, Vector *pos)
{
	memcpy(&(c->pos), pos, sizeof(Vector));
}
static Vector *getCollisionShapePos(CollisionShape *c)
{
	return &(c->pos);
}

static const int returnCollisionShape()
{
	return COLLISION_SHAPE;
}

static void destoryCollisionShape(CollisionShape *cs)
{
	free(cs->super.vptr);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	destoryShape(cs->shape);
	free(cs);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	cs = NULL;
}

/*--------------------------------Texture Part------------------------------------*/

Texture *newTexture(char *resPath, Vector *pos, char *color, int pointSize)
{
	Texture *t = (Texture *)calloc(1, sizeof(Texture));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	initTexture(t, resPath, *pos, color, pointSize);
	return t;
}

static void initTexture(Texture *t, char *resPath, Vector pos, char *color, int pointSize)
{

	Component *super = newComponent(renderTexture, updateTexture);
	memcpy(&(t->super), super, sizeof(Component));
	t->super.vptr = NULL;
	t->super.vptr = (componentvTable *)calloc(1, sizeof(componentvTable));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	if (t->super.vptr == NULL)
	{
		printf("Cannot allocate memory for vptr of Texture component\n");
		return;
	}
	memcpy(t->super.vptr, super->vptr, sizeof(componentvTable));
	t->super.vptr->getComponentType = returnTexture;
	destoryComponent(super);

	t->color = color;
	t->resPath = resPath;
	t->pos = pos;

	t->setPos = setTexturePos;
	t->getPos = getTexturePos;
	t->getHeight = getHeight;
	t->getWidth = getWidth;

	t->getMeta = t->super.getMeta;
	t->setMeta = t->super.setMeta;

	FILE *textureFile = fopen(resPath, "r");
	if (textureFile == NULL)
	{
		printf("CANNOT OPEN TEXTURE! PLEASE CHAECK YOUR PATH!");
		return;
	}

	t->textureString = (char **)calloc(1, sizeof(char *));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	t->lineNumber = 0;
	t->pointSize = pointSize;

	char buffer[500] = {'\0'};
	while (!feof(textureFile))
	{

		t->textureString = (char **)realloc(t->textureString, (t->lineNumber + 1) * sizeof(char *));
		fgets(buffer, 500, textureFile);
		t->textureString[t->lineNumber] = (char *)calloc(strlen(buffer) + 1, sizeof(char));
#if MEM_DEBUG
		MEM_BLOCK_NUM++;
		printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
		strcpy(t->textureString[t->lineNumber], buffer);
		t->textureString[t->lineNumber][strlen(t->textureString[t->lineNumber])] = '\0';
		memset(buffer, '\0', sizeof(buffer));
		t->lineNumber++;
	}
	fclose(textureFile);

	int pointSize_ = GetPointSize();
	SetFont("Courier New");
	SetStyle(Bold);
	SetPointSize(t->pointSize);
	t->height = t->lineNumber * (TextStringWidth(t->textureString[t->lineNumber - 1]) / strlen(t->textureString[t->lineNumber - 1]) * 1.01);
	t->width = TextStringWidth(t->textureString[t->lineNumber - 1]);
	SetPointSize(pointSize_);
}

static void updateTexture(Component *c, ...)
{
	Texture *t = (Texture *)c;
	Vector *pos = NULL;
	va_list argList;
	va_start(argList, c);
	pos = va_arg(argList, Vector *);
	t->setPos(t, pos);
	va_end(argList);
}

static void renderTexture(Component *c)
{
	Texture *t = (Texture *)c;
	if (t->visible)
	{
		if (t->textureString == NULL)
		{
			return;
		}
		SetEraseMode(FALSE);
		SetPenColor(t->color);
		int pointSize_ = GetPointSize();
		SetFont("Courier New");
		SetStyle(Bold);
		SetPointSize(t->pointSize);
		for (int i = 0; i < t->lineNumber; ++i)
		{
			MovePen(t->pos.x - t->width / 2.0, t->pos.y + (double)(t->lineNumber / 2 - i) * (t->height / t->lineNumber));
			DrawTextString(t->textureString[i]);
		}
		SetPointSize(pointSize_);
	}
	else
	{
		return;
	}
}

static void setTexturePos(Texture *t, Vector *pos)
{
	t->pos = *pos;
}

static Vector *getTexturePos(Texture *t)
{
	return &(t->pos);
}

static double getHeight(Texture *t)
{
	return t->height;
}

static double getWidth(Texture *t)
{
	return t->width;
}

static const int returnTexture()
{
	return TEXTURE;
}

static void destoryTexture(Texture *t)
{
	free(t->super.vptr);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	for (int i = 0; i < t->lineNumber; ++i)
	{
		free(t->textureString[i]);
#if MEM_DEBUG
		MEM_BLOCK_NUM--;
		printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	}
	free(t->textureString);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	free(t);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	t = NULL;
}

/*---------------------------Audio Part------------------------------*/
Audio *newAudio(char *filePath, bool loop)
{
	Audio *audio = (Audio *)malloc(sizeof(Audio));
	initAudio(audio, filePath, loop);
	return audio;
}

static void initAudio(struct Audio *audio, char *filePath, bool loop)
{

	Component *super = newComponent(renderAudio, updateAudio);
	memcpy(&(audio->super), super, sizeof(Component));
	audio->super.vptr = NULL;
	audio->super.vptr = (componentvTable *)calloc(1, sizeof(componentvTable));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	if (audio->super.vptr == NULL)
	{
		printf("Cannot allocate memory for vptr of audio component\n");
		return;
	}
	memcpy(audio->super.vptr, super->vptr, sizeof(componentvTable));
	destoryComponent(super);
	audio->super.vptr->getComponentType = returnAudioType;
	audio->super.meta = NULL;
	audio->super.next = NULL;
	audio->super.prev = NULL;

	audio->filePath = filePath;
	audio->playing = FALSE;
	audio->loop = loop;
	audio->volume = 100;
	audio->play = Audio_play;
	audio->pause = Audio_pause;
	audio->stop = Audio_stop;
	audio->setVolume = Audio_setVolume;
	audio->getFilePath = Audio_getFilePath;
	audio->getMeta = audio->super.getMeta;
	audio->setMeta = audio->super.setMeta;

	char command[256];
	sprintf(command, "open \"%s\" type mpegvideo alias audio%d", audio->filePath, (int)audio);
	mciSendString(command, NULL, 0, NULL);
}

static void renderAudio(Component *c)
{
	return;
}

static void updateAudio(Component *c, ...)
{
	return;
}

static const int returnAudioType(ComponentNode c)
{
	return AUDIO;
}

static void Audio_play(struct Audio *audio)
{
	char command[256];
	if (!audio->loop)
	{
		sprintf(command, "play audio%d", (int)audio);
		mciSendString(command, NULL, 0, NULL);
	}
	else
	{
		sprintf(command, "play audio%d repeat", (int)audio);
		mciSendString(command, NULL, 0, NULL);
	}

	audio->playing = TRUE;
}

static void Audio_pause(struct Audio *audio)
{
	char command[256];
	sprintf(command, "pause audio%d", (int)audio);
	mciSendString(command, NULL, 0, NULL);

	audio->playing = FALSE;
}

static void Audio_stop(struct Audio *audio)
{
	char command[256];
	sprintf(command, "stop audio%d", (int)audio);
	mciSendString(command, NULL, 0, NULL);
	sprintf(command, "seek audio%d to %ld", (int)audio, 0L);
	mciSendString(command, NULL, 0, NULL);

	audio->playing = FALSE;
}

static void Audio_setVolume(struct Audio *audio, int volume)
{
	char command[256];
	sprintf(command, "setaudio audio%d volume to %d", (int)audio, volume);
	mciSendString(command, NULL, 0, NULL);

	audio->volume = volume;
}

static char *Audio_getFilePath(struct Audio *audio)
{
	return audio->filePath;
}

static void destoryAudio(struct Audio *audio)
{
	char command[256];
	sprintf(command, "close audio%d", (int)audio);
	mciSendString(command, NULL, 0, NULL);
	free(audio->super.vptr);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	free(audio);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
}

/*---------------------------Timer Part------------------------------*/
Timer *newTimer(int id, int interval, TIMERPROC callBackFunction)
{
	Timer *timer = (Timer *)malloc(sizeof(Timer));
	initTimer(timer, id, interval, callBackFunction);
	return timer;
}

static void initTimer(struct Timer *timer, int id, int interval, TIMERPROC callbackFunction)
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
	timer->enable = TRUE;
	startTimer(timer->id, timer->interval, timer->callBackFunction);
}

static void stop(struct Timer *timer)
{
	timer->enable = FALSE;
	cancelTimer(timer->id);
}

static void setInterval(struct Timer *timer, int interval)
{
	if (timer->enable = TRUE)
	{
		cancelTimer(timer->id);
		timer->interval = interval;
		startTimer(timer->id, timer->interval, timer->callBackFunction);
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

static void destoryTimer(struct Timer *timer)
{
	if (timer->enable)
	{
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

/*---------------------------UIText Part----------------------------*/

UIText *newUIText(char *content, Vector *pos, char *color, char *font, int style, int pointSize)
{
	UIText *uiText = (UIText *)calloc(1, sizeof(UIText));
	initUIText(uiText, content, *pos, color, font, style, pointSize);
	return uiText;
}

static void initUIText(struct UIText *uiText, char *content, Vector pos, char *color, char *font, int style, int pointSize)
{
	Component *super = newComponent(renderUIText, updateUIText);
	memcpy(&(uiText->super), super, sizeof(Component));
	uiText->super.vptr = NULL;
	uiText->super.vptr = (componentvTable *)calloc(1, sizeof(componentvTable));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	if (uiText->super.vptr == NULL)
	{
		printf("Cannot allocate memory for vptr of Texture component\n");
		return;
	}
	memcpy(uiText->super.vptr, super->vptr, sizeof(componentvTable));
	uiText->super.vptr->getComponentType = returnUITextType;
	destoryComponent(super);

	uiText->pos = pos;
	uiText->content = content == NULL ? "" : content;
	uiText->font = font == NULL ? "Consolas" : font;
	uiText->pointSize = pointSize;
	uiText->style = style;
	uiText->color = color;

	double pointSize_ = GetPointSize();
	SetPointSize(uiText->pointSize);
	SetFont(uiText->font);
	SetStyle(uiText->style);
	uiText->width = TextStringWidth(uiText->content);
	SetPointSize(pointSize_);

	uiText->visible = TRUE;

	uiText->setContent = setUITextContent;
	uiText->getContent = getUITextContent;
	uiText->setPos = setUITextPos;
	uiText->getPos = getUITextPos;
	uiText->getWidth = getUITextWidth;
	uiText->setMeta = uiText->super.setMeta;
	uiText->getMeta - uiText->super.getMeta;
}

static void setUITextPos(UIText *t, Vector *pos)
{
	memcpy(&(t->pos), pos, sizeof(Vector));
}

static Vector *getUITextPos(UIText *t)
{
	return &(t->pos);
}

static void setUITextContent(UIText *t, char *content)
{
	t->content = content;
	t->width = TextStringWidth(t->content);
}

static char *getUITextContent(UIText *t)
{
	return t->content;
}

static double getUITextWidth(UIText *t)
{
	return TextStringWidth(t->content);
}

static void renderUIText(Component *c)
{
	UIText *t = (UIText *)c;
	if (t->visible)
	{	
		SetPenColor(t->color);
		double pointSize_ = GetPointSize();
		SetPointSize(t->pointSize);
		SetFont(t->font);
		SetStyle(t->style);
		double width = TextStringWidth(t->content);
		t->width = width;
		MovePen(t->pos.x - width / 2, t->pos.y);
		DrawTextString(t->content);
		SetPointSize(pointSize_);
	}
	else
	{
		return;
	}
}

static void updateUIText(Component *c, ...)
{
	UIText *t = (UIText *)c;
	Vector *pos = NULL;
	va_list argList;
	va_start(argList, c);
	pos = va_arg(argList, Vector *);
	t->setPos(t, pos);
	va_end(argList);
}

static const int returnUITextType(ComponentNode c)
{
	return UITEXT;
}

static void destoryUIText(struct UIText *uiText)
{
	free(uiText->super.vptr);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	free(uiText);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
}