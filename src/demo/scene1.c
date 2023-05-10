#include "2DEngine.h"
#include "scene1.h"

SceneNode scene1;
static void startButtonUpdate(ActorNode button, double delta);

void setupScene_scene1(){

    scene1 = newScene("scene1");

    Vector *backgroundPos = newVector(getww/2, getwh/2);
    Rect *backgroundRect = newRect(backgroundPos, 0, getww, getwh, TRUE, "Black", 1);
    ActorNode background = newActor("background", backgroundPos);
    CollisionShape *backgroundShape = newCollisionShape((Shape*)backgroundRect);
    backgroundShape->enable = FALSE;
    backgroundShape->visible = TRUE;
    backgroundShape->setMeta((ComponentNode)backgroundShape, "backgroundShape");
    background->addComponent(background, (ComponentNode)backgroundShape);
    destoryVector(backgroundPos);
    destoryShape((Shape*)backgroundRect);
        

    Vector *titlePos = newVector(getww/2, getwh*3/4);
    ActorNode title = newActor("startup_title", titlePos);
    Texture *titleTexture = newTexture("./res/scene1/title_texture.txt", titlePos, "Red", 18);
    titleTexture->visible = TRUE;
    titleTexture->setMeta((ComponentNode)titleTexture, "title_texture");
    title->addComponent(title, (ComponentNode)titleTexture);
    destoryVector(titlePos);

    Vector *startButtonPos = newVector(getww/2, getwh/3);
    ActorNode startButton = newActor("start_button", startButtonPos);
    Texture *startButtonTexture = newTexture("./res/scene1/start_button_texture.txt", startButtonPos, "Red", 8);
    startButtonTexture->visible = TRUE;
    startButtonTexture->setMeta((ComponentNode)startButtonTexture, "start_button_texture");
    startButton->addComponent(startButton, (ComponentNode)startButtonTexture);
    startButton->vptr->update = startButtonUpdate;
    destoryVector(startButtonPos);

    scene1->addActor(scene1, background);
    scene1->addActor(scene1, title);
    scene1->addActor(scene1, startButton);
}

static void startButtonUpdate(ActorNode button, double delta) {
    double mouseX = inmng.mouseX;
    double mouseY = inmng.mouseY;

    Texture *buttonTexture = (Texture*)(button->getComponent(button, "start_button_texture"));
    Vector pos = *(buttonTexture->getPos(buttonTexture));
    double width = buttonTexture->getWidth(buttonTexture);
    double height = buttonTexture->getHeight(buttonTexture);
    
    if (mouseX > pos.x - width / 2 && mouseX < pos.x + width / 2 && mouseY > pos.y - height / 2 && mouseY < pos.y + height / 2) {
        if (inmng.mouseButtons[0] && inmng.mouseEventType == BUTTON_UP){
            scene1->setSwitchTarget(scene1, "scene2");
        }
        else if(inmng.mouseButtons[0] && inmng.mouseEventType == BUTTON_DOWN){
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