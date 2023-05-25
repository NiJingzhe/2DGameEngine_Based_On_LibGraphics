#include "2DEngine.h"
#include "scene_info.h"
#include "player1.h"
#include "player2.h"
#include "pause_button.h"
#include "count_down.h"

SceneNode scene2;

/////////////////////////////// Actor 与 Component 声明部分 //////////////////////////////////
// 背景Actor
Vector *scene2BackgoundPos;
Rect *scene2BackgoundRect;
ActorNode scene2Backgound;
CollisionShape *scene2BackgoundShape;

static void setupScene_scene2(SceneNode scene2, void *param);

/// @brief 创建场景2，包括场景2与其中各种Actor和组件的内存申请
/// @param scene2
void createScene2(SceneNode *scene2)
{

    *scene2 = newScene("scene2");
    (*scene2)->setup = setupScene_scene2;

    scene2BackgoundPos = newVector(getww / 2, getwh / 2);
    scene2BackgoundRect = newRect(scene2BackgoundPos, 0, getww, getwh, TRUE, "Black", 1);
    scene2Backgound = newActor("scene2Backgound", scene2BackgoundPos);
    scene2BackgoundShape = newCollisionShape((Shape *)scene2BackgoundRect);
    destoryVector(scene2BackgoundPos);
    destoryShape((Shape *)scene2BackgoundRect);
    scene2Backgound->addComponent(scene2Backgound, (ComponentNode)scene2BackgoundShape);

    createPlayer1();
    createPlayer2();
    createCountDown();
    createPauseButton();

    (*scene2)->addActor((*scene2), scene2Backgound);
    (*scene2)->addActor((*scene2), countDownUI);
    (*scene2)->addActor((*scene2), pauseButton);
    (*scene2)->addActor((*scene2), player);
    (*scene2)->addActor((*scene2), player2);

}

/// @brief 场景2的初始化函数，初始化各个Component的属性
/// @param scene2
/// @param param
static void setupScene_scene2(SceneNode scene2, void *param)
{

    if (param != NULL)
    {
        if (*((bool *)param) == TRUE)
        {
            timer->start(timer);
        }
        else
        {
            countDown = 60 * 1000;
            dashPower = 90;
            freezed = FALSE;
        }
    }
    else
    {
        countDown = 60 * 1000;
        dashPower = 90;
        freezed = FALSE;
    }

    // background
    scene2BackgoundShape->enable = FALSE;
    scene2BackgoundShape->visible = TRUE;
    scene2BackgoundShape->setMeta((ComponentNode)scene2BackgoundShape, "scene2BackgoundShape");

    setupPlayer1(param);
    setupPlayer2(param);
    setupPauseButton(param);
    setupCountDown(param);
}

/////////////////////////////// 场景内Actor相关函数实现部分 //////////////////////////////////