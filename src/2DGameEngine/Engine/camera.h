#ifndef __CAM_H__
#define __CAM_H__
#include "public_headers.h"
#include "vector.h"

typedef struct Camera {
    Vector position;
    double zoom;
} Camera;

extern Camera globalCamera;
void initCamera(double x, double y, double zoom);

#endif