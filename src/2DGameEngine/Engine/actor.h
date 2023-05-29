#ifndef __ACTOR_H__
#define __ACTOR_H__
#include "vector.h"
#include "component.h"
#include "public_headers.h"

#define JUST_ACTOR 0

typedef struct Actor {

	ComponentNode componentList;

	Vector pos, vel;
	char* meta;
	struct actorvTable* vptr;

	struct Actor* prev, * next;

	void (*setVel)(struct Actor*, Vector*);
	Vector* (*getVel)(struct Actor*);
	void (*setPos)(struct Actor*, Vector*);
	Vector* (*getPos)(struct Actor*);
	char* (*getMeta)(struct Actor*);
	void (*setMeta)(struct Actor*, char* meta);
	void (*addComponent)(struct Actor*, Component*);
	void (*delComponent)(struct Actor*, char* meta);
	ComponentNode (*getComponent)(struct Actor*, char* meta);

	Vector* (*getCollisionVector)(struct Actor*, struct Actor*);
	bool (*isCollideWithActor)(struct Actor*, struct Actor*);

	
} Actor, *ActorNode;

typedef struct actorvTable {
	void (*update)(struct Actor*, double);
	void (*render)(struct Actor*);
	const int (*getActor)();
} actorvTable;

typedef void (*ActorRender)(struct Actor*);

Actor* newActor(char* meta, Vector* pos);
void destoryActor(Actor* actor);

#endif