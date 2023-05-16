#ifndef __COMPONENT_H__
#define __COMPONENT_H__
#include "public_headers.h"
#include "shape.h"

#define EMPTY_COMPONENT 0
#define COLLISION_SHAPE 1
#define TEXTURE 2
#define AUDIO 3

/*-------------------------Component------------------------------*/
typedef struct Component {
	struct componentvTable *vptr;
	char* meta;
	struct Component *next, *prev;

	char* (*getMeta)(struct Component*);
	void (*setMeta)(struct Component*, char* meta);

} Component, *ComponentNode;

typedef void (*ComponentRender)(struct Component*);
typedef void (*ComponentUpdate)(struct Component*,  ...);

typedef struct componentvTable {
	ComponentRender render;
	ComponentUpdate update;
	const int(*getComponentType)();
} componentvTable;

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

//written by GPT
typedef struct Audio {
    Component super;
    char* filePath;
    bool playing;
    bool loop;
    int volume;
    
    void (*play)(struct Audio*);
    void (*pause)(struct Audio*);
    void (*stop)(struct Audio*);
    void (*setVolume)(struct Audio*, int);
    char* (*getFilePath)(struct Audio*);
    char* (*getMeta)(struct Component*);
    void (*setMeta)(struct Component*, char* meta);
} Audio;

Audio* newAudio(char* filePath, bool loop);

#endif