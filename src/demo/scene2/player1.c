#include "player1.h"
#include "player2.h"
#include "room1_bkgnd.h"
#include "scene_info.h"

// 创建Player
Vector *playerPos;
ActorNode player;
// playerTexture
charTexture *playerTextureDown;
charTexture *playerTextureUp;
charTexture *playerTextureRight;
charTexture *playerTextureLeft;
// playerCollisionShape
Rect *collisionRect;
CollisionShape *playerCollisionShape;
Circle *targetCircle;
CollisionShape *playerDashTargetShape;
Vector *stripPos;
Rect *stripRect;
CollisionShape *playerDashPowerStrip;
Audio *bulletTimeSound;

ActorRender normalRender;
extern double GAME_TIME_TICK;
bool freezed = FALSE;

static double dashPower;
static bool skillEnabled = FALSE;
static void playerUpdate(ActorNode player, double delta);
static void playerSpecialRender(ActorNode player);

void createPlayer1()
{
    // player1
    playerPos = newVector(getww / 2, getwh / 2);
    player = newActor("player", playerPos);
    playerTextureDown = newcharTexture("./res/scene2/player_down.txt", playerPos, "White", 1);
    playerTextureUp = newcharTexture("./res/scene2/player_up.txt", playerPos, "White", 1);
    playerTextureRight = newcharTexture("./res/scene2/player_right.txt", playerPos, "White", 1);
    playerTextureLeft = newcharTexture("./res/scene2/player_left.txt", playerPos, "White", 1);
    collisionRect = newRect(
        playerPos,
        0,
        playerTextureDown->getWidth(playerTextureDown) * 0.8, playerTextureDown->getHeight(playerTextureDown) * 0.8,
        FALSE,
        "Red",
        1);
    playerCollisionShape = newCollisionShape((Shape *)collisionRect);
    targetCircle = newCircle(
        playerPos,
        0,
        playerTextureDown->getWidth(playerTextureDown) / 2.2,
        FALSE,
        "Cyan",
        1);
    playerDashTargetShape = newCollisionShape((Shape *)targetCircle);
    stripPos = newVector(playerPos->x, playerPos->y - playerTextureDown->getHeight(playerTextureDown) / 1.8);
    stripRect = newRect(
        stripPos,
        0,
        playerTextureDown->getWidth(playerTextureDown), playerTextureDown->getHeight(playerTextureDown) * 0.1,
        TRUE,
        "Green",
        1);
    playerDashPowerStrip = newCollisionShape((Shape *)stripRect);
    bulletTimeSound = newAudio("./res/scene2/bullet_time.mp3", FALSE);

    player->addComponent(player, (ComponentNode)playerTextureDown);
    player->addComponent(player, (ComponentNode)playerTextureUp);
    player->addComponent(player, (ComponentNode)playerTextureRight);
    player->addComponent(player, (ComponentNode)playerTextureLeft);
    player->addComponent(player, (ComponentNode)playerCollisionShape);
    player->addComponent(player, (ComponentNode)playerDashTargetShape);
    player->addComponent(player, (ComponentNode)playerDashPowerStrip);
    player->addComponent(player, (ComponentNode)bulletTimeSound);

    destoryVector(playerPos);
    destoryVector(stripPos);
    destoryShape((Shape *)collisionRect);
    destoryShape((Shape *)targetCircle);
    destoryShape((Shape *)stripRect);
}

void setupPlayer1(void *param)
{
    // player1
    playerTextureDown->visible = TRUE;
    playerTextureDown->setMeta((ComponentNode)playerTextureDown, "player_texture_down");
    playerTextureUp->visible = FALSE;
    playerTextureUp->setMeta((ComponentNode)playerTextureUp, "player_texture_up");
    playerTextureRight->visible = FALSE;
    playerTextureRight->setMeta((ComponentNode)playerTextureRight, "player_texture_right");
    playerTextureLeft->visible = FALSE;
    playerTextureLeft->setMeta((ComponentNode)playerTextureLeft, "player_texture_left");
    // collisionShape
    playerCollisionShape->visible = FALSE;
    playerCollisionShape->enable = TRUE;
    playerCollisionShape->setMeta((ComponentNode)playerCollisionShape, "player_collision_shape");
    // playerDashTargetShape
    playerDashTargetShape->visible = FALSE;
    playerDashTargetShape->enable = FALSE;
    playerDashTargetShape->setMeta((ComponentNode)playerDashTargetShape, "player_dash_circle");
    // dashPowerStrip
    playerDashPowerStrip->visible = FALSE;
    playerDashPowerStrip->enable = FALSE;
    playerDashPowerStrip->setMeta((ComponentNode)playerDashPowerStrip, "player_dash_power_strip");
    // Audio
    bulletTimeSound->setMeta((ComponentNode)bulletTimeSound, "bullet_time_sound");
    // update function
    player->vptr->update = playerUpdate;
    normalRender = player->vptr->render;

    
    if (param != NULL)
    {
        if (*((bool *)param) == TRUE)
        {
            ;
        }
        else
        {
            dashPower = 90;
            freezed = FALSE;
        }
    }
    else
    {
        dashPower = 90;
        freezed = FALSE;
    }
}

/// @brief 玩家更新方法
/// @param player type: ActorNode, 玩家Actor本身
/// @param delta  type: double, 游戏时间微分元
static void playerUpdate(ActorNode player, double delta)
{
    // 获取玩家在更新过过程中需要用到的组件
    charTexture *playerTextureDown = (charTexture *)(player->getComponent(player, "player_texture_down"));
    charTexture *playerTextureUp = (charTexture *)(player->getComponent(player, "player_texture_up"));
    charTexture *playerTextureRight = (charTexture *)(player->getComponent(player, "player_texture_right"));
    charTexture *playerTextureLeft = (charTexture *)(player->getComponent(player, "player_texture_left"));
    CollisionShape *playerCollisionShape = (CollisionShape *)(player->getComponent(player, "player_collision_shape"));
    CollisionShape *playerDashTargetCircle = (CollisionShape *)(player->getComponent(player, "player_dash_circle"));
    CollisionShape *playerDashPowerStrip = (CollisionShape *)(player->getComponent(player, "player_dash_power_strip"));
    Audio *bulletTimeSound = (Audio *)(player->getComponent(player, "bullet_time_sound"));
    // 部分量的定义
    static bool enterPressed = FALSE;
    double ACC_CONST = 10;
    double VEL_CONST = 5;

    if (freezed)
    {
        ACC_CONST = 4;
        VEL_CONST = 5 * 0.4;
    }

    // 键盘控制人物移动部分
    Vector *acc = newVector((-inmng.keyStates[VK_LEFT] + inmng.keyStates[VK_RIGHT]),
                            (-inmng.keyStates[VK_DOWN] + inmng.keyStates[VK_UP]));

    acc->normalize(acc);
    acc->mult(acc, delta * ACC_CONST);
    Vector vel;
    if (!(inmng.keyStates[VK_RETURN] && player->vel.length(&(player->vel)) <= 1.5 && !freezed && dashPower >= 45 && skillEnabled))
    {
        player->vel.add(&(player->vel), acc);
    }
    memcpy(&(vel), &(player->vel), sizeof(Vector));
    vel.mult(&(vel), delta * VEL_CONST);

    //在处理完玩家速度后，叠加到玩家位置之前做碰撞处理
    Vector *collisionVector;
    if ((collisionVector = player->isCollideWithActor(player, room1Background)) != NULL)
    {
        CollisionShape *icePointCollisionShape = (CollisionShape*)room1Background->getComponent(room1Background, "room1_skillpoint_ice");
        CollisionShape *lighteningPointCollisionShape = (CollisionShape*)room1Background->getComponent(room1Background, "room1_skillpoint_lightening");
        if (playerCollisionShape->isCollideWith(playerCollisionShape, lighteningPointCollisionShape)){
            skillEnabled = TRUE;
        }
        else if (!playerCollisionShape->isCollideWith(playerCollisionShape, icePointCollisionShape)){
            double collisionVectorAngle = collisionVector->getAngle(collisionVector);
            vel.rotate(&(vel), -collisionVectorAngle);
            vel.x = vel.x < 0 ? 0 : vel.x;
            vel.rotate(&(vel), collisionVectorAngle);
        }

    }
    destoryVector(collisionVector);
    if (skillEnabled){
        playerDashPowerStrip->visible = TRUE;
    }

    player->pos.add(&(player->pos), &(vel));
    if (!(inmng.keyStates[VK_RETURN] && player->vel.length(&(player->vel)) <= 1.5 && !freezed && dashPower >= 45 && skillEnabled))
        player->vel.mult(&(player->vel), 0.9);

    if (fabs(player->vel.x) <= 0.1)
        player->vel.x = 0;
    if (fabs(player->vel.y) <= 0.1)
        player->vel.y = 0;

    // 处理人物移动出边界的情况
    if (player->pos.x < 0)
        player->pos.x += getww;
    if (player->pos.y < 0)
        player->pos.y += getwh;
    if (player->pos.x > getww)
        player->pos.x -= getww;
    if (player->pos.y > getwh)
        player->pos.y -= getwh;

    // 处理人物不同方向的纹理展示
    if (player->vel.length(&(player->vel)) == 0)
    {
        playerTextureDown->visible = TRUE;
        playerTextureUp->visible = FALSE;
        playerTextureLeft->visible = FALSE;
        playerTextureRight->visible = FALSE;
    }
    else
    {
        if (player->vel.y > 0)
        {
            playerTextureDown->visible = FALSE;
            playerTextureUp->visible = TRUE;
            playerTextureLeft->visible = FALSE;
            playerTextureRight->visible = FALSE;
        }
        else
        {
            playerTextureDown->visible = TRUE;
            playerTextureUp->visible = FALSE;
            playerTextureLeft->visible = FALSE;
            playerTextureRight->visible = FALSE;
        }

        if (player->vel.x > 0 && fabs(player->vel.x) > 0.6 * fabs(player->vel.y))
        {
            playerTextureDown->visible = FALSE;
            playerTextureUp->visible = FALSE;
            playerTextureLeft->visible = FALSE;
            playerTextureRight->visible = TRUE;
        }
        else if (player->vel.x < 0 && fabs(player->vel.x) > 0.6 * fabs(player->vel.y))
        {
            playerTextureDown->visible = FALSE;
            playerTextureUp->visible = FALSE;
            playerTextureLeft->visible = TRUE;
            playerTextureRight->visible = FALSE;
        }
    }

    charTexture *playerTexture;
    if (playerTextureDown->visible)
        playerTexture = playerTextureDown;
    else if (playerTextureUp->visible)
        playerTexture = playerTextureUp;
    else if (playerTextureLeft->visible)
        playerTexture = playerTextureLeft;
    else
        playerTexture = playerTextureRight;

    if (freezed)
    {
        playerTexture->color = "Blue";
    }

    // ENTER 键技能

    Vector *dashDirection = newVector(inmng.mouseX - player->pos.x, inmng.mouseY - player->pos.y);
    if (dashDirection->length(dashDirection) > 5)
    {
        dashDirection->normalize(dashDirection);
        dashDirection->mult(dashDirection, 5);
    }
    if (inmng.keyStates[VK_RETURN] && player->vel.length(&(player->vel)) <= 1.5 && !freezed && dashPower >= 45 && skillEnabled)
    {
        if (!bulletTimeSound->playing)
            bulletTimeSound->play(bulletTimeSound);

        player->vptr->render = playerSpecialRender;

        GAME_TIME_TICK = 1.0 / 500.0;
        enterPressed = TRUE;
        playerDashTargetCircle->pos.x = player->pos.x + dashDirection->x;
        playerDashTargetCircle->pos.y = player->pos.y + dashDirection->y;
        playerDashTargetCircle->visible = TRUE;
        playerDashTargetCircle->super.vptr->update((ComponentNode)playerDashTargetCircle, &(playerDashTargetCircle->pos));
    }
    else
    {
        GAME_TIME_TICK = 1.0 / 60.0;
        player->vptr->render = normalRender;
        if (enterPressed && inmng.keyBoardEventType == KEY_UP && !freezed)
        {
            enterPressed = FALSE;
            playerDashTargetCircle->visible = FALSE;
            if (dashPower >= 10)
            {
                dashPower = dashPower - 40 > 0 ? dashPower - 40 : 0;
                player->vel.x = dashDirection->x;
                player->vel.y = dashDirection->y;
                Vector vel;
                memcpy(&(vel), &(player->vel), sizeof(Vector));
                vel.mult(&(vel), delta * VEL_CONST * 25);
                player->pos.add(&(player->pos), &(vel));
            }
        }
        if (freezed)
        {
            enterPressed = FALSE;
            playerDashTargetCircle->visible = FALSE;
            bulletTimeSound->stop(bulletTimeSound);
        }
    }
    // 技能条的长度更新
    dashPower = dashPower < 90.0 ? dashPower + 4 * delta : 90.0;
    ((Rect *)(playerDashPowerStrip->shape))->width = (dashPower / 90.0) * playerTextureDown->getWidth(playerTextureDown);
    //printf("\nLOG:\nplayerDashPowerStrip->shape->width is: %lf\n", ((Rect *)(playerDashPowerStrip->shape))->width);
    // 人物纹理颜色与可见性改变
    if (player->vel.length(&(player->vel)) >= 1.5 && !freezed)
    {
        playerTexture->color = "White";
        if (player->vel.length(&(player->vel)) >= 2.7)
        {
            playerTexture->visible = FALSE;
        }
        else
        {
            playerTexture->visible = TRUE;
        }
    }
    else if (!inmng.keyStates[VK_RETURN] && !freezed)
    {
        playerTexture->color = "Red";
    }
    // 人物冲刺时碰撞箱无效
    if (player->vel.length(&(player->vel)) >= 1.5)
    {
        playerCollisionShape->enable = FALSE;
    }
    else
    {
        playerCollisionShape->enable = TRUE;
    }

    // 根据人物位置更新各个组件的位置
    playerCollisionShape->super.vptr->update((ComponentNode)playerCollisionShape, &(player->pos));
    playerTexture->super.vptr->update((ComponentNode)playerTexture, &(player->pos));
    Vector *stripPos = newVector(player->pos.x, player->pos.y + playerTexture->getHeight(playerTexture) * 0.8);
    playerDashPowerStrip->super.vptr->update((ComponentNode)playerDashPowerStrip, stripPos);
    //printf("\nLOG:\nplayerDashPowerStrip->shape->width is: %lf\n", ((Rect *)(playerDashPowerStrip->shape))->width);
    // 碰撞检测
    if (player->isCollideWithActor(player, player2))
    {
        GAME_TIME_TICK = 1.0 / 600.0;
        char param[255] = "ChaserWin";
        if (scmng.getScene("scene3") == NULL)
        {
            scmng.loadScene(&scene3, createScene3, (void *)param);
        }
        scmng.switchTo("scene3", TRUE, TRUE, (void *)param, strlen(param) * sizeof(char));
    }

    destoryVector(stripPos);
    // 销毁所有申请的，级别在Component以下的内存空间
    destoryVector(acc);
    destoryVector(dashDirection);
}

static void playerSpecialRender(ActorNode player)
{
    ComponentNode currentComp = player->componentList;
    while (currentComp && currentComp->vptr != NULL && currentComp->vptr->render != 0)
    {
        currentComp->vptr->render(currentComp);
        currentComp = currentComp->next;
    }

    charTexture *playerTextureDown = (charTexture *)(player->getComponent(player, "player_texture_down"));
    charTexture *playerTextureUp = (charTexture *)(player->getComponent(player, "player_texture_up"));
    charTexture *playerTextureRight = (charTexture *)(player->getComponent(player, "player_texture_right"));
    charTexture *playerTextureLeft = (charTexture *)(player->getComponent(player, "player_texture_left"));
    charTexture *playerTexture;
    if (playerTextureDown->visible)
        playerTexture = playerTextureDown;
    else if (playerTextureUp->visible)
        playerTexture = playerTextureUp;
    else if (playerTextureLeft->visible)
        playerTexture = playerTextureLeft;
    else
        playerTexture = playerTextureRight;

    Vector originPos;
    memcpy(&(originPos), &(playerTexture->pos), sizeof(Vector));

    static double bias = 0;
    double temp = ((rand() % (5 - 0 + 1)) + 0) / 50.0;
    bias = abs(bias - temp) > 0.01 && abs(bias - temp) < 0.03 ? temp : bias;
    playerTexture->pos.x -= bias;
    playerTexture->color = "Cyan";
    playerTexture->super.vptr->render((ComponentNode)playerTexture);

    bias = ((rand() % (5 - 0 + 1)) + 0) / 30.0;
    playerTexture->pos.x += bias;
    playerTexture->color = "Red";
    playerTexture->super.vptr->render((ComponentNode)playerTexture);

    playerTexture->pos.x = originPos.x;
    playerTexture->pos.y = originPos.y;
    playerTexture->color = "White";
}