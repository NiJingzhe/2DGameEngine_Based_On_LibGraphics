#include "2DEngine.h"
#include "startup_title.h"

Vector *titlePos;
ActorNode title;
charTexture *titlecharTexture;

void createStartupTitle()
{
    titlePos = newVector(getww / 2, getwh * 3 / 4);
    title = newActor("startup_title", titlePos);
    titlecharTexture = newcharTexture("./res/scene1/title_texture.txt", titlePos, "White", 18);
    title->addComponent(title, (ComponentNode)titlecharTexture);

    destoryVector(titlePos);
}

void setupStartupTitle(void *param)
{
    titlecharTexture->visible = TRUE;
    titlecharTexture->setMeta((ComponentNode)titlecharTexture, "title_charTexture");

    title->vptr->render = titleRender;
}

void titleRender(ActorNode title)
{
    ComponentNode currentComp = title->componentList;
    while (currentComp && currentComp->vptr != NULL && currentComp->vptr->render != 0)
    {
        currentComp->vptr->render(currentComp);
        currentComp = currentComp->next;
    }

    charTexture *titlecharTexture = (charTexture *)(title->getComponent(title, "title_charTexture"));
    Vector originPos;
    memcpy(&(originPos), &(titlecharTexture->pos), sizeof(Vector));

    static double bias = 0;
    double temp = ((rand() % (5 - 0 + 1)) + 0) / 50.0;
    bias = abs(bias - temp) > 0.03 && abs(bias - temp) < 0.05 ? temp : bias;
    titlecharTexture->pos.x -= bias;
    titlecharTexture->color = "Cyan";
    titlecharTexture->super.vptr->render((ComponentNode)titlecharTexture);

    bias = ((rand() % (5 - 0 + 1)) + 0) / 30.0;
    titlecharTexture->pos.x += bias;
    titlecharTexture->color = "Red";
    titlecharTexture->super.vptr->render((ComponentNode)titlecharTexture);

    titlecharTexture->pos.x = originPos.x;
    titlecharTexture->pos.y = originPos.y;
    titlecharTexture->color = "White";
}
