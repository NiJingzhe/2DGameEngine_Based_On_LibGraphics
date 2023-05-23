#include "2DEngine.h"
#include "scene_info.h"
#include "startup_title.h"
#include "startup_button.h"
#include "scene1_bgm.h"
#include "background_pic.h"

SceneNode scene1;

Vector *scene1BackgroundPos;
Rect *scene1BackgroundRect;
ActorNode scene1Background;
CollisionShape *scene1BackgroundShape;


bool fromPause = FALSE;

static void setupScene_scene1(SceneNode scene1, void *param);

void createScene1(SceneNode *scene1)
{
    *scene1 = newScene("scene1");
    (*scene1)->setup = setupScene_scene1;

    scene1BackgroundPos = newVector(getww / 2, getwh / 2);
    scene1BackgroundRect = newRect(scene1BackgroundPos, 0, getww, getwh, TRUE, "Black", 1);
    scene1Background = newActor("scene1Background", scene1BackgroundPos);
    scene1BackgroundShape = newCollisionShape((Shape *)scene1BackgroundRect);
    scene1Background->addComponent(scene1Background, (ComponentNode)scene1BackgroundShape);
    (*scene1)->addActor(*scene1, scene1Background);
    destoryVector(scene1BackgroundPos);
    destoryShape((Shape *)scene1BackgroundRect);

    createBackgroundPic();
    createStartupTitle();
    //createStartupButton();
    createScene1BGM();

    (*scene1)->addActor(*scene1, background_pic);
    (*scene1)->addActor(*scene1, title);
    //(*scene1)->addActor(*scene1, startButton);
    (*scene1)->addActor(*scene1, scene1BGM);
}

void setupScene_scene1(SceneNode scene1, void *param)
{

    if (param != NULL)
    {
        if (strcmp(param, "from pause") == 0)
        {
            fromPause = TRUE;
        }
        else
        {
            fromPause = FALSE;
        }
    }
    else
    {
        fromPause = FALSE;
    }
    scene1BackgroundShape->enable = FALSE;
    scene1BackgroundShape->visible = TRUE;
    scene1BackgroundShape->setMeta((ComponentNode)scene1BackgroundShape, "scene1BackgroundShape");

    setupBackgroundPic(param);
    setupStartupTitle(param);
    //setupStartupButton(param);
    setupScene1BGM(param);

}

