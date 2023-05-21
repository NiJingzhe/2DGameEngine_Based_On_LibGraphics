#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "base_component.h"

typedef struct Texture
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

	void (*setPos)(struct Texture *, Vector *);
	Vector *(*getPos)(struct Texture *);
	double (*getWidth)(struct Texture *);
	double (*getHeight)(struct Texture *);
	char *(*getMeta)(struct Component *);
	void (*setMeta)(struct Component *, char *meta);
	void (*resetTexture)(struct Texture *, char* resPath);

} Texture;

Texture *newTexture(char *resPath, Vector *pos, char *color, int pointSize);
void destoryTexture(Texture *t);


#endif