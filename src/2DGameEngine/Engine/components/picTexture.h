#ifndef _PICTEXTURE_H
#define _PICTEXTURE_H
#include "base_component.h"
#include "public_headers.h"
#include "vector.h"

typedef struct picTexture
{
    Component super;
    Vector pos;
    char *resPath;
    double width, height;
    bool visible;

    void (*setPos)(struct picTexture *, Vector *);
    Vector *(*getPos)(struct picTexture *);
    double (*getWidth)(struct picTexture *);
    double (*getHeight)(struct picTexture *);
    char *(*getMeta)(struct Component *);
    void (*setMeta)(struct Component *, char *meta);
    void (*resetTexture)(struct picTexture *, char *resPath);
} picTexture;

picTexture *newPicTexture(char *resPath, Vector *pos, double width, double height);
void destroyPicTexture(picTexture *picTexture);

#endif