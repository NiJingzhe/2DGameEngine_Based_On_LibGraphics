#include "2DEngine.h"
#include "scene_info.h"

SceneNode scene2;

/////////////////////////////// Actor 与 Component 声明部分 //////////////////////////////////
// 背景Actor
Vector *scene2BackgoundPos;
Rect *scene2BackgoundRect;
ActorNode scene2Backgound;
CollisionShape *scene2BackgoundShape;

// 暂停按钮Actor
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

Vector *player2Pos;
ActorNode player2;
// player2Texture
Texture *player2TextureDown;
Texture *player2TextureUp;
Texture *player2TextureRight;
Texture *player2TextureLeft;
// player2CollisionShape
Rect *player2collisionRect;
CollisionShape *player2CollisionShape;
Timer *freezSkillTimer;
Audio *freezingFX;

// 倒计时UI
Vector *countDownPos;
ActorNode countDownUI;
Timer *timer;
UIText *countDownText;
/////////////////////////////// Actor 与 Component 声明部分 //////////////////////////////////
/*------------------------------------------------------------------------------------------*/
/////////////////////////////// 场景全局量声明部分 ///////////////////////////////////////////
// 单位是ms
long long int countDown;
// 单位是s
long long int aliveTime;
// 标志玩家1是否被控制
bool freezed = FALSE;
// 原始渲染函数
ActorRender normalRender;
// 玩家技能cd标志
bool inCD1 = FALSE;
bool inCD2 = FALSE;
/////////////////////////////// 场景全局量声明部分 ///////////////////////////////////////////
/*------------------------------------------------------------------------------------------*/
/////////////////////////////// 场景内Actor相关函数声明部分 //////////////////////////////////
static void pauseButtonUpdate(ActorNode button, double delta);
static void playerUpdate(ActorNode player, double delta);
static void playerSpecialRender(ActorNode player);
static void player2Update(ActorNode player2, double delta);
static void setupScene_scene2(SceneNode scene2, void *param);
static void CALLBACK countDownUpdate(HWND hwnd, UINT msg, UINT_PTR timerID, DWORD dwTime);
static void CALLBACK freezSkill(HWND hwnd, UINT msg, UINT_PTR timerID, DWORD dwTime);
static void countDownUIUpdate(ActorNode countDownUI, double delta);
/////////////////////////////// 场景内Actor相关函数声明部分 //////////////////////////////////
/*------------------------------------------------------------------------------------------*/
/////////////////////////////// 场景内Actor相关函数实现部分 //////////////////////////////////

/// @brief 创建场景2，包括场景2与其中各种Actor和组件的内存申请
/// @param scene2
void createScene2(SceneNode *scene2)
{

    *scene2 = newScene("scene2");
    (*scene2)->setup = setupScene_scene2;

    // background
    scene2BackgoundPos = newVector(getww / 2, getwh / 2);
    scene2BackgoundRect = newRect(scene2BackgoundPos, 0, getww, getwh, TRUE, "Black", 1);
    scene2Backgound = newActor("scene2Backgound", scene2BackgoundPos);
    scene2BackgoundShape = newCollisionShape((Shape *)scene2BackgoundRect);

    // pause button
    pauseButtonPos = newVector(getww / 12, getwh * 7 / 8);
    pauseButton = newActor("pause_button", pauseButtonPos);
    pauseButtonTexture = newTexture("./res/scene2/pause_button_texture.txt", pauseButtonPos, "White", 1);

    // player1
    playerPos = newVector(getww / 2, getwh / 2);
    player = newActor("player", playerPos);
    playerTextureDown = newTexture("./res/scene2/player_down.txt", playerPos, "White", 1);
    playerTextureUp = newTexture("./res/scene2/player_up.txt", playerPos, "White", 1);
    playerTextureRight = newTexture("./res/scene2/player_right.txt", playerPos, "White", 1);
    playerTextureLeft = newTexture("./res/scene2/player_left.txt", playerPos, "White", 1);
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

    // player2
    player2Pos = newVector(getww / 2, getwh / 5);
    player2 = newActor("player2", player2Pos);
    player2TextureDown = newTexture("./res/scene2/player_down.txt", player2Pos, "Cyan", 1);
    player2TextureUp = newTexture("./res/scene2/player_up.txt", player2Pos, "Cyan", 1);
    player2TextureRight = newTexture("./res/scene2/player_right.txt", player2Pos, "Cyan", 1);
    player2TextureLeft = newTexture("./res/scene2/player_left.txt", player2Pos, "Cyan", 1);
    player2collisionRect = newRect(
        player2Pos,
        0,
        player2TextureDown->getWidth(player2TextureDown), player2TextureDown->getHeight(player2TextureDown),
        FALSE,
        "Red",
        1);
    player2CollisionShape = newCollisionShape((Shape *)player2collisionRect);
    freezSkillTimer = newTimer(2, 2500, freezSkill);
    freezingFX = newAudio("./res/scene2/freezingfx.mp3", FALSE);

    // countdownUI
    countDownPos = newVector(getww / 2, getwh * 8 / 10);
    countDownUI = newActor("count_down_ui", countDownPos);
    timer = newTimer(1, 1000, countDownUpdate);
    countDownText = newUIText("01:00", countDownPos, "White", "Consolas", Bold, 50);
    

    // add component part
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

    player2->addComponent(player2, (ComponentNode)player2TextureDown);
    player2->addComponent(player2, (ComponentNode)player2TextureUp);
    player2->addComponent(player2, (ComponentNode)player2TextureRight);
    player2->addComponent(player2, (ComponentNode)player2TextureLeft);
    player2->addComponent(player2, (ComponentNode)player2CollisionShape);
    player2->addComponent(player2, (ComponentNode)freezSkillTimer);
    player2->addComponent(player2, (ComponentNode)freezingFX);

    countDownUI->addComponent(countDownUI, (ComponentNode)countDownText);
    countDownUI->addComponent(countDownUI, (ComponentNode)timer);

    // 添加Actor到场景
    (*scene2)->addActor((*scene2), scene2Backgound);
    (*scene2)->addActor((*scene2), countDownUI);
    ;
    (*scene2)->addActor((*scene2), pauseButton);
    (*scene2)->addActor((*scene2), player);
    (*scene2)->addActor((*scene2), player2);

    destoryVector(scene2BackgoundPos);
    destoryShape((Shape *)scene2BackgoundRect);
    destoryVector(pauseButtonPos);
    destoryVector(playerPos);
    destoryVector(player2Pos);
    destoryVector(stripPos);
    destoryShape((Shape *)collisionRect);
    destoryShape((Shape *)player2collisionRect);
    destoryShape((Shape *)targetCircle);
    destoryShape((Shape *)stripRect);
    destoryVector(countDownPos);
}

/// @brief 场景2的初始化函数，初始化各个Component的属性
/// @param scene2
/// @param param
static void setupScene_scene2(SceneNode scene2, void *param)
{

    if (param != NULL)
    {
        if (*((bool *)param) == TRUE)
        {
            timer->start(timer);
        }
        else
        {
            countDown = 60 * 1000;
            aliveTime = 0;
        }
    }
    else
    {
        countDown = 60 * 1000;
        aliveTime = 0;
    }

    // background
    scene2BackgoundShape->enable = FALSE;
    scene2BackgoundShape->visible = TRUE;
    scene2BackgoundShape->setMeta((ComponentNode)scene2BackgoundShape, "scene2BackgoundShape");

    // pause button
    pauseButtonTexture->visible = TRUE;
    pauseButtonTexture->setMeta((ComponentNode)pauseButtonTexture, "pause_button_texture");
    pauseButton->vptr->update = pauseButtonUpdate;

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
    playerDashPowerStrip->visible = TRUE;
    playerDashPowerStrip->enable = FALSE;
    playerDashPowerStrip->setMeta((ComponentNode)playerDashPowerStrip, "player_dash_power_strip");
    // Audio
    bulletTimeSound->setMeta((ComponentNode)bulletTimeSound, "bullet_time_sound");
    // update function
    player->vptr->update = playerUpdate;
    normalRender = player->vptr->render;
    // player2
    player2TextureDown->visible = TRUE;
    player2TextureDown->setMeta((ComponentNode)player2TextureDown, "player2_texture_down");
    player2TextureUp->visible = FALSE;
    player2TextureUp->setMeta((ComponentNode)player2TextureUp, "player2_texture_up");
    player2TextureRight->visible = FALSE;
    player2TextureRight->setMeta((ComponentNode)player2TextureRight, "player2_texture_right");
    player2TextureLeft->visible = FALSE;
    player2TextureLeft->setMeta((ComponentNode)player2TextureLeft, "player2_texture_left");
    player2CollisionShape->visible = FALSE;
    player2CollisionShape->enable = TRUE;
    player2CollisionShape->setMeta((ComponentNode)player2CollisionShape, "player2_collision_shape");
    freezSkillTimer->setMeta((ComponentNode)freezSkillTimer, "freez_skill_timer");
    freezingFX->setMeta((ComponentNode)freezingFX, "freezing_fx");
    player2->vptr->update = player2Update;

    // countdown ui
    timer->setMeta((ComponentNode)timer, "count_down_timer");
    timer->start(timer);
    countDownText->setMeta((ComponentNode)countDownText, "count_down_text");
    countDownUI->vptr->update = countDownUIUpdate;
}

extern double GAME_TIME_TICK;

/// @brief 玩家更新方法
/// @param player type: ActorNode, 玩家Actor本身
/// @param delta  type: double, 游戏时间微分元
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
    static bool enterPressed = FALSE;
    static double dashPower = 90;
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
    if (!inmng.keyStates[VK_RETURN] || player->vel.length(&(player->vel)) >= 2.0)
    {
        player->vel.add(&(player->vel), acc);
    }
    memcpy(&(vel), &(player->vel), sizeof(Vector));
    vel.mult(&(vel), delta * VEL_CONST);
    player->pos.add(&(player->pos), &(vel));
    if (!inmng.keyStates[VK_RETURN] || player->vel.length(&(player->vel)) >= 2.0)
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

    Texture *playerTexture;
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
    if (inmng.keyStates[VK_RETURN] && player->vel.length(&(player->vel)) <= 2.0 && !freezed && dashPower >= 10)
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
    dashPower = dashPower < 90.0 ? dashPower + 50 * delta : 90.0;
    ((Rect *)(playerDashPowerStrip->shape))->width = (dashPower / 90.0) * playerTextureDown->getWidth(playerTextureDown);
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

    Texture *playerTextureDown = (Texture *)(player->getComponent(player, "player_texture_down"));
    Texture *playerTextureUp = (Texture *)(player->getComponent(player, "player_texture_up"));
    Texture *playerTextureRight = (Texture *)(player->getComponent(player, "player_texture_right"));
    Texture *playerTextureLeft = (Texture *)(player->getComponent(player, "player_texture_left"));
    Texture *playerTexture;
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

/// @brief 第二玩家更新方法
/// @param player2
/// @param delta
static void player2Update(ActorNode player2, double delta)
{

    // 获取玩家在更新过过程中需要用到的组件
    Texture *player2TextureDown = (Texture *)(player2->getComponent(player2, "player2_texture_down"));
    Texture *player2TextureUp = (Texture *)(player2->getComponent(player2, "player2_texture_up"));
    Texture *player2TextureRight = (Texture *)(player2->getComponent(player2, "player2_texture_right"));
    Texture *player2TextureLeft = (Texture *)(player2->getComponent(player2, "player2_texture_left"));
    CollisionShape *player2CollisionShape = (CollisionShape *)(player2->getComponent(player2, "player2_collision_shape"));
    Timer *freezSkillTimer = (Timer *)(player2->getComponent(player2, "freez_skill_timer"));
    Audio *freezingFX = (Audio *)(player2->getComponent(player2, "freezing_fx"));

    // 部分量的定义
    static bool enterPressed = FALSE;
    static double dashPower = 90;
    double ACC_CONST = 10;
    double VEL_CONST = 5;

    // 键盘控制人物移动部分
    Vector *acc = newVector((-inmng.keyStates['A'] + inmng.keyStates['D']),
                            (-inmng.keyStates['S'] + inmng.keyStates['W']));

    acc->normalize(acc);
    acc->mult(acc, delta * ACC_CONST);
    Vector vel;

    player2->vel.add(&(player2->vel), acc);
    memcpy(&(vel), &(player2->vel), sizeof(Vector));
    vel.mult(&(vel), delta * VEL_CONST);
    player2->pos.add(&(player2->pos), &(vel));
    player2->vel.mult(&(player2->vel), 0.9);

    if (fabs(player2->vel.x) <= 0.1)
        player2->vel.x = 0;
    if (fabs(player2->vel.y) <= 0.1)
        player2->vel.y = 0;

    // 处理人物移动出边界的情况
    if (player2->pos.x < 0)
        player2->pos.x += getww;
    if (player2->pos.y < 0)
        player2->pos.y += getwh;
    if (player2->pos.x > getww)
        player2->pos.x -= getww;
    if (player2->pos.y > getwh)
        player2->pos.y -= getwh;

    // 处理人物不同方向的纹理展示
    if (player2->vel.length(&(player2->vel)) == 0)
    {
        player2TextureDown->visible = TRUE;
        player2TextureUp->visible = FALSE;
        player2TextureLeft->visible = FALSE;
        player2TextureRight->visible = FALSE;
    }
    else
    {
        if (player2->vel.y > 0)
        {
            player2TextureDown->visible = FALSE;
            player2TextureUp->visible = TRUE;
            player2TextureLeft->visible = FALSE;
            player2TextureRight->visible = FALSE;
        }
        else
        {
            player2TextureDown->visible = TRUE;
            player2TextureUp->visible = FALSE;
            player2TextureLeft->visible = FALSE;
            player2TextureRight->visible = FALSE;
        }

        if (player2->vel.x > 0 && fabs(player2->vel.x) > 0.6 * fabs(player2->vel.y))
        {
            player2TextureDown->visible = FALSE;
            player2TextureUp->visible = FALSE;
            player2TextureLeft->visible = FALSE;
            player2TextureRight->visible = TRUE;
        }
        else if (player2->vel.x < 0 && fabs(player2->vel.x) > 0.6 * fabs(player2->vel.y))
        {
            player2TextureDown->visible = FALSE;
            player2TextureUp->visible = FALSE;
            player2TextureLeft->visible = TRUE;
            player2TextureRight->visible = FALSE;
        }
    }

    Texture *playerTexture;
    if (player2TextureDown->visible)
        playerTexture = player2TextureDown;
    else if (player2TextureUp->visible)
        playerTexture = player2TextureUp;
    else if (player2TextureLeft->visible)
        playerTexture = player2TextureLeft;
    else
        playerTexture = player2TextureRight;

    // 空格键技能
    static bool spacePressing = FALSE;
    if (inmng.keyStates[VK_SPACE] && !spacePressing)
    {
        spacePressing = TRUE;
        freezed = TRUE;
        freezingFX->play(freezingFX);
        freezSkillTimer->start(freezSkillTimer);
    }
    else
    {
        spacePressing = FALSE;
    }

    // 根据人物位置更新各个组件的位置
    player2CollisionShape->super.vptr->update((ComponentNode)player2CollisionShape, &(player2->pos));
    playerTexture->super.vptr->update((ComponentNode)playerTexture, &(player2->pos));
    // 销毁所有申请的，级别在Component以下的内存空间
    destoryVector(acc);
}

/// @brief 暂停按钮更新方法
/// @param button
/// @param delta
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

/// @brief 冻结效果计时器
/// @param hwnd
/// @param msg
/// @param timerID
/// @param dwTime
static void CALLBACK freezSkill(HWND hwnd, UINT msg, UINT_PTR timerID, DWORD dwTime)
{
    static int count = 0;
    count++;
    if (count == 1)
    {
        freezed = FALSE;
        count = 0;
        freezingFX->stop(freezingFX);
        freezSkillTimer->stop(freezSkillTimer);
    }
}

/// @brief 倒计时器回调函数
/// @param hwnd
/// @param msg
/// @param timerID
/// @param dwTime
/// @return
static void CALLBACK countDownUpdate(HWND hwnd, UINT msg, UINT_PTR timerID, DWORD dwTime)
{
    aliveTime++;
    countDown = countDown > 0 ? countDown - 1000 : 0;
}

/// @brief 倒计时UI更新函数
/// @param countDownUI
/// @param delta
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
/////////////////////////////// 场景内Actor相关函数实现部分 //////////////////////////////////