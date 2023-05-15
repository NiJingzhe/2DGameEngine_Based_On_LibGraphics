#include "scene.h"

static void initScene(Scene* s, char* meta);
static void setSceneMeta(Scene* s, char* meta);
static char* getSceneMeta(Scene* s);
static void addActor(Scene* s, ActorNode actor);
static ActorNode getActor(Scene* s, char* meta);
static void delActor(Scene* s, char* meta);
//static void setSwitchTarget(Scene* s, char* switchTarget);    need to be deleted
static void update(Scene* s, double delta);
static void render(Scene* s);
static void setup(Scene* s, void* param);
static void exit_scene(Scene* s);

Scene* newScene(char* meta){
	Scene* s = (Scene*)calloc(1, sizeof(Scene));
	initScene(s, meta);
	return s;
}

static void initScene(Scene* s, char* meta) {
	s->meta = meta;
	s->actorList = NULL;
	//s->switchTarget = "";
	s->prev = NULL;
	s->next = NULL;

	s->addActor = addActor;
	s->delActor = delActor;
	s->getActor = getActor;
	s->getMeta = getSceneMeta;
	s->setMeta = setSceneMeta;
	//s->setSwitchTarget = setSwitchTarget;
	s->render = render;
	s->setup = setup;
	s->exit = exit_scene;
	s->update = update;
}

static void setSceneMeta(Scene* s, char* meta){
	s->meta = meta;
}

static char* getSceneMeta(Scene* s){
	return s->meta;
}

static void addActor(Scene* s, ActorNode actor){
	ActorNode curActor = s->actorList;
	if (curActor == NULL){
		s->actorList = actor;
		return;
	}

	while(curActor->next){
		curActor = curActor->next;
	}
	curActor->next = actor;
	actor->prev = curActor;
	actor->next = NULL;
	return;
}

static ActorNode getActor(Scene* s, char* meta){
	if (s->actorList == NULL){
		return NULL;
	}
	
	ActorNode curActor = s->actorList;
	while(curActor->next && strcmp(curActor->getMeta(curActor), meta) != 0){
		curActor = curActor->next;
	}	

	if (strcmp(curActor->getMeta(curActor), meta) == 0){
		return curActor;
	}
	else{
		return NULL;
	}
}

static void delActor(Scene* s, char* meta){
	if (s->actorList == NULL){
		return;
	}
	
	ActorNode curActor = s->actorList;
	while(curActor->next && strcmp(curActor->getMeta(curActor), meta) != 0){
		curActor = curActor->next;
	}	

	if (strcmp(curActor->getMeta(curActor), meta) == 0){
		if (curActor->prev != NULL)
			curActor->prev->next = curActor->next;
		if (curActor->next != NULL)
			curActor->next->prev = curActor->prev;
		destoryActor(curActor);
	}
	else{
		return;
	}
}

/*
static void setSwitchTarget(Scene* s, char* switchTarget){
	s->switchTarget = switchTarget;	
	return;
}
*/
static void update(Scene* s, double delta){
	ActorNode curActor = s->actorList;
	if (curActor == NULL){
		//printf("%s scene is updating\n", s->getMeta(s));
		return;
	}

	while(curActor){
		curActor->vptr->update(curActor, delta);
		curActor = curActor->next;
	}
	return;
}

static void render(Scene* s){
	ActorNode curActor = s->actorList;
	if (curActor == NULL){
		//printf("%s scene is updating\n", s->getMeta(s));
		return;
	}

	while(curActor && curActor->vptr != NULL && curActor->vptr->render != NULL){
		curActor->vptr->render(curActor);
		curActor = curActor->next;
	}
	return;
}

static void setup(Scene* s, void* param){
	printf("%s scene default updating\n", s->getMeta(s));
}

static void exit_scene(Scene* s){
	destoryScene(s);
	return;
}


void destoryScene(Scene* s) {
	ActorNode currentActor = s->actorList;
	if (currentActor == NULL) {
		printf("Actor List is empty in scene %s\n", s->getMeta(s));
		free(s);
		s = NULL;
		return;
	}
	while (currentActor->next) {
		currentActor = currentActor->next;
		destoryActor(currentActor->prev);
	}

	destoryActor(currentActor);

	free(s);
	s = NULL;
}