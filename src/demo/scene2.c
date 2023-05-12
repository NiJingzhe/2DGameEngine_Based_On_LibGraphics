#include "2DEngine.h"
#include "scene2.h"
/*---------TODO---------
1. 完成场景2的构建 
2. 实现平滑人物控制器                         finished
3. 实现人物技能与技能槽机制                   finished
4. 完成敌人NPC 
5. 为人物技能释放过程增加一定的视觉效果
---------END TODO-------*/

SceneNode scene2;
static void pauseButtonUpdate(ActorNode button, double delta);
static void playerUpdate(ActorNode player, double delta);

void setupScene_scene2()
{
    //创建场景
    scene2 = newScene("scene2");

    //创建背景Actor
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

    //创建暂停按钮Actor
    Vector *pauseButtonPos = newVector(getww / 12, getwh * 7 / 8);
    ActorNode pauseButton = newActor("back_button", pauseButtonPos);
    Texture *pauseButtonTexture = newTexture("./res/scene2/pause_button_texture.txt", pauseButtonPos, "White", 1);
    pauseButtonTexture->visible = TRUE;
    pauseButtonTexture->setMeta((ComponentNode)pauseButtonTexture, "pause_button_texture");
    pauseButton->addComponent(pauseButton, (ComponentNode)pauseButtonTexture);
    pauseButton->vptr->update = pauseButtonUpdate;
    destoryVector(pauseButtonPos);

    //创建Player
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
    //Audio
    Audio* bulletTimeSound = newAudio("./res/scene2/bullet_time.mp3", FALSE);
    bulletTimeSound->setMeta((ComponentNode)bulletTimeSound, "bullet_time_sound");
    player->addComponent(player, (ComponentNode)bulletTimeSound);

    player->vptr->update = playerUpdate;
    destoryVector(playerPos);
    destoryVector(stripPos);
    destoryShape((Shape *)collisionRect);
    destoryShape((Shape *)targetCircle);
    destoryShape((Shape *)stripRect);

    //添加Actor到场景
    scene2->addActor(scene2, background);
    scene2->addActor(scene2, pauseButton);
    scene2->addActor(scene2, player);
}

extern double GAME_TIME_TICK;

static void playerUpdate(ActorNode player, double delta)
{
    //获取玩家在更新过过程中需要用到的组件
    Texture *playerTexture = (Texture *)(player->getComponent(player, "player_texture"));
    CollisionShape *playerCollisionShape = (CollisionShape *)(player->getComponent(player, "player_collision_shape"));
    CollisionShape *playerDashTargetCircle = (CollisionShape *)(player->getComponent(player, "player_dash_circle"));
    CollisionShape *playerDashPowerStrip = (CollisionShape *)(player->getComponent(player, "player_dash_power_strip"));
    Audio* bulletTimeSound = (Audio*)(player->getComponent(player, "bullet_time_sound"));
    //部分量的定义
    static bool rPressed = FALSE;
    static double dashPower = 90;
    double ACC_CONST = 10;
    double VEL_CONST = 5;

     //键盘控制人物移动部分
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
    
    //R键技能
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
            if (dashPower >= 10){
                dashPower = dashPower - 40 > 0 ? dashPower - 40 : 0;
                player->vel.x = dashDirection->x; player->vel.y = dashDirection->y;
                Vector vel;
                memcpy(&(vel), &(player->vel), sizeof(Vector));
                vel.mult(&(vel), delta * VEL_CONST * 25);
                player->pos.add(&(player->pos), &(vel));
                bulletTimeSound->stop(bulletTimeSound);
            }
            
        }
    }
    //技能条的长度更新
    dashPower = dashPower < 90.0 ? dashPower + 50 * delta : 90.0;
    ((Rect *)(playerDashPowerStrip->shape))->width = (dashPower / 90.0) * playerTexture->getWidth(playerTexture);
    //人物纹理颜色与可见性改变
    if (player->vel.length(&(player->vel)) >= 1.5){
        playerTexture->color = "Blue";
        if (player->vel.length(&(player->vel)) >= 2.5){
            playerTexture->visible = FALSE;
        }
        else{
            playerTexture->visible = TRUE;
        }
    }
    else{
        playerTexture->color = "Cyan";
    }
    //人物冲刺时碰撞箱无效
    if (player->vel.length(&(player->vel)) >= 1.5){
        playerCollisionShape->enable = FALSE;
    }
    else{
        playerCollisionShape->enable = TRUE;
    }


    //根据人物位置更新各个组件的位置
    playerCollisionShape->super.vptr->update((ComponentNode)playerCollisionShape, &(player->pos));
    Vector *stripPos = newVector(player->pos.x, player->pos.y - playerTexture->getHeight(playerTexture) / 1.5);
    playerTexture->super.vptr->update((ComponentNode)playerTexture, stripPos);
    destoryVector(stripPos);
    playerDashPowerStrip->super.vptr->update((ComponentNode)playerDashPowerStrip, &(player->pos));
    
    //销毁所有申请的，级别在Component以下的内存空间
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