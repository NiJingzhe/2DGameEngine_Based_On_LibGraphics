#include "camera.h"

Camera globalCamera;

void initCamera(double x, double y, double zoom){
    Vector* camPos = newVector(x, y);
    memcpy(&(globalCamera.position), camPos, sizeof(Vector));
    destoryVector(camPos);
    globalCamera.zoom = zoom;
}