#ifndef __SCENE_H__
#define __SCENE_H__
#include "public_headers.h"
#include "actor.h"

typedef struct Scene {
	ActorNode actorList;
	char* meta;
	struct Scene* next, * prev; 
	char* switchTarget;

	void (*setMeta)(struct Scene*, char*);
	char* (*getMeta)(struct Scene*);
	void (*addActor)(struct Scene*, ActorNode);
	ActorNode (*getActor)(struct Scene*, char*);
	void (*delActor)(struct Scene*, char* meta);
	void (*setSwitchTarget)(struct Scene*, char*);
	void (*update)(struct Scene*, double);
	void (*render)(struct Scene*);

} Scene, *SceneNode;

Scene* newScene(char* meta);
void destoryScene(Scene* s);

#endif