#include "pause_button.h"
#include "scene_info.h"
#include "count_down.h"

Vector *pauseButtonPos;
ActorNode pauseButton;
charTexture *pauseButtonTexture;

static void pauseButtonUpdate(ActorNode button, double delta);

void createPauseButton()
{
    // pause button
    pauseButtonPos = newVector(getww / 12, getwh * 7 / 8);
    pauseButton = newActor("pause_button", pauseButtonPos);
    pauseButtonTexture = newcharTexture("./res/scene2/pause_button_texture.txt", pauseButtonPos, "White", 1);
    pauseButton->addComponent(pauseButton, (ComponentNode)pauseButtonTexture);
    destoryVector(pauseButtonPos);
}

void setupPauseButton(void *param)
{
    // pause button
    pauseButtonTexture->visible = TRUE;
    pauseButtonTexture->setMeta((ComponentNode)pauseButtonTexture, "pause_button_texture");
    pauseButton->vptr->update = pauseButtonUpdate;
}

static void pauseButtonUpdate(ActorNode button, double delta)
{
    double mouseX = inmng.mouseX;
    double mouseY = inmng.mouseY;

    charTexture *buttonTexture = (charTexture *)(button->getComponent(button, "pause_button_texture"));
    Vector pos = *(buttonTexture->getPos(buttonTexture));
    double width = buttonTexture->getWidth(buttonTexture);
    double height = buttonTexture->getHeight(buttonTexture);

    if (mouseX > pos.x - width / 2 && mouseX < pos.x + width / 2 && mouseY > pos.y - height / 2 && mouseY < pos.y + height / 2 ||
        inmng.keyStates[VK_ESCAPE])
    {
        if (inmng.mouseButtons[0] && inmng.mouseEventType == BUTTON_UP || inmng.keyStates[VK_ESCAPE])
        {
            buttonTexture->color = "Blue";
            if (scmng.getScene("scene1") == NULL)
                scmng.loadScene(&scene1, createScene1, "from pause");
            scmng.switchTo("scene1", FALSE, TRUE, "from pause", strlen("from pause") * sizeof(char));
            timer->stop(timer);
        }
        else if (inmng.mouseButtons[0] && inmng.mouseEventType == BUTTON_DOWN)
        {
            buttonTexture->color = "Blue";
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