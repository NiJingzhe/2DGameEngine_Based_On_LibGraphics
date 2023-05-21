#include "vector.h"
#include "public_headers.h"

static void initVector(Vector *v, double x, double y);
static void add(Vector *v1, Vector *v2);
static void sub(Vector *v1, Vector *v2);
static void mult(Vector *v, double number);
static void rotate(Vector *v, double angle);
static double getAngle(Vector *v);
static double length(Vector *v);
static void normalize(Vector *v);

Vector *newVector(double x, double y)
{
	Vector *v = (Vector *)calloc(1, sizeof(Vector));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	initVector(v, x, y);
	return v;
}

static void initVector(Vector *v, double x, double y)
{
	v->x = x;
	v->y = y;
	v->add = add;
	v->sub = sub;
	v->mult = mult;
	v->rotate = rotate;
	v->getAngle = getAngle;
	v->length = length;
	v->normalize = normalize;
	v->destoryVector = destoryVector;
}

static void add(Vector *v1, Vector *v2)
{
	v1->x += v2->x;
	v1->y += v2->y;
}

static void sub(Vector *v1, Vector *v2)
{
	v1->x -= v2->x;
	v1->y -= v2->y;
}

static void mult(Vector *v, double number)
{
	v->x *= number;
	v->y *= number;
}

static void rotate(Vector *v, double angle)
{
	double x = v->x * cos(angle) - v->y * sin(angle);
	double y = v->x * sin(angle) + v->y * cos(angle);
	v->x = x;
	v->y = y;
}

static double getAngle(Vector *v)
{
	if (v->y == 0)
	{
		return v->x > 0 ? 0 : PI;
	}
	if (v->x == 0)
	{
		return v->y > 0 ? PI / 2 : -PI / 2;
	}
	if (v->x == 0 && v->y == 0)
	{
		return 0.0;
	}
	return atan2(v->y, v->x);
}

static double length(Vector *v)
{
	return sqrt(v->x * v->x + v->y * v->y);
}

static void normalize(Vector *v)
{
	if (v->length(v) == 0)
	{
		return;
	}
	double len = v->length(v);
	v->x /= len;
	v->y /= len;
}

void destoryVector(Vector *v)
{
	free(v);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	v = NULL;
}
