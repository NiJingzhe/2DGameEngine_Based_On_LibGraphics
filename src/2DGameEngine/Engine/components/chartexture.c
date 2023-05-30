#include "chartexture.h"
#include "base_component.h"
static void initTexture(charTexture *t, char *resPath, Vector pos, char *color, int pointSize);
static void renderTexture(Component *c);
static void updateTexture(Component *c, ...); // ... should contain and only contain a Vector pointer represents the position of texture
static void setTexturePos(charTexture *t, Vector *pos);
static void resetTexture(charTexture *t, char *resPath);
static Vector *getTexturePos(charTexture *t);
static double getWidth(charTexture *t);
static double getHeight(charTexture *t);
static const int returnTexture();

charTexture *newcharTexture(char *resPath, Vector *pos, char *color, int pointSize)
{
	charTexture *t = (charTexture *)calloc(1, sizeof(charTexture));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	initTexture(t, resPath, *pos, color, pointSize);
	return t;
}

static void initTexture(charTexture *t, char *resPath, Vector pos, char *color, int pointSize)
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
	t->resetTexture = resetTexture;

	t->getMeta = t->super.getMeta;
	t->setMeta = t->super.setMeta;

	FILE *textureFile = fopen(resPath, "r");
	if (textureFile == NULL)
	{
		Error("CANNOT OPEN CHARTEXTURE! PLEASE CHAECK YOUR PATH!\n");
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

static void resetTexture(charTexture *t, char *resPath)
{
	FILE *textureFile = fopen(resPath, "r");
	if (textureFile == NULL)
	{
		Error("CANNOT OPEN CHARTEXTURE! PLEASE CHAECK YOUR PATH!\n");
		return;
	}

	if (t->textureString != NULL)
	{
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
	}

	t->textureString = (char **)calloc(1, sizeof(char *));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	t->lineNumber = 0;

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
	charTexture *t = (charTexture *)c;
	Vector *pos = NULL;
	va_list argList;
	va_start(argList, c);
	pos = va_arg(argList, Vector *);
	t->setPos(t, pos);
	va_end(argList);
}

static void renderTexture(Component *c)
{
	charTexture *t = (charTexture *)c;
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
		SetPointSize(t->pointSize * globalCamera.zoom);
		for (int i = 0; i < t->lineNumber; ++i)
		{
			double width = TextStringWidth(t->textureString[i]);
			double height = width / strlen(t->textureString[i]) * 1.01 * t->lineNumber;
			Vector* posUnderCam = newVector(0,0);
			posUnderCam->x = (t->pos.x - globalCamera.position.x)*globalCamera.zoom + getww / 2;
			posUnderCam->y = (t->pos.y - globalCamera.position.y)*globalCamera.zoom + getwh / 2;
			MovePen(posUnderCam->x - width / 2.0, posUnderCam->y + (double)(t->lineNumber / 2 - i) * (height / t->lineNumber));
			DrawTextString(t->textureString[i]);
			t->width = width;
			t->height = height;
		}
		SetPointSize(pointSize_);
	}
	else
	{
		return;
	}
}

static void setTexturePos(charTexture *t, Vector *pos)
{
	t->pos = *pos;
}

static Vector *getTexturePos(charTexture *t)
{
	return &(t->pos);
}

static double getHeight(charTexture *t)
{
	return t->height;
}

static double getWidth(charTexture *t)
{
	return t->width;
}

static const int returnTexture()
{
	return CHARTEXTURE;
}

void destorycharTexture(charTexture *t)
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
