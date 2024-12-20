#include "public_headers.h"
#include "vector.h"
#include "shape.h"

double COLLISION_EXPAND_FACTOR = 1.01;

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
    // 获取两个矩形的顶点
    Vector vertices1[4], vertices2[4];
    for(int i = 0; i < 4; i++) {
        vertices1[i] = *rect1->super.vertices[i];
        vertices1[i].rotate(&vertices1[i], rect1->super.angle);
        vertices1[i].add(&vertices1[i], &rect1->super.pos);
        
        vertices2[i] = *rect2->super.vertices[i];
        vertices2[i].rotate(&vertices2[i], rect2->super.angle);
        vertices2[i].add(&vertices2[i], &rect2->super.pos);
    }

    // 获取两个矩形的边的方向向量（标准化）
    Vector axes[4];
    axes[0] = *rect1->super.vertices[1];
    axes[0].sub(&axes[0], rect1->super.vertices[0]);
    axes[1] = *rect1->super.vertices[3];
    axes[1].sub(&axes[1], rect1->super.vertices[0]);
    axes[2] = *rect2->super.vertices[1];
    axes[2].sub(&axes[2], rect2->super.vertices[0]);
    axes[3] = *rect2->super.vertices[3];
    axes[3].sub(&axes[3], rect2->super.vertices[0]);

    for(int i = 0; i < 4; i++) {
        axes[i].normalize(&axes[i]);
    }

    // 在每个轴上进行投影检测
    for(int i = 0; i < 4; i++) {
        double min1 = 999999, max1 = -999999;
        double min2 = 999999, max2 = -999999;
        
        // 计算rect1的投影
        for(int j = 0; j < 4; j++) {
            double proj = vertices1[j].x * axes[i].x + vertices1[j].y * axes[i].y;
            min1 = fmin(min1, proj);
            max1 = fmax(max1, proj);
        }
        
        // 计算rect2的投影
        for(int j = 0; j < 4; j++) {
            double proj = vertices2[j].x * axes[i].x + vertices2[j].y * axes[i].y;
            min2 = fmin(min2, proj);
            max2 = fmax(max2, proj);
        }
        
        // 检查投影是否重叠
        if(min1 > max2 || min2 > max1) {
            return FALSE;
        }
    }
    
    return TRUE;
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

bool isIntersect(Vector a, Vector b, Vector c, Vector d)
{
	double u, v, w, z;
	u = (c.x - a.x) * (b.y - a.y) - (b.x - a.x) * (c.y - a.y);
	v = (d.x - a.x) * (b.y - a.y) - (b.x - a.x) * (d.y - a.y);
	w = (a.x - c.x) * (d.y - c.y) - (d.x - c.x) * (a.y - c.y);
	z = (b.x - c.x) * (d.y - c.y) - (d.x - c.x) * (b.y - c.y);
	return (u * v <= 0.002 && w * z <= 0.002);
}

double pointToSegementDist(Vector point, Vector s1, Vector s2)
{
	double delta_y = s2.y - s1.y;
	double delta_x = s2.x - s1.x;
	double k, b;
	if (delta_x != 0)
	{
		k = delta_y / delta_x;
		b = s1.y - k * s1.x;
		double dist = fabs(k * point.x - point.y + b) / sqrt(k * k + 1);
		return dist;
	}
	else
	{
		return fabs(point.x - s1.x);
	}
}

/*
 * 获取圆形之间的碰撞向量
 * 从c2指向c1的单位向量，即两圆心连线的方向
 */
Vector *getCircleCollsionVector(Circle *c1, Circle *c2)
{
    // 计算从c2指向c1的向量
    Vector *v = newVector(c1->super.pos.x - c2->super.pos.x, 
                         c1->super.pos.y - c2->super.pos.y);
    v->normalize(v);  // 标准化为单位向量
    return v;
}

/*
 * 获取矩形之间的碰撞向量
 * 基于分离轴定理(SAT)实现
 * 返回最小重叠轴的方向向量(从rect2指向rect1)
 */
static Vector *getRectCollisionVector(Rect *rect1, Rect *rect2)
{
    // 计算世界坐标系中的顶点位置
    Vector vertices1[4], vertices2[4];
    for(int i = 0; i < 4; i++) {
        // 获取局部坐标系中的顶点
        vertices1[i] = *rect1->super.vertices[i];
        vertices2[i] = *rect2->super.vertices[i];
        
        // 应用旋转变换
        vertices1[i].rotate(&vertices1[i], rect1->super.angle);
        vertices2[i].rotate(&vertices2[i], rect2->super.angle);
        
        // 应用平移变换到世界坐标系
        vertices1[i].add(&vertices1[i], &rect1->super.pos);
        vertices2[i].add(&vertices2[i], &rect2->super.pos);
    }

    // 计算用于投影的轴向量
    // 每个矩形贡献两个轴向量(相邻边的法向量)
    Vector axes[4];
    // rect1的两个轴
    axes[0] = *rect1->super.vertices[1];
    axes[0].sub(&axes[0], rect1->super.vertices[0]);  // 上边
    axes[1] = *rect1->super.vertices[3];
    axes[1].sub(&axes[1], rect1->super.vertices[0]);  // 左边
    // rect2的两个轴
    axes[2] = *rect2->super.vertices[1];
    axes[2].sub(&axes[2], rect2->super.vertices[0]);  // 上边
    axes[3] = *rect2->super.vertices[3];
    axes[3].sub(&axes[3], rect2->super.vertices[0]);  // 左边

    double minOverlap = 999999;  // 记录最小重叠量
    Vector *minAxis = NULL;      // 记录最小重叠量对应的轴向量
    
    // 对每个轴进行投影测试
    for(int i = 0; i < 4; i++) {
        axes[i].normalize(&axes[i]);  // 标准化轴向量
        
        double min1 = 999999, max1 = -999999;  // rect1的投影范围
        double min2 = 999999, max2 = -999999;  // rect2的投影范围
        
        // 计算两个矩形在当前轴上的投影范围
        for(int j = 0; j < 4; j++) {
            // 计算顶点在轴上的投影长度(点积)
            double proj1 = vertices1[j].x * axes[i].x + vertices1[j].y * axes[i].y;
            double proj2 = vertices2[j].x * axes[i].x + vertices2[j].y * axes[i].y;
            // 更新投影范围
            min1 = fmin(min1, proj1);
            max1 = fmax(max1, proj1);
            min2 = fmin(min2, proj2);
            max2 = fmax(max2, proj2);
        }
        
        // 计算当前轴上的重叠量
        double overlap = fmin(max1, max2) - fmax(min1, min2);
        // 如果找到更小的重叠量，更新记录
        if(overlap < minOverlap) {
            minOverlap = overlap;
            minAxis = newVector(axes[i].x, axes[i].y);
        }
    }

    // 确保碰撞向量方向正确（从rect2指向rect1）
    if(minAxis != NULL) {
        Vector center_diff;  // 两矩形中心点之差
        center_diff.x = rect1->super.pos.x - rect2->super.pos.x;
        center_diff.y = rect1->super.pos.y - rect2->super.pos.y;
        // 如果轴向量与中心差向量方向相反，翻转轴向量
        if(center_diff.x * minAxis->x + center_diff.y * minAxis->y < 0) {
            minAxis->mult(minAxis, -1);
        }
    }
    
    return minAxis;
}

/*
 * 获取矩形和圆形之间的碰撞向量
 * 从圆心指向矩形中心的单位向量
 */
Vector *getRectCircleCollisionVector(Rect *r, Circle *c)
{
    // 计算从圆心指向矩形中心的向量
    Vector *v = newVector(r->super.pos.x - c->super.pos.x, 
                         r->super.pos.y - c->super.pos.y);
    v->normalize(v);  // 标准化为单位向量
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
	rect->super.vertices[0]->rotate(rect->super.vertices[0], angle);
	rect->super.vertices[1] = newVector(width / 2, height / 2);
	rect->super.vertices[1]->rotate(rect->super.vertices[1], angle);
	rect->super.vertices[2] = newVector(width / 2, -height / 2);
	rect->super.vertices[2]->rotate(rect->super.vertices[2], angle);
	rect->super.vertices[3] = newVector(-width / 2, -height / 2);
	rect->super.vertices[3]->rotate(rect->super.vertices[3], angle);

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
		vertex[i].x = (vertex[i].x - globalCamera.position.x) * globalCamera.zoom + getww / 2;
		vertex[i].y = (vertex[i].y - globalCamera.position.y) * globalCamera.zoom + getwh / 2;
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
	Vector *renderPos = newVector(0, 0);
	renderPos->x = (circle->super.pos.x - globalCamera.position.x) * globalCamera.zoom + getww / 2;
	renderPos->y = (circle->super.pos.y - globalCamera.position.y) * globalCamera.zoom + getwh / 2;
	double renderRadius = circle->radius * globalCamera.zoom;
	MovePen(renderPos->x + renderRadius, renderPos->y);
	SetPenColor(circle->super.color);
	SetPenSize(1);
	if (circle->super.fill)
		fill(circle->super.density);

	DrawArc(renderRadius, 0, 360);

	if (circle->super.fill)
		endfill;

	destoryVector(renderPos);
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
