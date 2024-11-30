#ifndef __UI_TEXT_H__
#define __UI_TEXT_H__

#include "base_component.h"
#include "vector.h"

typedef struct UIText{
	Component super;
	char* content;
	char* font;
	char* color;
	int style;
	int pointSize;
	double width;
	bool visible;
	Vector pos;

	void (*setContent)(struct UIText* ,char*);
	char* (*getContent)(struct UIText*);
	void (*setPos)(struct UIText *, Vector *);
	Vector *(*getPos)(struct UIText *);
	char* (*getMeta)(struct Component*);
	void (*setMeta)(struct Component*, char* meta);
	double (*getWidth)(struct UIText*);
} UIText;

UIText *newUIText(char* content, Vector* pos, char* color, char* font, int style, int pointSize);
void destoryUIText(struct UIText *uiText);

#endif