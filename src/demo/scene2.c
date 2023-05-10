#include "2DEngine.h"
#include "scene2.h"

SceneNode scene2;
static void pauseButtonUpdate(ActorNode button, double delta);
static void playerUpdate(ActorNode player, double delta);

void setupScene_scene2()
{

    scene2 = newScene("scene2");

    Vector *backgroundPos = newVector(getww / 2, getwh / 2);
    Rect *backgroundRect = newRect(backgroundPos, 0, getww, getwh, TRUE, "Black", 1);
    ActorNode background = newActor("background", backgroundPos);
    CollisionShape *backgroundShape = newCollisionShape((Shape *)backgroundRect);
    backgroundShape->enable = FALSE;
    backgroundShape->visible = TRUE;
    backgroundShape->setMeta((ComponentNode)backgroundShape, "backgroundShape");
    background->addComponent(background, (ComponentNode)backgroundShape);
    destoryVector(backgroundPos);
    destoryShape((Shape *)backgroundRect);

    Vector *pauseButtonPos = newVector(getww / 12, getwh * 7 / 8);
    ActorNode pauseButton = newActor("back_button", pauseButtonPos);
    Texture *pauseButtonTexture = newTexture("./res/scene2/pause_button_texture.txt", pauseButtonPos, "White", 1);
    pauseButtonTexture->visible = TRUE;
    pauseButtonTexture->setMeta((ComponentNode)pauseButtonTexture, "pause_button_texture");
    pauseButton->addComponent(pauseButton, (ComponentNode)pauseButtonTexture);
    pauseButton->vptr->update = pauseButtonUpdate;
    destoryVector(pauseButtonPos);

    Vector *playerPos = newVector(getww / 2, getwh / 2);
    ActorNode player = newActor("player", playerPos);
    // playerTexture
    Texture *playerTexture = newTexture("./res/scene2/playerBody.txt", playerPos, "Cyan", 1);
    playerTexture->visible = TRUE;
    playerTexture->setMeta((ComponentNode)playerTexture, "player_texture");
    player->addComponent(player, (ComponentNode)playerTexture);
    // playerCollisionShape
    Rect *collisionRect = newRect(
        playerPos,
        0,
        playerTexture->getWidth(playerTexture), playerTexture->getHeight(playerTexture),
        FALSE,
        "Red",
        1);
    CollisionShape *playerCollisionShape = newCollisionShape((Shape *)collisionRect);
    playerCollisionShape->visible = FALSE;
    playerCollisionShape->enable = TRUE;
    playerCollisionShape->setMeta((ComponentNode)playerCollisionShape, "player_collision_shape");
    player->addComponent(player, (ComponentNode)playerCollisionShape);
    // playerDashTargetShape
    Circle *targetCircle = newCircle(
        playerPos,
        0,
        playerTexture->getWidth(playerTexture) / 2.2,
        FALSE,
        "Cyan",
        1);
    CollisionShape *playerDashTargetShape = newCollisionShape((Shape *)targetCircle);
    playerDashTargetShape->visible = FALSE;
    playerDashTargetShape->enable = FALSE;
    playerDashTargetShape->setMeta((ComponentNode)playerDashTargetShape, "player_dash_circle");
    player->addComponent(player, (ComponentNode)playerDashTargetShape);
    // dashPowerStrip
    Vector *stripPos = newVector(playerPos->x, playerPos->y - playerTexture->getHeight(playerTexture) / 1.8);
    Rect *stripRect = newRect(
        stripPos,
        0,
        playerTexture->getWidth(playerTexture), playerTexture->getHeight(playerTexture) * 0.1,
        TRUE,
        "Green",
        1);
    CollisionShape *playerDashPowerStrip = newCollisionShape((Shape *)stripRect);
    playerDashPowerStrip->visible = TRUE;
    playerDashPowerStrip->enable = FALSE;
    playerDashPowerStrip->setMeta((ComponentNode)playerDashPowerStrip, "player_dash_power_strip");
    player->addComponent(player, (ComponentNode)playerDashPowerStrip);

    player->vptr->update = playerUpdate;
    destoryVector(playerPos);
    destoryVector(stripPos);
    destoryShape((Shape *)collisionRect);
    destoryShape((Shape *)targetCircle);
    destoryShape((Shape *)stripRect);

    scene2->addActor(scene2, background);
    scene2->addActor(scene2, pauseButton);
    scene2->addActor(scene2, player);
}

extern double GAME_TIME_TICK;

static void playerUpdate(ActorNode player, double delta)
{
    Texture *playerTexture = (Texture *)(player->getComponent(player, "player_texture"));
    CollisionShape *playerCollisionShape = (CollisionShape *)(player->getComponent(player, "player_collision_shape"));
    CollisionShape *playerDashTargetCircle = (CollisionShape *)(player->getComponent(player, "player_dash_circle"));
    CollisionShape *playerDashPowerStrip = (CollisionShape *)(player->getComponent(player, "player_dash_power_strip"));

    static bool rPressed = FALSE;
    static double dashPower = 90;
    Vector *dashDirection = newVector(inmng.mouseX - player->pos.x, inmng.mouseY - player->pos.y);
    if (dashDirection->length(dashDirection) > 5)
    {
        dashDirection->normalize(dashDirection);
        dashDirection->mult(dashDirection, 5);
    }
    if (inmng.keyStates['R'])
    {
        GAME_TIME_TICK = 1.0 / 400.0;
        rPressed = TRUE;
        playerDashTargetCircle->pos.x = player->pos.x + dashDirection->x;
        playerDashTargetCircle->pos.y = player->pos.y + dashDirection->y;
        playerDashTargetCircle->visible = TRUE;
        playerDashTargetCircle->super.vptr->update((ComponentNode)playerDashTargetCircle, &(playerDashTargetCircle->pos));
    }
    else
    {
        if (rPressed && inmng.keyBoardEventType == KEY_UP && dashPower >= 30)
        {
            rPressed = FALSE;
            dashPower -= 40;
            player->pos.add(&(player->pos), dashDirection);
            playerDashTargetCircle->visible = FALSE;
            player->vel.x = dashDirection->x * 0.08;
            player->vel.y = dashDirection->y * 0.08;
        }
        else
        {
            GAME_TIME_TICK = 1.0 / 60.0;
        }
    }
    dashPower = dashPower < 90.0 ? dashPower + 50 * delta : 90.0;
    ((Rect *)(playerDashPowerStrip->shape))->width = (dashPower / 90.0) * playerTexture->getWidth(playerTexture);

    Vector *acc = newVector((-inmng.keyStates['A'] + inmng.keyStates['D']),
                            (-inmng.keyStates['S'] + inmng.keyStates['W']));

    double ACC_CONST = 10;
    double VEL_CONST = 5;

    acc->normalize(acc);
    acc->mult(acc, delta * ACC_CONST);
    Vector vel;
    if (!inmng.keyStates['R'])
    {
        player->vel.add(&(player->vel), acc);
    }
    memcpy(&(vel), &(player->vel), sizeof(Vector));
    vel.mult(&(vel), delta * VEL_CONST);
    player->pos.add(&(player->pos), &(vel));
    playerCollisionShape->super.vptr->update((ComponentNode)playerCollisionShape, &(player->pos));
    Vector *stripPos = newVector(player->pos.x, player->pos.y - playerTexture->getHeight(playerTexture) / 1.5);
    playerTexture->super.vptr->update((ComponentNode)playerTexture, stripPos);
    destoryVector(stripPos);
    playerDashPowerStrip->super.vptr->update((ComponentNode)playerDashPowerStrip, &(player->pos));
    if (!inmng.keyStates['R'])
        player->vel.mult(&(player->vel), 0.9);

    destoryVector(acc);
    destoryVector(dashDirection);
}

static void pauseButtonUpdate(ActorNode button, double delta)
{
    double mouseX = inmng.mouseX;
    double mouseY = inmng.mouseY;

    Texture *buttonTexture = (Texture *)(button->getComponent(button, "pause_button_texture"));
    Vector pos = *(buttonTexture->getPos(buttonTexture));
    double width = buttonTexture->getWidth(buttonTexture);
    double height = buttonTexture->getHeight(buttonTexture);

    if (mouseX > pos.x - width / 2 && mouseX < pos.x + width / 2 && mouseY > pos.y - height / 2 && mouseY < pos.y + height / 2 ||
        inmng.keyStates[VK_ESCAPE])
    {
        if (inmng.mouseButtons[0] && inmng.mouseEventType == BUTTON_UP || inmng.keyStates[VK_ESCAPE])
        {
            buttonTexture->color = "Blue";
            scene2->setSwitchTarget(scene2, "scene1");
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