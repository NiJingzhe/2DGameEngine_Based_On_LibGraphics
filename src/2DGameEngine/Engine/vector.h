#ifndef __VECTOR_H__
#define __VECTOR_H__

typedef struct Vector{
	double x, y;
	
	void (*add)(struct Vector*, struct Vector*);
	void (*sub)(struct Vector* v1, struct Vector*);
	void (*mult)(struct Vector*, double);
	void (*rotate)(struct Vector*, double);
	double (*getAngle)(struct Vector*);
	double (*length)(struct Vector*);
	void (*normalize)(struct Vector*);
	struct Vector* (*getOrthogonal)(struct Vector*);
	void (*destoryVector)(struct Vector*);

} Vector;

Vector* newVector(double x, double y);

void destoryVector(Vector* v);

#endif