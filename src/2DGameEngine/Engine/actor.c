#include "public_headers.h"
#include "actor.h"

static void initActor(Actor *actor, char *meta, Vector pos);
static void addComponent(Actor *actor, ComponentNode comp);
static void delComponent(Actor *actor, char *meta);
static ComponentNode getComponent(Actor *actor, char *meta);
static void setActorVel(Actor *actor, Vector *vel);
static void setActorPos(Actor *actor, Vector *pos);
static Vector *getActorVel(Actor *actor);
static Vector *getActorPos(Actor *actor);
static void setActorMeta(Actor *actor, char *meta);
static char *getActorMeta(Actor *actor);
static void renderActor(Actor *actor);
static const int returnActorType();
static void updateActor(Actor *actor, double delta);
static bool isCollideWithActor(Actor *a1, Actor *a2);
static Vector* getCollisionVector(Actor *a1, Actor *a2);

Actor *newActor(char *meta, Vector *pos)
{
	Actor *actor = (Actor *)calloc(1, sizeof(Actor));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	initActor(actor, meta, *pos);
	return actor;
}

static void initActor(Actor *actor, char *meta, Vector pos)
{
	actor->meta = meta;
	actor->pos = pos;
	Vector *vel = newVector(0, 0);
	actor->vel = *vel;
	destoryVector(vel);
	actor->vptr = (actorvTable *)calloc(1, sizeof(actorvTable));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	if (actor->vptr == NULL)
	{
		printf("Cannot allocate memory for vptr of Actor\n");
		return;
	}
	actor->vptr->getActor = returnActorType;

	actor->componentList = NULL;
	actor->prev = NULL;
	actor->next = NULL;

	actor->setVel = setActorVel;
	actor->getVel = getActorVel;
	actor->setPos = setActorPos;
	actor->getPos = getActorPos;
	actor->setMeta = setActorMeta;
	actor->getMeta = getActorMeta;
	actor->addComponent = addComponent;
	actor->delComponent = delComponent;
	actor->getComponent = getComponent;
	actor->isCollideWithActor = isCollideWithActor;
	actor->getCollisionVector = getCollisionVector;
	actor->vptr->render = renderActor;
	actor->vptr->update = updateActor;
}

static void setActorVel(Actor *actor, Vector *vel)
{
	actor->vel = *vel;
}

static void setActorPos(Actor *actor, Vector *pos)
{
	actor->pos = *pos;
}

static Vector *getActorVel(Actor *actor)
{
	return &(actor->vel);
}

static Vector *getActorPos(Actor *actor)
{
	return &(actor->pos);
}

static void setActorMeta(Actor *actor, char *meta)
{
	actor->meta = meta;
}

static char *getActorMeta(Actor *actor)
{
	return actor->meta;
}

static void renderActor(Actor *actor)
{
	ComponentNode currentComp = actor->componentList;
	while (currentComp && currentComp->vptr != NULL && currentComp->vptr->render != 0)
	{
		currentComp->vptr->render(currentComp);
		currentComp = currentComp->next;
	}
}

const int returnActorType()
{
	return JUST_ACTOR;
}

void updateActor(Actor *actor, double delta)
{	
	return;
}

static void addComponent(Actor *actor, ComponentNode comp)
{
	ComponentNode currentComp = actor->componentList;
	// Component List is still empty, then we directly set head as comp(the new node)
	if (currentComp == NULL)
	{
		actor->componentList = comp;
		return;
	}
	// or we already have something in the ComponentList, then we have to find the "TAIL" node and add
	// new Component node after it.
	while (currentComp->next)
	{
		currentComp = currentComp->next;
	}
	// Don' t forget to set the prev pointer of the new node
	comp->prev = currentComp;
	currentComp->next = comp;
}

static void delComponent(Actor *actor, char *meta)
{
	ComponentNode currentComp = actor->componentList;
	// if the componentList is empty, we should not delet any node, which means we should directly exit this funciton
	if (currentComp == NULL)
	{
		return;
	}
	// Or we have something, then we have to find the correct node with compType and meta
	while (currentComp->next && strcmp(currentComp->getMeta(currentComp), meta) == 0)
	{
		currentComp = currentComp->next;
	}
	// Reach the tail node but still not found, directly exit this function.
	if (currentComp->next == NULL && strcmp(currentComp->getMeta(currentComp), meta) != 0)
	{
		return;
	}
	else
	{ // We've found one node that exactly we need to delete.
		if (currentComp->next != NULL)
			currentComp->next->prev = currentComp->prev; //  prev <- cur <- next     prev <- next

		if (currentComp->prev != NULL)					 //                              \_cur
			currentComp->prev->next = currentComp->next; // prev -> cur -> next      prev -> next
														 //                              cur_/
		destoryComponent(currentComp);					 // prev -> next
		return;
	}
}

static ComponentNode getComponent(Actor *actor, char *meta)
{
	ComponentNode currentComp = actor->componentList;
	// if the componentList is empty, we should not delet any node, which means we should directly exit this funciton
	if (currentComp == NULL)
	{
		return NULL;
	}
	// Or we have something, then we have to find the correct node with compType and meta
	while (currentComp->next && strcmp(currentComp->getMeta(currentComp), meta) != 0)
	{
		currentComp = currentComp->next;
	}
	// Reach the tail node but still not found, directly exit this function.
	if (currentComp->next == NULL && strcmp(currentComp->getMeta(currentComp), meta) != 0)
	{
		return NULL;
	}
	else
	{ // We've found one node that exactly we need to delete.
		return currentComp;
	}
}

static Vector* getCollisionVector(Actor *a1, Actor *a2)
{

	ComponentNode a1CurrentComp = a1->componentList, a2CurrentComp = a2->componentList;
	Vector *collisionVec = NULL;

	while (a1CurrentComp)
	{
		a2CurrentComp = a2->componentList;
		while (a2CurrentComp)
		{
			if (a1CurrentComp->vptr->getComponentType() == COLLISION_SHAPE &&
				a2CurrentComp->vptr->getComponentType() == COLLISION_SHAPE)
			{

				if (((CollisionShape *)a1CurrentComp)->isCollideWith(((CollisionShape *)a1CurrentComp), ((CollisionShape *)a2CurrentComp)))
				{
					//Vector* tempColVec = 
					if (collisionVec == NULL)
						collisionVec = ((CollisionShape *)a1CurrentComp)->getCollisionVector(((CollisionShape *)a1CurrentComp), ((CollisionShape *)a2CurrentComp));
					else{
						collisionVec->add(collisionVec, ((CollisionShape *)a1CurrentComp)->getCollisionVector(((CollisionShape *)a1CurrentComp), ((CollisionShape *)a2CurrentComp)));
						collisionVec->normalize(collisionVec);
					}
				}
			}
			a2CurrentComp = a2CurrentComp->next;
		}
		a1CurrentComp = a1CurrentComp->next;
	}
	return collisionVec;
}

static bool isCollideWithActor(Actor *a1, Actor *a2)
{

	ComponentNode a1CurrentComp = a1->componentList, a2CurrentComp = a2->componentList;
	bool collisionResult = FALSE;

	while (a1CurrentComp)
	{
		a2CurrentComp = a2->componentList;
		while (a2CurrentComp)
		{
			if (a1CurrentComp->vptr->getComponentType() == COLLISION_SHAPE &&
				a2CurrentComp->vptr->getComponentType() == COLLISION_SHAPE)
			{

				collisionResult |= ((CollisionShape *)a1CurrentComp)->isCollideWith(((CollisionShape *)a1CurrentComp), ((CollisionShape *)a2CurrentComp));
			}
			a2CurrentComp = a2CurrentComp->next;
		}
		a1CurrentComp = a1CurrentComp->next;
	}
	return collisionResult;
}

void destoryActor(Actor *actor)
{
	#if MEM_DEBUG
	printf("\nLOG:\n	Enter destoryActor, destorying actor: %s\n", actor->meta);
	#endif
	ComponentNode currentComponent = actor->componentList;
	if (currentComponent == NULL) {
		return;
	}
	while (currentComponent->next)
	{
		currentComponent = currentComponent->next;
		destoryComponent(currentComponent->prev);
	}
	destoryComponent(currentComponent);

	free(actor->vptr);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	free(actor);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	actor = NULL;
}