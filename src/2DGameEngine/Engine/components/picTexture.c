#include "picTexture.h"

#include "picTexture.h"
#include "base_component.h"
static void initTexture(picTexture *t, char *resPath, Vector pos, double width, double height);
static void renderTexture(Component *c);
static void updateTexture(Component *c, ...); // ... should contain and only contain a Vector pointer represents the position of texture
static void setTexturePos(picTexture *t, Vector *pos);
static void resetTexture(picTexture *t, char *resPath);
static Vector *getTexturePos(picTexture *t);
static double getWidth(picTexture *t);
static double getHeight(picTexture *t);
static const int returnTexture();

picTexture *newPicTexture(char *resPath, Vector *pos, double width, double height)
{
    picTexture *t = (picTexture *)calloc(1, sizeof(picTexture));
#if MEM_DEBUG
    MEM_BLOCK_NUM++;
    printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
    initTexture(t, resPath, *pos, width, height);
    return t;
}

static void initTexture(picTexture *t, char *resPath, Vector pos, double width, double height)
{

    Component *super = newComponent(renderTexture, updateTexture);
    memcpy(&(t->super), super, sizeof(Component));
    t->super.vptr = NULL;
    t->super.vptr = (componentvTable *)calloc(1, sizeof(componentvTable));
#if MEM_DEBUG
    MEM_BLOCK_NUM++;
    printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
    if (t->super.vptr == NULL)
    {
        printf("Cannot allocate memory for vptr of Texture component\n");
        return;
    }
    memcpy(t->super.vptr, super->vptr, sizeof(componentvTable));
    t->super.vptr->getComponentType = returnTexture;
    destoryComponent(super);

    t->resPath = resPath;
    t->pos = pos;
    t->width = width;
    t->height = height;

    t->setPos = setTexturePos;
    t->getPos = getTexturePos;
    t->getHeight = getHeight;
    t->getWidth = getWidth;
    t->resetTexture = resetTexture;

    t->getMeta = t->super.getMeta;
    t->setMeta = t->super.setMeta;
}

static void resetTexture(picTexture *t, char *resPath)
{
    t->resPath = resPath;
}

static void updateTexture(Component *c, ...)
{
    picTexture *t = (picTexture *)c;
    Vector *pos = NULL;
    va_list argList;
    va_start(argList, c);
    pos = va_arg(argList, Vector *);
    t->setPos(t, pos);
    va_end(argList);
}

static void renderTexture(Component *c)
{
    picTexture *t = (picTexture *)c;
    if (t->visible)
    {
        LoadBmp(t->resPath, t->pos.x, t->pos.y, t->width, t->height);
    }
    else
    {
        return;
    }
}

static void setTexturePos(picTexture *t, Vector *pos)
{
    t->pos = *pos;
}

static Vector *getTexturePos(picTexture *t)
{
    return &(t->pos);
}

static double getHeight(picTexture *t)
{
    return t->height;
}

static double getWidth(picTexture *t)
{
    return t->width;
}

static const int returnTexture()
{
    return PICTEXTURE;
}

void destroyPicTexture(picTexture *picTexture)
{
    free(picTexture->super.vptr);
#if MEM_DEBUG
    MEM_BLOCK_NUM--;
    printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
    free(picTexture);
#if MEM_DEBUG
    MEM_BLOCK_NUM--;
    printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
}