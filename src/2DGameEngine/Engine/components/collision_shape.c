#include "collision_shape.h"
#include "base_component.h"

static void initCollisionShape(CollisionShape *c, Shape *shape);
static void renderCollisionShape(Component *c);
static void updateCollisionShape(Component *c, ...);
static bool isCollideWith(CollisionShape *s1, CollisionShape *s2);
static Vector *getCollisionVector(CollisionShape *cs1, CollisionShape *cs2);
static const int returnCollisionShape();
static void setCollisionShapePos(CollisionShape *c, Vector *pos);
static Vector *getCollisionShapePos(CollisionShape *c);

CollisionShape *newCollisionShape(Shape *shape)
{
	CollisionShape *collisionShape = (CollisionShape *)calloc(1, sizeof(CollisionShape));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	initCollisionShape(collisionShape, shape);
	return collisionShape;
}

static void initCollisionShape(CollisionShape *c, Shape *shape)
{
	switch (shape->vptr->getShape())
	{
	case RECT:
		c->shape = (Shape *)calloc(1, sizeof(Rect));
#if MEM_DEBUG
		MEM_BLOCK_NUM++;
		printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
		if (c->shape == NULL)
		{
			printf("Cannot allocate memory for shape of collionShape component\n");
			return;
		}
		memcpy(c->shape, (Rect *)shape, sizeof(Rect));
		c->shape->vptr = NULL;
		c->shape->vptr = (shapevTable *)calloc(1, sizeof(shapevTable));
#if MEM_DEBUG
		MEM_BLOCK_NUM++;
		printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
		memcpy(c->shape->vptr, shape->vptr, sizeof(shapevTable));

		// 为自身shape属性下的顶点数组开辟新空间，并内存拷贝
		c->shape->vertices = (Vector **)calloc(4, sizeof(Vector *));
#if MEM_DEBUG
		MEM_BLOCK_NUM += 4;
		printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
		for (int i = 0; i < 4; ++i)
		{
			c->shape->vertices[i] = (Vector *)calloc(1, sizeof(Vector));
			memcpy(c->shape->vertices[i], shape->vertices[i], sizeof(Vector));
#if MEM_DEBUG
			MEM_BLOCK_NUM++;
			printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
		}
		break;

	case CIRCLE:
		c->shape = (Shape *)calloc(1, sizeof(Circle));
#if MEM_DEBUG
		MEM_BLOCK_NUM++;
		printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
		if (c->shape == NULL)
		{
			printf("Cannot allocate memory for shape of collionShape component\n");
			return;
		}
		memcpy(c->shape, (Circle *)shape, sizeof(Circle));
		c->shape->vptr = NULL;
		c->shape->vptr = (shapevTable *)calloc(1, sizeof(shapevTable));
#if MEM_DEBUG
		MEM_BLOCK_NUM++;
		printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
		memcpy(c->shape->vptr, shape->vptr, sizeof(shapevTable));
		break;

	default:
		printf("YOU SHOULD SPECIFIC A TYPE OF SHAPE!!!\n");
		return;
	}

	Component *super = newComponent(renderCollisionShape, updateCollisionShape);
	memcpy(&(c->super), super, sizeof(Component));
	c->super.vptr = NULL;
	c->super.vptr = (componentvTable *)calloc(1, sizeof(componentvTable));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	if (c->super.vptr == NULL)
	{
		printf("Cannot allocate memory for vptr of collionShape component\n");
		return;
	}
	memcpy(c->super.vptr, super->vptr, sizeof(componentvTable));
	c->super.vptr->getComponentType = returnCollisionShape;
	destoryComponent(super);

	c->enable = TRUE;
	c->pos.x = c->shape->pos.x;
	c->pos.y = c->shape->pos.y;
	c->getMeta = c->super.getMeta;
	c->setMeta = c->super.setMeta;
	c->getPos = getCollisionShapePos;
	c->setPos = setCollisionShapePos;
	c->visible = FALSE;

	c->isCollideWith = isCollideWith;
	c->getCollisionVector = getCollisionVector;
}

static void renderCollisionShape(Component *c)
{
	CollisionShape *cs = (CollisionShape *)c;
	if (cs->visible)
		cs->shape->vptr->render(cs->shape);
	else
		return;
}

static void updateCollisionShape(Component *c, ...)
{
	CollisionShape *cs = (CollisionShape *)c;
	Vector *pos = NULL;
	va_list argList;
	va_start(argList, c);
	pos = va_arg(argList, Vector *);
	cs->shape->setPos(cs->shape, pos);
	cs->setPos(cs, pos);
	va_end(argList);

	if (cs->shape->vptr->getShape() == RECT)
	{
		double width = ((Rect *)cs->shape)->width;
		double height = ((Rect *)cs->shape)->height;
		((Rect *)cs->shape)->super.vertices[0]->x = -width / 2;
		((Rect *)cs->shape)->super.vertices[0]->y = height / 2;
		((Rect *)cs->shape)->super.vertices[1]->x = width / 2;
		((Rect *)cs->shape)->super.vertices[1]->y = height / 2;
		((Rect *)cs->shape)->super.vertices[2]->x = width / 2;
		((Rect *)cs->shape)->super.vertices[2]->y = -height / 2;
		((Rect *)cs->shape)->super.vertices[3]->x = -width / 2;
		((Rect *)cs->shape)->super.vertices[3]->y = -height / 2;
	}
}

static bool isCollideWith(CollisionShape *s1, CollisionShape *s2)
{
	if (s1->enable && s2->enable)
	{
		if (s1->shape->isCollideWith(s1->shape, s2->shape))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

static Vector *getCollisionVector(CollisionShape *cs1, CollisionShape *cs2)
{
	return cs1->shape->getCollisionVector(cs1->shape, cs2->shape);
}

void setCollisionShapePos(CollisionShape *c, Vector *pos)
{
	memcpy(&(c->pos), pos, sizeof(Vector));
}
static Vector *getCollisionShapePos(CollisionShape *c)
{
	return &(c->pos);
}

static const int returnCollisionShape()
{
	return COLLISION_SHAPE;
}

void destoryCollisionShape(CollisionShape *cs)
{
	free(cs->super.vptr);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	destoryShape(cs->shape);
	free(cs);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	cs = NULL;
}