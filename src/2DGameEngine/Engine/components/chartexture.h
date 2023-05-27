#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "base_component.h"

typedef struct charTexture
{
	Component super;
	Vector pos;
	int lineNumber;
	char **textureString;
	char *resPath;
	char *color;
	int pointSize;
	double width, height;
	bool visible;

	void (*setPos)(struct charTexture *, Vector *);
	Vector *(*getPos)(struct charTexture *);
	double (*getWidth)(struct charTexture *);
	double (*getHeight)(struct charTexture *);
	char *(*getMeta)(struct Component *);
	void (*setMeta)(struct Component *, char *meta);
	void (*resetTexture)(struct charTexture *, char* resPath);

} charTexture;

charTexture *newcharTexture(char *resPath, Vector *pos, char *color, int pointSize);
void destorycharTexture(charTexture *t);

#endif