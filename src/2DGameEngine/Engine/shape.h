#ifndef __SHAPE_H__
#define __SHAPE_H__
#include "vector.h"
#include "public_headers.h"
#include "camera.h"

#define EMPTY_SHAPE 0
#define RECT 1
#define CIRCLE 2
#define CONVEXPOLYGON 3

extern double COLLISION_EXPAND_FACTOR;
/*---------------------------Shape Type----------------------------*/
typedef struct Shape {
	
	Vector pos;
	double angle;
	bool fill;
	char* color;
	double density;

	//顶点数组
	Vector** vertices;

	void (*setAngle)(struct Shape*, double);
	double (*getAngle)(struct Shape*);
	void (*setPos)(struct Shape*, Vector*);
	Vector* (*getPos)(struct Shape*);
	bool (*isCollideWith)(struct Shape*, struct Shape*);
	Vector* (*getCollisionVector)(struct Shape*, struct Shape*);

	struct shapevTable* vptr;
} Shape;

/*----------------------Virtual Table Type-------------------------*/
typedef struct shapevTable{
	void (*render)(struct Shape*);
	const int (*getShape)();
} shapevTable;


Shape* newShape(Vector *pos, double angle, bool fill, char* color, double density);
void destoryShape(Shape* s);

/*----------------------------Rect---------------------------------*/
typedef struct Rect {
	Shape super;
	double width, height;
} Rect;

Rect* newRect(Vector *pos, double angle, double width, double height, bool fill, char* color, double density);
void destoryRect(Rect* rt);


/*-----------------------------Circle------------------------------*/
typedef struct Circle {
	Shape super;
	double radius;

} Circle;

Circle* newCircle(Vector *pos, double angle, double radius, bool fill, char* color, double density);
void destoryCircle(Circle* c);


/*-----------------------------ConvexPolygon-----------------------*/

// typedef struct ConvexPolygon {
// 	Shape super;
// 	int vertexNum;
// } ConvexPolygon;

// ConvexPolygon* newConvexPolygon(Vector *pos, double angle, Vector** vertices, int vertexNum, bool fill, char* color, double density);
// void destoryConvexPolygon(ConvexPolygon* cp);


#endif