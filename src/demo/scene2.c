#include "2DEngine.h"
#include "scene_info.h"
/*---------TODO---------
1. 完成场景2的构建
2. 实现平滑人物控制器                         finished
3. 实现人物技能与技能槽机制                   finished
4. 完成敌人NPC
5. 为人物技能释放过程增加一定的视觉效果
---------END TODO-------*/

SceneNode scene2;

Vector *scene2BackgoundPos;
Rect *scene2BackgoundRect;
ActorNode scene2Backgound;
CollisionShape *scene2BackgoundShape;

Vector *pauseButtonPos;
ActorNode pauseButton;
Texture *pauseButtonTexture;

// 创建Player
Vector *playerPos;
ActorNode player;
// playerTexture
Texture *playerTextureDown;
Texture *playerTextureUp;
Texture *playerTextureRight;
Texture *playerTextureLeft;
// playerCollisionShape
Rect *collisionRect;
CollisionShape *playerCollisionShape;
Circle *targetCircle;
CollisionShape *playerDashTargetShape;
Vector *stripPos;
Rect *stripRect;
CollisionShape *playerDashPowerStrip;
Audio *bulletTimeSound;

static void pauseButtonUpdate(ActorNode button, double delta);
static void playerUpdate(ActorNode player, double delta);
static void setupScene_scene2(SceneNode scene2, void *param);

void createScene2(SceneNode *scene2)
{

    *scene2 = newScene("scene2");
    (*scene2)->setup = setupScene_scene2;

    scene2BackgoundPos = newVector(getww / 2, getwh / 2);
    scene2BackgoundRect = newRect(scene2BackgoundPos, 0, getww, getwh, TRUE, "Black", 1);
    scene2Backgound = newActor("scene2Backgound", scene2BackgoundPos);
    scene2BackgoundShape = newCollisionShape((Shape *)scene2BackgoundRect);

    pauseButtonPos = newVector(getww / 12, getwh * 7 / 8);
    pauseButton = newActor("pause_button", pauseButtonPos);
    pauseButtonTexture = newTexture("./res/scene2/pause_button_texture.txt", pauseButtonPos, "White", 1);

    playerPos = newVector(getww / 2, getwh / 2);
    player = newActor("player", playerPos);
    playerTextureDown = newTexture("./res/scene2/player_down.txt", playerPos, "Cyan", 1);
    playerTextureUp = newTexture("./res/scene2/player_up.txt", playerPos, "Cyan", 1);
    playerTextureRight = newTexture("./res/scene2/player_right.txt", playerPos, "Cyan", 1);
    playerTextureLeft = newTexture("./res/scene2/player_left.txt", playerPos, "Cyan", 1);

    collisionRect = newRect(
        playerPos,
        0,
        playerTextureDown->getWidth(playerTextureDown), playerTextureDown->getHeight(playerTextureDown),
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

    scene2Backgound->addComponent(scene2Backgound, (ComponentNode)scene2BackgoundShape);
    pauseButton->addComponent(pauseButton, (ComponentNode)pauseButtonTexture);
    player->addComponent(player, (ComponentNode)playerTextureDown);
    player->addComponent(player, (ComponentNode)playerTextureUp);
    player->addComponent(player, (ComponentNode)playerTextureRight);
    player->addComponent(player, (ComponentNode)playerTextureLeft);
    player->addComponent(player, (ComponentNode)playerCollisionShape);
    player->addComponent(player, (ComponentNode)playerDashTargetShape);
    player->addComponent(player, (ComponentNode)playerDashPowerStrip);
    player->addComponent(player, (ComponentNode)bulletTimeSound);
    // 添加Actor到场景
    (*scene2)->addActor((*scene2), scene2Backgound);
    (*scene2)->addActor((*scene2), pauseButton);
    (*scene2)->addActor((*scene2), player);

    destoryVector(scene2BackgoundPos);
    destoryShape((Shape *)scene2BackgoundRect);
    destoryVector(pauseButtonPos);
    destoryVector(playerPos);
    destoryVector(stripPos);
    destoryShape((Shape *)collisionRect);
    destoryShape((Shape *)targetCircle);
    destoryShape((Shape *)stripRect);
}

static void setupScene_scene2(SceneNode scene2, void *param)
{

    // 创建背景Actor
    scene2BackgoundShape->enable = FALSE;
    scene2BackgoundShape->visible = TRUE;
    scene2BackgoundShape->setMeta((ComponentNode)scene2BackgoundShape, "scene2BackgoundShape");

    // 创建暂停按钮Actor

    pauseButtonTexture->visible = TRUE;
    pauseButtonTexture->setMeta((ComponentNode)pauseButtonTexture, "pause_button_texture");
    pauseButton->vptr->update = pauseButtonUpdate;

    playerTextureDown->visible = TRUE;
    playerTextureDown->setMeta((ComponentNode)playerTextureDown, "player_texture_down");
    playerTextureUp->visible = FALSE;
    playerTextureUp->setMeta((ComponentNode)playerTextureUp, "player_texture_up");
    playerTextureRight->visible = FALSE;
    playerTextureRight->setMeta((ComponentNode)playerTextureRight, "player_texture_right");
    playerTextureLeft->visible = FALSE;
    playerTextureLeft->setMeta((ComponentNode)playerTextureLeft, "player_texture_left");

    playerCollisionShape->visible = FALSE;
    playerCollisionShape->enable = TRUE;
    playerCollisionShape->setMeta((ComponentNode)playerCollisionShape, "player_collision_shape");

    // playerDashTargetShape

    playerDashTargetShape->visible = FALSE;
    playerDashTargetShape->enable = FALSE;
    playerDashTargetShape->setMeta((ComponentNode)playerDashTargetShape, "player_dash_circle");

    // dashPowerStrip

    playerDashPowerStrip->visible = TRUE;
    playerDashPowerStrip->enable = FALSE;
    playerDashPowerStrip->setMeta((ComponentNode)playerDashPowerStrip, "player_dash_power_strip");

    // Audio

    bulletTimeSound->setMeta((ComponentNode)bulletTimeSound, "bullet_time_sound");

    player->vptr->update = playerUpdate;
}

extern double GAME_TIME_TICK;

static void playerUpdate(ActorNode player, double delta)
{
    // 获取玩家在更新过过程中需要用到的组件
    Texture *playerTextureDown = (Texture *)(player->getComponent(player, "player_texture_down"));
    Texture *playerTextureUp = (Texture *)(player->getComponent(player, "player_texture_up"));
    Texture *playerTextureRight = (Texture *)(player->getComponent(player, "player_texture_right"));
    Texture *playerTextureLeft = (Texture *)(player->getComponent(player, "player_texture_left"));
    CollisionShape *playerCollisionShape = (CollisionShape *)(player->getComponent(player, "player_collision_shape"));
    CollisionShape *playerDashTargetCircle = (CollisionShape *)(player->getComponent(player, "player_dash_circle"));
    CollisionShape *playerDashPowerStrip = (CollisionShape *)(player->getComponent(player, "player_dash_power_strip"));
    Audio *bulletTimeSound = (Audio *)(player->getComponent(player, "bullet_time_sound"));
    // 部分量的定义
    static bool rPressed = FALSE;
    static double dashPower = 90;
    double ACC_CONST = 10;
    double VEL_CONST = 5;

    // 键盘控制人物移动部分
    Vector *acc = newVector((-inmng.keyStates['A'] + inmng.keyStates['D']),
                            (-inmng.keyStates['S'] + inmng.keyStates['W']));

    acc->normalize(acc);
    acc->mult(acc, delta * ACC_CONST);
    Vector vel;
    if (!inmng.keyStates['R'] || player->vel.length(&(player->vel)) >= 2.0)
    {
        player->vel.add(&(player->vel), acc);
    }
    memcpy(&(vel), &(player->vel), sizeof(Vector));
    vel.mult(&(vel), delta * VEL_CONST);
    player->pos.add(&(player->pos), &(vel));
    if (!inmng.keyStates['R'] || player->vel.length(&(player->vel)) >= 2.0)
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

    // R键技能
    Vector *dashDirection = newVector(inmng.mouseX - player->pos.x, inmng.mouseY - player->pos.y);
    if (dashDirection->length(dashDirection) > 5)
    {
        dashDirection->normalize(dashDirection);
        dashDirection->mult(dashDirection, 5);
    }
    if (inmng.keyStates['R'] && player->vel.length(&(player->vel)) <= 2.0)
    {
        if (!bulletTimeSound->playing)
            bulletTimeSound->play(bulletTimeSound);

        GAME_TIME_TICK = 1.0 / 500.0;
        rPressed = TRUE;
        playerDashTargetCircle->pos.x = player->pos.x + dashDirection->x;
        playerDashTargetCircle->pos.y = player->pos.y + dashDirection->y;
        playerDashTargetCircle->visible = TRUE;
        playerDashTargetCircle->super.vptr->update((ComponentNode)playerDashTargetCircle, &(playerDashTargetCircle->pos));
    }
    else
    {
        GAME_TIME_TICK = 1.0 / 60.0;
        if (rPressed && inmng.keyBoardEventType == KEY_UP)
        {
            rPressed = FALSE;
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
                bulletTimeSound->stop(bulletTimeSound);
            }
        }
    }
    // 技能条的长度更新
    dashPower = dashPower < 90.0 ? dashPower + 50 * delta : 90.0;
    ((Rect *)(playerDashPowerStrip->shape))->width = (dashPower / 90.0) * playerTextureDown->getWidth(playerTextureDown);
    // 人物纹理颜色与可见性改变
    Texture *playerTexture;
    if (playerTextureDown->visible)
        playerTexture = playerTextureDown;
    else if (playerTextureUp->visible)
        playerTexture = playerTextureUp;
    else if (playerTextureLeft->visible)
        playerTexture = playerTextureLeft;
    else
        playerTexture = playerTextureRight;

    if (player->vel.length(&(player->vel)) >= 1.5)
    {
        playerTexture->color = "Blue";
        if (player->vel.length(&(player->vel)) >= 2.5)
        {
            playerTexture->visible = FALSE;
        }
        else
        {
            playerTexture->visible = TRUE;
        }
    }
    else
    {
        playerTexture->color = "Cyan";
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
    Vector *stripPos = newVector(player->pos.x, player->pos.y - playerTexture->getHeight(playerTexture) / 1.5);
    playerTexture->super.vptr->update((ComponentNode)playerTexture, stripPos);
    destoryVector(stripPos);
    playerDashPowerStrip->super.vptr->update((ComponentNode)playerDashPowerStrip, &(player->pos));

    // 销毁所有申请的，级别在Component以下的内存空间
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
            if (scmng.getScene("scene1") == NULL)
                scmng.loadScene(&scene1, createScene1);
            scmng.switchTo("scene1", TRUE, FALSE, NULL, 0);
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