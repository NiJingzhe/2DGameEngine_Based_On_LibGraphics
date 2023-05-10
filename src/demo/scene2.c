#include "2DEngine.h"
#include "scene2.h"

SceneNode scene2;
static void backButtonUpdate(ActorNode button, double delta);

void setupScene_scene2(){

    scene2 = newScene("scene2");

    Vector *backgroundPos = newVector(getww/2, getwh/2);
    Rect *backgroundRect = newRect(backgroundPos, 0, getww, getwh, TRUE, "White", 1);
    ActorNode background = newActor("background", backgroundPos);
    CollisionShape *backgroundShape = newCollisionShape((Shape*)backgroundRect);
    backgroundShape->enable = FALSE;
    backgroundShape->visible = TRUE;
    backgroundShape->setMeta((ComponentNode)backgroundShape, "backgroundShape");
    background->addComponent(background, (ComponentNode)backgroundShape);
    destoryVector(backgroundPos);
    destoryShape((Shape*)backgroundRect);

    Vector *backButtonPos = newVector(getww/2, getwh/3);
    ActorNode backButton = newActor("start_button", backButtonPos);
    Texture *backButtonTexture = newTexture("./res/scene2/back_button_texture.txt", backButtonPos, "Blue", 8);
    backButtonTexture->visible = TRUE;
    backButtonTexture->setMeta((ComponentNode)backButtonTexture, "back_button_texture");
    backButton->addComponent(backButton, (ComponentNode)backButtonTexture);
    backButton->vptr->update = backButtonUpdate;
    destoryVector(backButtonPos);

    scene2->addActor(scene2, background);
    scene2->addActor(scene2, backButton);
}

static void backButtonUpdate(ActorNode button, double delta) {
    double mouseX = inmng.mouseX;
    double mouseY = inmng.mouseY;

    Texture *buttonTexture = (Texture*)(button->getComponent(button, "back_button_texture"));
    Vector pos = *(buttonTexture->getPos(buttonTexture));
    double width = buttonTexture->getWidth(buttonTexture);
    double height = buttonTexture->getHeight(buttonTexture);
    
    if (mouseX > pos.x - width / 2 && mouseX < pos.x + width / 2 && mouseY > pos.y - height / 2 && mouseY < pos.y + height / 2) {
        if (inmng.mouseButtons[0] && inmng.mouseEventType == BUTTON_UP){
            buttonTexture->color = "Blue";
            scene2->setSwitchTarget(scene2, "scene1");
        }else if(inmng.mouseButtons[0] && inmng.mouseEventType == BUTTON_DOWN){
            buttonTexture->color = "Blue";
        }
        else{
            buttonTexture->color = "Green";
        }
    }
    else {
        buttonTexture->color = "Red";
    }
}