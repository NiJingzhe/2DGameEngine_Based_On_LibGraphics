#include "2DEngine.h"
#include "scene_info.h"

SceneNode scene3;

Vector *scene3BackgroundPos;
Rect *scene3BackgroundRect;
ActorNode scene3Background;
CollisionShape *scene3BackgroundShape;

Vector *resultPos;
ActorNode result;
charTexture *resultTexture;

Vector *replayButtonPos;
ActorNode replayButton;
charTexture *replayButtonTexture;


static void setupScene_scene3(SceneNode scene3, void *param);
static void replayButtonUpdate(ActorNode button, double delta);
static void resultRenderChaserWin(ActorNode result);
static void resultRenderEscaperWin(ActorNode result);

void createScene3(SceneNode *scene3)
{
    *scene3 = newScene("scene3");
    (*scene3)->setup = setupScene_scene3;

    scene3BackgroundPos = newVector(getww / 2, getwh / 2);
    scene3BackgroundRect = newRect(scene3BackgroundPos, 0, getww, getwh, TRUE, "Black", 1);
    scene3Background = newActor("scene3Background", scene3BackgroundPos);
    scene3BackgroundShape = newCollisionShape((Shape *)scene3BackgroundRect);

    resultPos = newVector(getww / 2, getwh * 3 / 4);
    result = newActor("replayup_result", resultPos);
    resultTexture = newcharTexture("./res/scene3/chaserwin_texture.txt", resultPos, "White", 18);

    replayButtonPos = newVector(getww / 2, getwh * 4 / 10);
    replayButton = newActor("replay_button", replayButtonPos);
    replayButtonTexture = newcharTexture("./res/scene3/replay_button.txt", replayButtonPos, "White", 5);

    scene3Background->addComponent(scene3Background, (ComponentNode)scene3BackgroundShape);
    result->addComponent(result, (ComponentNode)resultTexture);
    replayButton->addComponent(replayButton, (ComponentNode)replayButtonTexture);

    (*scene3)->addActor(*scene3, scene3Background);
    (*scene3)->addActor(*scene3, result);
    (*scene3)->addActor(*scene3, replayButton);

    destoryVector(scene3BackgroundPos);
    destoryShape((Shape *)scene3BackgroundRect);
    destoryVector(resultPos);
    destoryVector(replayButtonPos);
}

void setupScene_scene3(SceneNode scene3, void *param)
{
    scene3BackgroundShape->enable = FALSE;
    scene3BackgroundShape->visible = TRUE;
    scene3BackgroundShape->setMeta((ComponentNode)scene3BackgroundShape, "scene3BackgroundShape");

    resultTexture->visible = TRUE;
    resultTexture->setMeta((ComponentNode)resultTexture, "result_texture");

    result->vptr->render = resultRenderChaserWin;

    if (param != NULL && strcmp((char*)param, "EscaperWin") == 0){
        resultTexture->resetTexture(resultTexture, "./res/scene3/escapewin_texture.txt");
        result->vptr->render = resultRenderEscaperWin;
    }
    else if (param == NULL){
        Error("Invalid parameter");
    }


    replayButtonTexture->visible = TRUE;
    replayButtonTexture->setMeta((ComponentNode)replayButtonTexture, "replay_button_texture");

    replayButton->vptr->update = replayButtonUpdate;

}

static void resultRenderChaserWin(ActorNode result)
{
	ComponentNode currentComp = result->componentList;
	while (currentComp && currentComp->vptr != NULL && currentComp->vptr->render != 0)
	{
		currentComp->vptr->render(currentComp);
		currentComp = currentComp->next;
	}

    charTexture *resultTexture = (charTexture*)(result->getComponent(result, "result_texture"));
    Vector originPos;
    memcpy(&(originPos), &(resultTexture->pos), sizeof(Vector));

    static double bias = 0;
    double temp = ((rand() % (5 - 0 + 1)) + 0) / 50.0;
    bias = abs(bias - temp) > 0.03 && abs(bias - temp) < 0.05 ? temp : bias;
    resultTexture->pos.x -= bias;
    resultTexture->color = "Red";
    resultTexture->super.vptr->render((ComponentNode)resultTexture);

    bias = ((rand() % (5 - 0 + 1)) + 0) / 30.0;
    resultTexture->pos.x += bias;
    resultTexture->color = "Red";
    resultTexture->super.vptr->render((ComponentNode)resultTexture);

    resultTexture->pos.x = originPos.x;
    resultTexture->pos.y = originPos.y;
    resultTexture->color = "Blue";

}

static void resultRenderEscaperWin(ActorNode result)
{
	ComponentNode currentComp = result->componentList;
	while (currentComp && currentComp->vptr != NULL && currentComp->vptr->render != 0)
	{
		currentComp->vptr->render(currentComp);
		currentComp = currentComp->next;
	}

    charTexture *resultTexture = (charTexture*)(result->getComponent(result, "result_texture"));
    Vector originPos;
    memcpy(&(originPos), &(resultTexture->pos), sizeof(Vector));

    static double bias = 0;
    double temp = ((rand() % (5 - 0 + 1)) + 0) / 50.0;
    bias = abs(bias - temp) > 0.03 && abs(bias - temp) < 0.05 ? temp : bias;
    resultTexture->pos.x -= bias;
    resultTexture->color = "Cyan";
    resultTexture->super.vptr->render((ComponentNode)resultTexture);

    bias = ((rand() % (5 - 0 + 1)) + 0) / 30.0;
    resultTexture->pos.x += bias;
    resultTexture->color = "Cyan";
    resultTexture->super.vptr->render((ComponentNode)resultTexture);

    resultTexture->pos.x = originPos.x;
    resultTexture->pos.y = originPos.y;
    resultTexture->color = "Red";

}


static void replayButtonUpdate(ActorNode button, double delta)
{
    double mouseX = inmng.mouseX;
    double mouseY = inmng.mouseY;

    charTexture *buttonTexture = (charTexture *)(button->getComponent(button, "replay_button_texture"));
    Vector pos = *(buttonTexture->getPos(buttonTexture));
    double width = buttonTexture->getWidth(buttonTexture);
    double height = buttonTexture->getHeight(buttonTexture);

    if (mouseX > pos.x - width / 2 && mouseX < pos.x + width / 2 && mouseY > pos.y - height / 2 && mouseY < pos.y + height / 2)
    {
        if (inmng.mouseButtons[0] && inmng.mouseEventType == BUTTON_UP)
        {
            if (scmng.getScene("scene1") == NULL)
                scmng.loadScene(&scene1, createScene1, NULL);
            scmng.switchTo("scene1", TRUE, TRUE, NULL, 0);
        }
        else if (inmng.mouseButtons[0] && inmng.mouseEventType == BUTTON_DOWN)
        {
            buttonTexture->color = "Red";
        }
        else
        {
            buttonTexture->color = "Cyan";
        }
    }
    else
    {
        buttonTexture->color = "White";
    }
}