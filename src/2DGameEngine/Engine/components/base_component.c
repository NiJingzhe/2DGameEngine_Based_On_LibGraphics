#include "base_component.h"
//自定义组件的头文件需要包含于此
#include "collision_shape.h"
#include "texture.h"
#include "uitext.h"
#include "timer.h"
#include "audio.h"

static void initComponent(Component *c, ComponentRender render, ComponentUpdate update);
static char *getComponentMeta(Component *c);
static void setComponentMeta(Component *c, char *meta);
static const int returnEmptyType();


Component *newComponent(ComponentRender render, ComponentUpdate update)
{
	Component *component = (Component *)calloc(1, sizeof(Component));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	initComponent(component, render, update);
	return component;
}

static void initComponent(Component *c, ComponentRender render, ComponentUpdate update)
{
	c->vptr = (componentvTable *)calloc(1, sizeof(componentvTable));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	if (c->vptr == NULL)
	{
		printf("Cannot allocate memory for component vptr");
		return;
	}
	c->vptr->render = render;
	c->vptr->update = update;
	c->vptr->getComponentType = returnEmptyType;
	c->getMeta = getComponentMeta;
	c->setMeta = setComponentMeta;

	c->next = NULL;
	c->prev = NULL;

	c->meta = "ComponentBaseClass";
}

static char *getComponentMeta(Component *c)
{
	return c->meta;
}

static void setComponentMeta(Component *c, char *meta)
{
	c->meta = meta;
}

static const int returnEmptyType()
{
	return EMPTY_COMPONENT;
}

void destoryComponent(Component *c)
{
#if MEM_DEBUG
	printf("\nLOG:\n		Enter destoryComponent, destorying component: %s\n", c->meta);
#endif
	switch (c->vptr->getComponentType())
	{
	case EMPTY_COMPONENT:
		free(c->vptr);
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
		break;

	case COLLISION_SHAPE:
		destoryCollisionShape((CollisionShape *)c);
		break;

	case TEXTURE:
		destoryTexture((Texture *)c);
		break;

	case AUDIO:
		destoryAudio((Audio *)c);
		break;

	case TIMER:
		destoryTimer((Timer *)c);
		break;

	case UITEXT:
		destoryUIText((UIText *)c);
		break;
	}
}
