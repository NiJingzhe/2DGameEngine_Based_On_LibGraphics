#include "2DEngine.h"
#include "scene_info.h"

SceneNode scene1;

Vector *backgroundPos;
Rect *backgroundRect;
ActorNode background;
CollisionShape *backgroundShape;
Vector *titlePos;
ActorNode title;
Texture *titleTexture;

Vector *startButtonPos;
ActorNode startButton;
Texture *startButtonTexture;

static void setupScene_scene1(SceneNode scene1, void *param);
static void startButtonUpdate(ActorNode button, double delta);

void createScene1(SceneNode *scene1)
{
    *scene1 = newScene("scene1");
    (*scene1)->setup = setupScene_scene1;

    backgroundPos = newVector(getww / 2, getwh / 2);
    backgroundRect = newRect(backgroundPos, 0, getww, getwh, TRUE, "Black", 1);
    background = newActor("background", backgroundPos);
    backgroundShape = newCollisionShape((Shape *)backgroundRect);

    titlePos = newVector(getww / 2, getwh * 3 / 4);
    title = newActor("startup_title", titlePos);
    titleTexture = newTexture("./res/scene1/title_texture.txt", titlePos, "Red", 18);
    startButtonPos = newVector(getww / 2, getwh / 3);
    startButton = newActor("start_button", startButtonPos);
    startButtonTexture = newTexture("./res/scene1/start_button_texture.txt", startButtonPos, "Red", 8);

    background->addComponent(background, (ComponentNode)backgroundShape);
    title->addComponent(title, (ComponentNode)titleTexture);
    startButton->addComponent(startButton, (ComponentNode)startButtonTexture);

    (*scene1)->addActor(*scene1, background);
    (*scene1)->addActor(*scene1, title);
    (*scene1)->addActor(*scene1, startButton);

    destoryVector(backgroundPos);
    destoryShape((Shape *)backgroundRect);
    destoryVector(titlePos);
    destoryVector(startButtonPos);

    LOG("finished Scene1 create.");
}

void setupScene_scene1(SceneNode scene1, void *param)
{
    LOG("enter setup scene1\n");
    backgroundShape->enable = FALSE;
    backgroundShape->visible = TRUE;
    backgroundShape->setMeta((ComponentNode)backgroundShape, "backgroundShape");

    titleTexture->visible = TRUE;
    titleTexture->setMeta((ComponentNode)titleTexture, "title_texture");

    startButtonTexture->visible = TRUE;
    startButtonTexture->setMeta((ComponentNode)startButtonTexture, "start_button_texture");

    startButton->vptr->update = startButtonUpdate;

    LOG("finished setup scene1\n");
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
            scmng.loadScene(&scene2, createScene2);
            scmng.switchTo("scene2", TRUE, FALSE, NULL, 0);
        }
        else if (inmng.mouseButtons[0] && inmng.mouseEventType == BUTTON_DOWN)
        {
            buttonTexture->color = "Blue";
        }
        else
        {
            buttonTexture->color = "Green";
        }
    }
    else
    {
        buttonTexture->color = "Red";
    }
}