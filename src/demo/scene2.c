#include "2DEngine.h"
#include "scene2.h"

SceneNode scene2;
static void backButtonUpdate(ActorNode button, double delta);
static void playerUpdate(ActorNode player, double delta);

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

    Vector *backButtonPos = newVector(getww/5, getwh*4/5);
    ActorNode backButton = newActor("back_button", backButtonPos);
    Texture *backButtonTexture = newTexture("./res/scene2/back_button_texture.txt", backButtonPos, "Blue", 3);
    backButtonTexture->visible = TRUE;
    backButtonTexture->setMeta((ComponentNode)backButtonTexture, "back_button_texture");
    backButton->addComponent(backButton, (ComponentNode)backButtonTexture);
    backButton->vptr->update = backButtonUpdate;
    destoryVector(backButtonPos);

    Vector *playerPos = newVector(getww/2, getwh/2);
    ActorNode player = newActor("player", playerPos);
    Texture *playerTexture = newTexture("./res/scene2/playerBody.txt", playerPos, "Blue", 1);
    playerTexture->visible = TRUE;
    playerTexture->setMeta((ComponentNode)playerTexture, "player_texture");
    player->addComponent(player, (ComponentNode)playerTexture);
    player->vptr->update = playerUpdate;
    destoryVector(playerPos);

    scene2->addActor(scene2, background);
    scene2->addActor(scene2, backButton);
    scene2->addActor(scene2, player);
}

extern double GAME_TIME_TICK;

static void playerUpdate(ActorNode player, double delta) {

    if (inmng.keyStates['T']){
        GAME_TIME_TICK = 1.0/ 600.0;
    }
    else{
        GAME_TIME_TICK = 1.0 / 60.0;
    }

    Vector *acc = newVector((-inmng.keyStates['A'] + inmng.keyStates['D']),
                            (-inmng.keyStates['S'] + inmng.keyStates['W']));

    double ACC_CONST = 5;

    acc->normalize(acc);
    acc->mult(acc, delta * ACC_CONST);
    player->vel.add(&(player->vel), acc);
    player->pos.add(&(player->pos), player->getVel(player));
    player->vel.mult(&(player->vel), 0.6);

    Texture* playerTexture = (Texture*)(player->getComponent(player, "player_texture"));
    if (playerTexture != NULL){
        playerTexture->setPos(playerTexture, player->getPos(player));
    }
    else{
        printf("Get Texture failed\n");
        return;
    }
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