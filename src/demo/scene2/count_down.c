#include "count_down.h"
#include "scene_info.h"

Vector *countDownPos;
ActorNode countDownUI;
Timer *timer;
UIText *countDownText;

// 单位是ms
long long int countDown;
// 单位是s
long long int aliveTime;

static void countDownUIUpdate(ActorNode countDownUI, double delta);
static void CALLBACK countDownUpdate(HWND hwnd, UINT msg, UINT_PTR timerID, DWORD dwTime);

void createCountDown()
{
    // countdownUI
    countDownPos = newVector(getww / 2, getwh * 8 / 10);
    countDownUI = newActor("count_down_ui", countDownPos);
    timer = newTimer(1, 1000, countDownUpdate);
    countDownText = newUIText("01:00", countDownPos, "White", "Consolas", Bold, 50);

    countDownUI->addComponent(countDownUI, (ComponentNode)countDownText);
    countDownUI->addComponent(countDownUI, (ComponentNode)timer);

    destoryVector(countDownPos);
}

void setupCountDown(void *param)
{
    // countdown ui
    timer->setMeta((ComponentNode)timer, "count_down_timer");
    timer->start(timer);
    countDownText->setMeta((ComponentNode)countDownText, "count_down_text");
    countDownUI->vptr->update = countDownUIUpdate;
}

static void countDownUIUpdate(ActorNode countDownUI, double delta)
{
    UIText *text = (UIText *)countDownUI->getComponent(countDownUI, "count_down_text");
    Timer *timer = (Timer *)countDownUI->getComponent(countDownUI, "count_down_timer");

    static char countDownString[255] = {'\0'};
    static int second = 0, minute = 2;
    second = countDown / 1000;
    minute = 0;
    if (second == 60)
    {
        minute = 1;
        second = 0;
    }

    sprintf(countDownString, "%02d : %02d", minute, second);
    text->setContent(text, countDownString);

    if (countDown == 0)
    {
        timer->stop(timer);
        char param[255] = "EscaperWin";
        if (scmng.getScene("scene3") == NULL)
        {
            scmng.loadScene(&scene3, createScene3, (void *)param);
        }
        scmng.switchTo("scene3", TRUE, TRUE, (void *)param, strlen(param) * sizeof(char));
        // 切换到解算界面
    }
}

static void CALLBACK countDownUpdate(HWND hwnd, UINT msg, UINT_PTR timerID, DWORD dwTime)
{
    aliveTime++;
    countDown = countDown > 0 ? countDown - 1000 : 0;
}