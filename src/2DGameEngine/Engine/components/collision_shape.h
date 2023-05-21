#ifndef __COLLISION_SHAPE_H__
#define __COLLISION_SHAPE_H__

#include "base_component.h"

typedef struct CollisionShape
{
	Component super;
	Shape *shape;
	Vector pos;
	bool enable;
	bool visible;

	bool (*isCollideWith)(struct CollisionShape *, struct CollisionShape *);
	Vector *(*getCollisionVector)(struct CollisionShape *, struct CollisionShape *);
	void (*setPos)(struct CollisionShape *, Vector *);
	Vector *(*getPos)(struct CollisionShape *);
	char *(*getMeta)(struct Component *);
	void (*setMeta)(struct Component *, char *meta);

} CollisionShape;

CollisionShape *newCollisionShape(Shape *shape);
void destoryCollisionShape(CollisionShape *cs);

#endif