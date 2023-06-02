#ifndef __BASE_COMP_H__
#define __BASE_COMP_H__

#include "public_headers.h"
#include "Vector.h"
#include "shape.h"
#include "camera.h"

//自定义组件宏定义序号需要写在这里
#define EMPTY_COMPONENT 0
#define COLLISION_SHAPE 1
#define CHARTEXTURE 2
#define AUDIO 3
#define TIMER 4
#define UITEXT 5
#define PICTEXTURE 6


typedef struct Component
{
	struct componentvTable *vptr;
	char *meta;
	struct Component *next, *prev;

	char *(*getMeta)(struct Component *);
	void (*setMeta)(struct Component *, char *meta);

} Component, *ComponentNode;

typedef void (*ComponentRender)(struct Component *);
typedef void (*ComponentUpdate)(struct Component *, ...);

typedef struct componentvTable
{
	ComponentRender render;
	ComponentUpdate update;
	const int (*getComponentType)();
} componentvTable;

Component *newComponent(ComponentRender render, ComponentUpdate update);
void destoryComponent(Component *c);

#endif