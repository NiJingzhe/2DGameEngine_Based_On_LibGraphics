#include "2DEngine.h"
#include "scene_info.h"

SceneNode scene1;

Vector *scene1BackgroundPos;
Rect *scene1BackgroundRect;
ActorNode scene1Background;
CollisionShape *scene1BackgroundShape;

Vector *titlePos;
ActorNode title;
Texture *titleTexture;

Vector *startButtonPos;
ActorNode startButton;
Texture *startButtonTexture;

ActorNode scene1BGM;
Audio *bgm;

static void setupScene_scene1(SceneNode scene1, void *param);
static void startButtonUpdate(ActorNode button, double delta);
static void bgmUpdate(ActorNode scene1BGM, double delta);
static void titleRender(ActorNode title);

void createScene1(SceneNode *scene1)
{
    *scene1 = newScene("scene1");
    (*scene1)->setup = setupScene_scene1;

    scene1BackgroundPos = newVector(getww / 2, getwh / 2);
    scene1BackgroundRect = newRect(scene1BackgroundPos, 0, getww, getwh, TRUE, "Black", 1);
    scene1Background = newActor("scene1Background", scene1BackgroundPos);
    scene1BackgroundShape = newCollisionShape((Shape *)scene1BackgroundRect);

    titlePos = newVector(getww / 2, getwh * 3 / 4);
    title = newActor("startup_title", titlePos);
    titleTexture = newTexture("./res/scene1/title_texture.txt", titlePos, "White", 18);

    startButtonPos = newVector(getww / 2, getwh * 4 / 10);
    startButton = newActor("start_button", startButtonPos);
    startButtonTexture = newTexture("./res/scene1/start_button_texture.txt", startButtonPos, "White", 5);

    scene1BGM = newActor("scene1_bgm", startButtonPos);
    bgm = newAudio("./res/scene1/bgm.mp3", TRUE);

    scene1Background->addComponent(scene1Background, (ComponentNode)scene1BackgroundShape);
    title->addComponent(title, (ComponentNode)titleTexture);
    startButton->addComponent(startButton, (ComponentNode)startButtonTexture);
    scene1BGM->addComponent(scene1BGM, (ComponentNode)bgm);

    (*scene1)->addActor(*scene1, scene1Background);
    (*scene1)->addActor(*scene1, title);
    (*scene1)->addActor(*scene1, startButton);
    (*scene1)->addActor(*scene1, scene1BGM);

    destoryVector(scene1BackgroundPos);
    destoryShape((Shape *)scene1BackgroundRect);
    destoryVector(titlePos);
    destoryVector(startButtonPos);
}

void setupScene_scene1(SceneNode scene1, void *param)
{

    scene1BackgroundShape->enable = FALSE;
    scene1BackgroundShape->visible = TRUE;
    scene1BackgroundShape->setMeta((ComponentNode)scene1BackgroundShape, "scene1BackgroundShape");

    titleTexture->visible = TRUE;
    titleTexture->setMeta((ComponentNode)titleTexture, "title_texture");

    title->vptr->render = titleRender;

    startButtonTexture->visible = TRUE;
    startButtonTexture->setMeta((ComponentNode)startButtonTexture, "start_button_texture");

    startButton->vptr->update = startButtonUpdate;

    bgm->setMeta((ComponentNode)bgm, "scene1_bgm");

    scene1BGM->vptr->update = bgmUpdate;
    
}

static void titleRender(ActorNode title)
{
	ComponentNode currentComp = title->componentList;
	while (currentComp && currentComp->vptr != NULL && currentComp->vptr->render != 0)
	{
		currentComp->vptr->render(currentComp);
		currentComp = currentComp->next;
	}

    Texture *titleTexture = (Texture*)(title->getComponent(title, "title_texture"));
    Vector originPos;
    memcpy(&(originPos), &(titleTexture->pos), sizeof(Vector));

    static double bias = 0;
    double temp = ((rand() % (5 - 0 + 1)) + 0) / 50.0;
    bias = abs(bias - temp) > 0.03 && abs(bias - temp) < 0.05 ? temp : bias;
    titleTexture->pos.x -= bias;
    titleTexture->color = "Cyan";
    titleTexture->super.vptr->render((ComponentNode)titleTexture);

    bias = ((rand() % (5 - 0 + 1)) + 0) / 30.0;
    titleTexture->pos.x += bias;
    titleTexture->color = "Red";
    titleTexture->super.vptr->render((ComponentNode)titleTexture);

    titleTexture->pos.x = originPos.x;
    titleTexture->pos.y = originPos.y;
    titleTexture->color = "White";

}

static void bgmUpdate(ActorNode scene1BGM, double delta){
    Audio *bgm = (Audio*)(scene1BGM->getComponent(scene1BGM, "scene1_bgm"));
    if (!bgm->playing){
        bgm->play(bgm);
    }
}

static void startButtonUpdate(ActorNode button, double delta)
{
    double mouseX = inmng.mouseX;
    double mouseY = inmng.mouseY;

    Texture *buttonTexture = (Texture *)(button->getComponent(button, "start_button_texture"));
    Vector pos = *(buttonTexture->getPos(buttonTexture));
    double width = buttonTexture->getWidth(buttonTexture);
    double height = buttonTexture->getHeight(buttonTexture);

    if (mouseX > pos.x - width / 2 && mouseX < pos.x + width / 2 && mouseY > pos.y - height / 2 && mouseY < pos.y + height / 2)
    {
        if (inmng.mouseButtons[0] && inmng.mouseEventType == BUTTON_UP)
        {
            if (scmng.getScene("scene2") == NULL)
                scmng.loadScene(&scene2, createScene2);
            scmng.switchTo("scene2", FALSE, FALSE, NULL, 0);
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