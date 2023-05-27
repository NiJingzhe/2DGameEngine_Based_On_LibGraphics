#include "uitext.h"
#include "Vector.h"
#include "base_component.h"
static void initUIText(struct UIText *uiText, char *content, Vector pos, char *color, char *font, int style, int pointSize);
static void renderUIText(Component *c);
static void updateUIText(Component *c, ...);
static void setUITextPos(UIText *t, Vector *pos);
static Vector *getUITextPos(UIText *t);
static void setUITextContent(UIText *t, char *content);
static char *getUITextContent(UIText *t);
static double getUITextWidth(UIText *t);
static const int returnUITextType(ComponentNode c);

UIText *newUIText(char *content, Vector *pos, char *color, char *font, int style, int pointSize)
{
	UIText *uiText = (UIText *)calloc(1, sizeof(UIText));
	#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	initUIText(uiText, content, *pos, color, font, style, pointSize);
	return uiText;
}

static void initUIText(struct UIText *uiText, char *content, Vector pos, char *color, char *font, int style, int pointSize)
{
	Component *super = newComponent(renderUIText, updateUIText);
	memcpy(&(uiText->super), super, sizeof(Component));
	uiText->super.vptr = NULL;
	uiText->super.vptr = (componentvTable *)calloc(1, sizeof(componentvTable));
#if MEM_DEBUG
	MEM_BLOCK_NUM++;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	if (uiText->super.vptr == NULL)
	{
		printf("Cannot allocate memory for vptr of Texture component\n");
		return;
	}
	memcpy(uiText->super.vptr, super->vptr, sizeof(componentvTable));
	uiText->super.vptr->getComponentType = returnUITextType;
	destoryComponent(super);

	uiText->pos = pos;
	uiText->content = content == NULL ? "" : content;
	uiText->font = font == NULL ? "Consolas" : font;
	uiText->pointSize = pointSize;
	uiText->style = style;
	uiText->color = color;

	double pointSize_ = GetPointSize();
	SetPointSize(uiText->pointSize);
	SetFont(uiText->font);
	SetStyle(uiText->style);
	uiText->width = TextStringWidth(uiText->content);
	SetPointSize(pointSize_);

	uiText->visible = TRUE;

	uiText->setContent = setUITextContent;
	uiText->getContent = getUITextContent;
	uiText->setPos = setUITextPos;
	uiText->getPos = getUITextPos;
	uiText->getWidth = getUITextWidth;
	uiText->setMeta = uiText->super.setMeta;
	uiText->getMeta = uiText->super.getMeta;
}

static void setUITextPos(UIText *t, Vector *pos)
{
	memcpy(&(t->pos), pos, sizeof(Vector));
}

static Vector *getUITextPos(UIText *t)
{
	return &(t->pos);
}

static void setUITextContent(UIText *t, char *content)
{
	t->content = content;
	t->width = TextStringWidth(t->content);
}

static char *getUITextContent(UIText *t)
{
	return t->content;
}

static double getUITextWidth(UIText *t)
{
	return TextStringWidth(t->content);
}

static void renderUIText(Component *c)
{
	UIText *t = (UIText *)c;
	if (t->visible)
	{
		SetPenColor(t->color);
		double pointSize_ = GetPointSize();
		SetPointSize(t->pointSize);
		SetFont(t->font);
		SetStyle(t->style);
		double width = TextStringWidth(t->content);
		t->width = width;
		MovePen(t->pos.x - width / 2, t->pos.y);
		DrawTextString(t->content);
		SetPointSize(pointSize_);
	}
	else
	{
		return;
	}
}

static void updateUIText(Component *c, ...)
{
	UIText *t = (UIText *)c;
	Vector *pos = NULL;
	va_list argList;
	va_start(argList, c);
	pos = va_arg(argList, Vector *);
	t->setPos(t, pos);
	va_end(argList);
}

static const int returnUITextType(ComponentNode c)
{
	return UITEXT;
}

void destoryUIText(struct UIText *uiText)
{
	free(uiText->super.vptr);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
	free(uiText);
#if MEM_DEBUG
	MEM_BLOCK_NUM--;
	printf("\nLOG:\n MEM_BLOCK_NUM: %d", MEM_BLOCK_NUM);
#endif
}