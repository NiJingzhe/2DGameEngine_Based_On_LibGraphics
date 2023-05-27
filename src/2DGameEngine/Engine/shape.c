#include "public_headers.h"
#include "vector.h"
#include "shape.h"

static bool isRectCollide(Rect *rect1, Rect *rect2);
static bool isCircleCollide(Circle *circle1, Circle *circle2);
static bool isRectCircleCollide(Rect *rect_, Circle *circle);

static void initShape(Shape *shape, Vector pos, double angle, bool fill, char *color, double density);
static void setShapeAngle(Shape *s, double angle);
static double getShapeAngle(Shape *s);
static void setShapePos(Shape *s, Vector *pos);
static Vector *getShapePos(Shape *s);
static const int returnEmptyShape();
static bool isCollideWith(Shape *shape1, Shape *shape2);
static Vector *getCollisionVector(Shape *s1, Shape *s2);

static void initRect(Rect *rect, Vector pos, double angle, double width, double height, bool fill, char *color, double density);
static void renderRect(Shape *s);
static const int returnRectType();

static void initCircle(Circle *circle, Vector pos, double angle, double radius, bool fill, char *color, double density);
static void renderCircle(Shape *s);
static const int returnCircleType();

/*-----------------------------------------Private Function---------------------------------------*/
static bool isRectCollide(Rect *rect1, Rect *rect2)
{
	Vector pos1 = rect1->super.pos;
	Vector pos2 = rect2->super.pos;
	Vector *delta_p = newVector(pos1.x - pos2.x, pos1.y - pos2.y);

	double angle1 = rect1->super.angle;
	double angle2 = rect2->super.angle;

	Vector vertex[4] = {0};
	memcpy(&vertex[0], rect1->super.vertices[0], sizeof(Vector));
	memcpy(&vertex[1], rect1->super.vertices[1], sizeof(Vector));
	memcpy(&vertex[2], rect1->super.vertices[2], sizeof(Vector));
	memcpy(&vertex[3], rect1->super.vertices[3], sizeof(Vector));

	delta_p->rotate(delta_p, -angle2);
	for (int i = 0; i < 4; ++i)
	{
		vertex[i].rotate(&vertex[i], angle1 - angle2);
		vertex[i].mult(&vertex[1], 1.01);
		vertex[i].add(&vertex[i], delta_p);
	}

	double rect1X_min = vertex[0].x, rect1X_max = vertex[0].x;
	double rect1Y_min = vertex[0].y, rect1Y_max = vertex[0].y;
	for (int i = 1; i < 4; ++i)
	{
		rect1X_min = rect1X_min >= vertex[i].x ? vertex[i].x : rect1X_min;
		rect1Y_min = rect1Y_min >= vertex[i].y ? vertex[i].y : rect1Y_min;
		rect1X_max = rect1X_max <= vertex[i].x ? vertex[i].x : rect1X_max;
		rect1Y_max = rect1Y_max <= vertex[i].y ? vertex[i].y : rect1Y_max;
	}

	double rect2X_min = -rect2->width / 2 - 0.1;
	double rect2X_max = -rect2X_min;
	double rect2Y_min = -rect2->height / 2 - 0.1;
	double rect2Y_max = -rect2Y_min;

	double deltaXTotal = fmax(rect2X_max, rect1X_max) - fmin(rect2X_min, rect1X_min);
	double deltaYTotal = fmax(rect2Y_max, rect1Y_max) - fmin(rect2Y_min, rect1Y_min);
	double rect2DeltaX = rect2X_max - rect2X_min;
	double rect2DeltaY = rect2Y_max - rect2Y_min;
	double rect1DeltaX = rect1X_max - rect1X_min;
	double rect1DeltaY = rect1Y_max - rect1Y_min;

	destoryVector(delta_p);

	if (rect1DeltaX + rect2DeltaX >= deltaXTotal && rect1DeltaY + rect2DeltaY >= deltaYTotal)
		return TRUE;
	else
		return FALSE;
}

static bool isCircleCollide(Circle *circle1, Circle *circle2)
{
	double dx = circle1->super.pos.x - circle2->super.pos.x;
	double dy = circle1->super.pos.y - circle2->super.pos.y;
	double distance = sqrt(dx * dx + dy * dy);
	double radiusSum = circle1->radius + circle2->radius;
	return distance <= radiusSum;
}

static bool isRectCircleCollide(Rect *rect_, Circle *circle)
{

	Rect rect_copy = *rect_;
	Rect *rect = &rect_copy;

	rect->super.pos.sub(&(rect->super.pos), &(circle->super.pos));
	rect->super.pos.rotate(&(rect->super.pos), -(rect->super.getAngle(&(rect->super))));
	rect->super.pos.add(&(rect->super.pos), &(circle->super.pos));

	double circleDistanceX = fabs(circle->super.pos.x - rect->super.pos.x);
	double circleDistanceY = fabs(circle->super.pos.y - rect->super.pos.y);

	if (circleDistanceX > (rect->width / 2 + circle->radius))
	{
		return FALSE;
	}
	if (circleDistanceY > (rect->height / 2 + circle->radius))
	{
		return FALSE;
	}

	if (circleDistanceX <= (rect->width / 2))
	{
		return TRUE;
	}
	if (circleDistanceY <= (rect->height / 2))
	{
		return TRUE;
	}

	double cornerDistance_sq = (circleDistanceX - rect->width / 2) * (circleDistanceX - rect->width / 2) +
							   (circleDistanceY - rect->height / 2) * (circleDistanceY - rect->height / 2);

	return (cornerDistance_sq <= (circle->radius * circle->radius));
}

bool isIntersect(Vector a, Vector b, Vector c, Vector d){
	double u, v, w, z;
	u = (c.x - a.x) * (b.y - a.y) - (b.x - a.x) * (c.y - a.y);
	v = (d.x - a.x) * (b.y - a.y) - (b.x - a.x) * (d.y - a.y);
	w = (a.x - c.x) * (d.y - c.y) - (d.x - c.x) * (a.y - c.y);
	z = (b.x - c.x) * (d.y - c.y) - (d.x - c.x) * (b.y - c.y);
	return (u * v <= 0.002 && w * z <= 0.002);
}

Vector *getCircleCollsionVector(Circle *c1, Circle *c2)
{
	Vector *v = newVector(c1->super.pos.x - c2->super.pos.x, c1->super.pos.y - c2->super.pos.y);
	v->normalize(v);
	return v;
}

Vector *getRectCollisionVector(Rect *rect1, Rect *rect2)
{
	// 获取两个rect的pos
	Vector pos1 = rect1->super.pos;
	Vector pos2 = rect2->super.pos;

	double halfDiagonalLengthofRect2;
	halfDiagonalLengthofRect2 = sqrt(rect2->width * rect2->width + rect2->height * rect2->height) / 2;

	int i;
	Vector rect1Vertex_[4] = {0};
	double minDist = 9999;
	Vector minDistVertex;
	bool surfaceContact = FALSE;
	for (i = 0; i < 4; ++i)
	{	
		memcpy(&rect1Vertex_[i], rect1->super.vertices[i], sizeof(Vector));
		rect1Vertex_[i].rotate(&rect1Vertex_[i], rect1->super.angle);
	    rect1Vertex_[i].add(&rect1Vertex_[i], &(rect1->super.pos));
		double dist = sqrt((rect1Vertex_[i].x - pos2.x) * (rect1Vertex_[i].x - pos2.x) + (rect1Vertex_[i].y - pos2.y) * (rect1Vertex_[i].y - pos2.y));
		if (dist < minDist)
		{
		 	minDist = dist;
		 	minDistVertex = rect1Vertex_[i];
		}
		if (halfDiagonalLengthofRect2 > dist)
		{
			surfaceContact = TRUE;
		}
	}

	Vector *v = NULL;

	if (surfaceContact)
	{
		// printf("\nLOG:\nrect2 side");
		//  在rect2坐标系下判断rect1UnderRect2与rect2的四条边是否相交
		Vector rect2Vertex[4] = {0};
		memcpy(&rect2Vertex[0], rect2->super.vertices[0], sizeof(Vector));
		memcpy(&rect2Vertex[1], rect2->super.vertices[1], sizeof(Vector));
		memcpy(&rect2Vertex[2], rect2->super.vertices[2], sizeof(Vector));
		memcpy(&rect2Vertex[3], rect2->super.vertices[3], sizeof(Vector));

		// 将顶点坐标从rect2坐标系变换到世界坐标系
		for (int i = 0; i < 4; ++i)
		{
			rect2Vertex[i].rotate(&rect2Vertex[i], rect2->super.angle);
			rect2Vertex[i].add(&rect2Vertex[i], &(rect2->super.pos));
		}
		// 判断以pos1和pos2为起点终点的有向线段和rect2的四条边是否相交，是则计算边向量的法向量，并返回
		if (isIntersect(minDistVertex, pos2, rect2Vertex[0], rect2Vertex[1]))
		{
			v = newVector(rect2Vertex[1].y - rect2Vertex[0].y, rect2Vertex[0].x - rect2Vertex[1].x);
		}
		else if (isIntersect(minDistVertex, pos2, rect2Vertex[1], rect2Vertex[2]))
		{
			v = newVector(rect2Vertex[2].y - rect2Vertex[1].y, rect2Vertex[1].x - rect2Vertex[2].x);
		}
		else if (isIntersect(minDistVertex, pos2, rect2Vertex[2], rect2Vertex[3]))
		{
			v = newVector(rect2Vertex[3].y - rect2Vertex[2].y, rect2Vertex[2].x - rect2Vertex[3].x);
		}
		else if (isIntersect(minDistVertex, pos2, rect2Vertex[3], rect2Vertex[0]))
		{
			v = newVector(rect2Vertex[0].y - rect2Vertex[3].y, rect2Vertex[3].x - rect2Vertex[0].x);
		}
	}
	else
	{
		// 重复以上步骤，但是对rect1操作
		// printf("\nLOG:\nrect1 side");
		Vector rect1Vertex[4] = {0};
		memcpy(&rect1Vertex[0], rect1->super.vertices[0], sizeof(Vector));
		memcpy(&rect1Vertex[1], rect1->super.vertices[1], sizeof(Vector));
		memcpy(&rect1Vertex[2], rect1->super.vertices[2], sizeof(Vector));
		memcpy(&rect1Vertex[3], rect1->super.vertices[3], sizeof(Vector));

		for (int i = 0; i < 4; ++i)
		{
			rect1Vertex[i].rotate(&rect1Vertex[i], rect1->super.angle);
			rect1Vertex[i].add(&rect1Vertex[i], &(rect1->super.pos));
		}

		if (isIntersect(pos1, pos2, rect1Vertex[0], rect1Vertex[1]))
		{
			v = newVector(rect1Vertex[1].y - rect1Vertex[0].y, rect1Vertex[0].x - rect1Vertex[1].x);
		}
		else if (isIntersect(pos1, pos2, rect1Vertex[1], rect1Vertex[2]))
		{
			v = newVector(rect1Vertex[2].y - rect1Vertex[1].y, rect1Vertex[1].x - rect1Vertex[2].x);
		}
		else if (isIntersect(pos1, pos2, rect1Vertex[2], rect1Vertex[3]))
		{
			v = newVector(rect1Vertex[3].y - rect1Vertex[2].y, rect1Vertex[2].x - rect1Vertex[3].x);
		}
		else if (isIntersect(pos1, pos2, rect1Vertex[3], rect1Vertex[0]))
		{
			v = newVector(rect1Vertex[0].y - rect1Vertex[3].y, rect1Vertex[3].x - rect1Vertex[0].x);
		}
	}

	if (v != NULL)
	{
		v->normalize(v);
		if (0 > (v->x * (pos1.x - pos2.x) + v->y * (pos1.y - pos2.y)))
		{
			v->mult(v, -1);
		}
	}

	return v;
}

Vector *getRectCircleCollisionVector(Rect *r, Circle *c)
{
	Vector *v = newVector(r->super.pos.x - c->super.pos.x, r->super.pos.y - c->super.pos.y);
	v->normalize(v);
	return v;
}

/*--------------------------------------Public Function------------------------------------------*/
Shape *newShape(Vector *pos, double angle, bool fill, char *color, double density)
{
	Shape *s = (Shape *)calloc(1, sizeof(Shape));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	initShape(s, *pos, angle, fill, color, density);
	return s;
}

static void initShape(Shape *shape, Vector pos, double angle, bool fill, char *color, double density)
{
	shape->pos = pos;
	shape->fill = fill;
	shape->color = color;
	shape->density = density;
	shape->angle = angle;

	shape->getAngle = getShapeAngle;
	shape->getPos = getShapePos;
	shape->setAngle = setShapeAngle;
	shape->setPos = setShapePos;
	shape->getCollisionVector = getCollisionVector;
	shape->isCollideWith = isCollideWith;
	shape->vertices = NULL;

	shape->vptr = (shapevTable *)calloc(1, sizeof(shapevTable));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	shape->vptr->getShape = returnEmptyShape;
	shape->vptr->render = NULL;
}

static void setShapeAngle(Shape *s, double angle)
{
	s->angle = angle;
}

static double getShapeAngle(Shape *s)
{
	return s->angle;
}

static void setShapePos(Shape *s, Vector *pos)
{
	s->pos = *pos;
}

static Vector *getShapePos(Shape *s)
{
	return &(s->pos);
}

static bool isCollideWith(Shape *shape1, Shape *shape2)
{
	int shapeType1 = shape1->vptr->getShape();
	int shapeType2 = shape2->vptr->getShape();

	if (shapeType1 == RECT && shapeType2 == RECT)
	{
		Rect *rect1 = (Rect *)shape1;
		Rect *rect2 = (Rect *)shape2;
		return isRectCollide(rect1, rect2) && isRectCollide(rect2, rect1);
	}
	else if (shapeType1 == CIRCLE && shapeType2 == CIRCLE)
	{
		Circle *circle1 = (Circle *)shape1;
		Circle *circle2 = (Circle *)shape2;
		return isCircleCollide(circle1, circle2);
	}
	else if (shapeType1 == RECT && shapeType2 == CIRCLE)
	{
		Rect *rect = (Rect *)shape1;
		Circle *circle = (Circle *)shape2;
		return isRectCircleCollide(rect, circle);
	}
	else if (shapeType1 == CIRCLE && shapeType2 == RECT)
	{
		Rect *rect = (Rect *)shape2;
		Circle *circle = (Circle *)shape1;
		return isRectCircleCollide(rect, circle);
	}
	else
	{
		return FALSE;
	}
}

static Vector *getCollisionVector(Shape *s1, Shape *s2)
{
	int shapeType1 = s1->vptr->getShape();
	int shapeType2 = s2->vptr->getShape();

	if (shapeType1 == RECT && shapeType2 == RECT)
	{
		Rect *rect1 = (Rect *)s1;
		Rect *rect2 = (Rect *)s2;
		return getRectCollisionVector(rect1, rect2);
	}
	else if (shapeType1 == CIRCLE && shapeType2 == CIRCLE)
	{
		Circle *circle1 = (Circle *)s1;
		Circle *circle2 = (Circle *)s2;
		return getCircleCollsionVector(circle1, circle2);
	}
	else if (shapeType1 == RECT && shapeType2 == CIRCLE)
	{
		Rect *rect = (Rect *)s1;
		Circle *circle = (Circle *)s2;
		return getRectCircleCollisionVector(rect, circle);
	}
	else if (shapeType1 == CIRCLE && shapeType2 == RECT)
	{
		Rect *rect = (Rect *)s2;
		Circle *circle = (Circle *)s1;
		return getRectCircleCollisionVector(rect, circle);
	}
	else
	{
		return NULL;
	}
}

static const int returnEmptyShape()
{
	return EMPTY_SHAPE;
}

void destoryShape(Shape *s)
{
	if (s->vptr->getShape() == RECT)
	{
		destoryRect((Rect *)s);
		return;
	}
	else if (s->vptr->getShape() == CIRCLE)
	{
		destoryCircle((Circle *)s);
		return;
	}
	else
	{
		free(s->vptr);
#if MEM_DEBUG
		MEM_BLOCK_NUM--;
		printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
		free(s);
#if MEM_DEBUG
		MEM_BLOCK_NUM--;
		printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
		s = NULL;
		return;
	}
}

Rect *newRect(Vector *pos, double angle, double width, double height, bool fill, char *color, double density)
{
	Rect *rt = (Rect *)calloc(1, sizeof(Rect));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	initRect(rt, *pos, angle, width, height, fill, color, density);
	return rt;
}

static void initRect(Rect *rect, Vector pos, double angle, double width, double height,
					 bool fill, char *color, double density)
{

	Shape *shapeptr = newShape(&pos, angle, fill, color, density);
	memcpy(&(rect->super), shapeptr, sizeof(Shape));
	rect->super.vptr = NULL;
	rect->super.vptr = (shapevTable *)calloc(1, sizeof(shapevTable));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	if (rect->super.vptr == NULL)
	{
		printf("Cannot allocate memory for vptr of rect shape\n");
		return;
	}
	rect->super.vptr->render = renderRect;
	rect->super.vptr->getShape = returnRectType;
	rect->super.vertices = (Vector **)calloc(4, sizeof(Vector *));
#if MEM_DEBUG
	MEM_BLOCK_NUM += 4;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	if (rect->super.vertices == NULL)
	{
		printf("Cannot allocate memory for vertices of rect shape\n");
		return;
	}
	rect->super.vertices[0] = newVector(-width / 2, height / 2);
	rect->super.vertices[1] = newVector(width / 2, height / 2);
	rect->super.vertices[2] = newVector(width / 2, -height / 2);
	rect->super.vertices[3] = newVector(-width / 2, -height / 2);

	destoryShape(shapeptr);

	rect->width = width;
	rect->height = height;
}

static void renderRect(Shape *s)
{

	SetEraseMode(FALSE);

	Rect *rect = (Rect *)s;

	Vector vertex[4] = {0};
	memcpy(&vertex[0], rect->super.vertices[0], sizeof(Vector));
	memcpy(&vertex[1], rect->super.vertices[1], sizeof(Vector));
	memcpy(&vertex[2], rect->super.vertices[2], sizeof(Vector));
	memcpy(&vertex[3], rect->super.vertices[3], sizeof(Vector));

	for (int i = 0; i < 4; ++i)
	{
		vertex[i].rotate(&vertex[i], rect->super.angle);
		vertex[i].add(&vertex[i], &(rect->super.pos));
	}

	SetPenColor(rect->super.color);
	SetPenSize(1);
	MovePen(vertex[0].x, vertex[0].y);
	if (rect->super.fill)
		fill(rect->super.density);

	DrawLine(vertex[1].x - vertex[0].x, vertex[1].y - vertex[0].y);
	DrawLine(vertex[2].x - vertex[1].x, vertex[2].y - vertex[1].y);
	DrawLine(vertex[3].x - vertex[2].x, vertex[3].y - vertex[2].y);
	DrawLine(vertex[0].x - vertex[3].x, vertex[0].y - vertex[3].y);

	if (rect->super.fill)
		endfill;
}

static const int returnRectType()
{
	return RECT;
}

void destoryRect(Rect *rt)
{
	free(rt->super.vptr);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif

	for (int i = 0; i < 4; ++i)
	{
		destoryVector(rt->super.vertices[i]);
		rt->super.vertices[i] = NULL;
	}

	free(rt->super.vertices);
#if MEM_DEBUG
	MEM_BLOCK_NUM -= 4;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif

	free(rt);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	rt = NULL;
}

Circle *newCircle(Vector *pos, double angle, double radius, bool fill, char *color, double density)
{
	Circle *c = (Circle *)calloc(1, sizeof(Circle));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	initCircle(c, *pos, angle, radius, fill, color, density);
	return c;
}

static void initCircle(Circle *circle, Vector pos, double angle, double radius, bool fill, char *color, double density)
{
	Shape *shapeptr = newShape(&pos, angle, fill, color, density);
	memcpy(&(circle->super), shapeptr, sizeof(Shape));
	circle->super.vptr = NULL;
	circle->super.vptr = (shapevTable *)calloc(1, sizeof(shapevTable));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	if (circle->super.vptr == NULL)
	{
		printf("Cannot allocate memory for vptr of circle shape\n");
		return;
	}
	circle->super.vptr->render = renderCircle;
	circle->super.vptr->getShape = returnCircleType;
	destoryShape(shapeptr);
	circle->radius = radius;
}

static void renderCircle(Shape *s)
{

	SetEraseMode(FALSE);
	Circle *circle = (Circle *)s;
	MovePen(circle->super.pos.x + circle->radius, circle->super.pos.y);
	SetPenColor(circle->super.color);
	SetPenSize(1);
	if (circle->super.fill)
		fill(circle->super.density);

	DrawArc(circle->radius, 0, 360);

	if (circle->super.fill)
		endfill;
}

static const int returnCircleType()
{
	return CIRCLE;
}

void destoryCircle(Circle *c)
{
	// destoryShape(&(c->super));
	free(c->super.vptr);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	free(c);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	c = NULL;
}
