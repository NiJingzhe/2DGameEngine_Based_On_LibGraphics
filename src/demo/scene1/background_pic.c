#include "background_pic.h"
#include "scene_info.h"

ActorNode background_pic;
picTexture *background;
picTexture *backgroundWithText;
Timer *switchTimer;
extern bool fromPause;

static void switchBackgroundPic();
static void backgroundUpdate(ActorNode background, double delta);

void createBackgroundPic()
{
    Vector *background_pic_pos = newVector(getww / 2, getwh / 2);
    background_pic = newActor("background_pic", background_pic_pos);
    background = newPicTexture("./res/scene1/background.bmp", background_pic_pos, getww, getwh);
    backgroundWithText = newPicTexture("./res/scene1/background_with_text.bmp", background_pic_pos, getww, getwh);
    switchTimer = newTimer(3, 500, switchBackgroundPic);
    destoryVector(background_pic_pos);

    background_pic->addComponent(background_pic, (ComponentNode)backgroundWithText);
    background_pic->addComponent(background_pic, (ComponentNode)background);
    background_pic->addComponent(background_pic, (ComponentNode)switchTimer);
}

void setupBackgroundPic(void *param)
{
    background->visible = TRUE;
    background->setMeta((ComponentNode)background, "background_pic");
    backgroundWithText->visible = FALSE;
    backgroundWithText->setMeta((ComponentNode)backgroundWithText, "backgroundWithText");
    switchTimer->setMeta((ComponentNode)switchTimer, "switchTimer");
    switchTimer->start(switchTimer);

    background_pic->vptr->update = backgroundUpdate;
}

static void switchBackgroundPic()
{
    background->visible = !background->visible;
    backgroundWithText->visible = !backgroundWithText->visible;
}

static void backgroundUpdate(ActorNode background, double delta)
{
    for (int i = 0; i < 255; ++i)
    {
        if (inmng.keyStates['R'])
        {
            if (inmng.keyBoardEventType == KEY_DOWN)
            {
                if (scmng.getScene("scene2") == NULL)
                    scmng.loadScene(&scene2, createScene2, &fromPause);
                scmng.switchTo("scene2", FALSE, TRUE, &fromPause, sizeof(bool));
            }
        }
    }
}