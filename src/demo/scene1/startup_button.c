#include "2DEngine.h"
#include "scene_info.h"
#include "startup_button.h"

Vector *startButtonPos;
ActorNode startButton;
picTexture *startButtonTexture;

extern bool fromPause;

void createStartupButton()
{

    DefineColor("LightBlue", to255(111), to255(160), to255(255));
    DefineColor("DeepPink", to255(242), to255(104), to255(223));
    DefineColor("BrighterPink", to255(255), to255(43), to255(201));
    DefineColor("DeepBlue", to255(64), to255(31), to255(247));
    startButtonPos = newVector(getww / 2, getwh * 2 / 10);
    startButton = newActor("start_button", startButtonPos);
    startButtonTexture = newPicTexture("./res/scene1/startupButton.bmp", startButtonPos, 2, 1.5);
    startButton->addComponent(startButton, (ComponentNode)startButtonTexture);
    destoryVector(startButtonPos);
}

void setupStartupButton(void *param)
{
    startButtonTexture->visible = TRUE;
    startButtonTexture->setMeta((ComponentNode)startButtonTexture, "start_button_texture");

    startButton->vptr->update = startButtonUpdate;
}

void startButtonUpdate(ActorNode button, double delta)
{
    double mouseX = inmng.mouseX;
    double mouseY = inmng.mouseY;

    Vector pos = *(startButtonTexture->getPos(startButtonTexture));
    double width = startButtonTexture->getWidth(startButtonTexture);
    double height = startButtonTexture->getHeight(startButtonTexture);

    if (mouseX > pos.x - width / 2 && mouseX < pos.x + width / 2 && mouseY > pos.y - height / 2 && mouseY < pos.y + height / 2)
    {
        if (inmng.mouseButtons[0] && inmng.mouseEventType == BUTTON_UP)
        {
            if (scmng.getScene("scene2") == NULL)
                scmng.loadScene(&scene2, createScene2, &fromPause);
            scmng.switchTo("scene2", FALSE, TRUE, &fromPause, sizeof(bool));
        }
    }

}