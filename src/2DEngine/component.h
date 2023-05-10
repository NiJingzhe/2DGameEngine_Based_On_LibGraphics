#ifndef __COMPONENT_H__
#define __COMPONENT_H__
#include "public_headers.h"
#include "shape.h"

#define EMPTY_COMPONENT 0
#define COLLISION_SHAPE 1
#define TEXTURE 2

typedef void (*ComponentRender)(struct Component*);
typedef void (*ComponentUpdate)(struct Component*, ...);

/*-------------------------Component------------------------------*/
typedef struct componentvTable {
	ComponentRender render;
	ComponentUpdate update;
	const int(*getComponentType)();
} componentvTable;

typedef struct Component {
	componentvTable *vptr;
	char* meta;
	struct Component *next, *prev;

	char* (*getMeta)(struct Component*);
	void (*setMeta)(struct Component*, char* meta);

} Component, *ComponentNode;

Component* newComponent(ComponentRender render, ComponentUpdate update);
void destoryComponent(Component* c);

/*-------------------------Shape Component------------------------*/
typedef struct CollisionShape {
	Component super;
	Shape* shape;
	Vector pos;
	bool enable;
	bool visible;

	bool (*isCollideWith)(struct CollisionShape*, struct CollisionShape*);
	Vector* (*getCollisionVector)(struct CollisionShape*, struct CollisionShape*);
	void (*setPos)(struct CollisionShape*, Vector*);
	Vector* (*getPos)(struct CollisionShape*);
	char* (*getMeta)(struct Component*);
	void (*setMeta)(struct Component*, char* meta);

} CollisionShape;

CollisionShape* newCollisionShape(Shape* shape);


/*-------------------------Texture Component-----------------------*/

typedef struct Texture {
	Component super;
	Vector pos;
	int lineNumber;
	char** textureString;
	char* resPath;
	char* color;
	int pointSize;
	double width, height;
	bool visible;

	void (*setPos)(struct Texture*, Vector*);
	Vector* (*getPos)(struct Texture*);
	double (*getWidth)(struct Texture*);
	double (*getHeight)(struct Texture*);
	char* (*getMeta)(struct Component*);
	void (*setMeta)(struct Component*, char* meta);

} Texture;

Texture* newTexture(char* resPath, Vector* pos, char* color, int pointSize);


#endif