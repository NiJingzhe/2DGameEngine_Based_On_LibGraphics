#include "player2.h"
#include "player1.h"
#include "room1_bkgnd.h"
#include "count_down.h"

Vector *player2Pos;
ActorNode player2;
// player2Texture
charTexture *player2TextureDown;
charTexture *player2TextureUp;
charTexture *player2TextureRight;
charTexture *player2TextureLeft;
// player2CollisionShape
Rect *player2collisionRect;
CollisionShape *player2CollisionShape;
Timer *freezSkillTimer;
Audio *freezingFX;

static bool skillEnabled = FALSE;

static void freezSkill();
static void player2Update(ActorNode player2, double delta);

void createPlayer2()
{
    // player2
    player2Pos = newVector(getww / 2, getwh / 5);
    player2 = newActor("player2", player2Pos);
    player2TextureDown = newcharTexture("./res/scene2/player_down.txt", player2Pos, "Cyan", 1);
    player2TextureUp = newcharTexture("./res/scene2/player_up.txt", player2Pos, "Cyan", 1);
    player2TextureRight = newcharTexture("./res/scene2/player_right.txt", player2Pos, "Cyan", 1);
    player2TextureLeft = newcharTexture("./res/scene2/player_left.txt", player2Pos, "Cyan", 1);
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

    player2->addComponent(player2, (ComponentNode)player2TextureDown);
    player2->addComponent(player2, (ComponentNode)player2TextureUp);
    player2->addComponent(player2, (ComponentNode)player2TextureRight);
    player2->addComponent(player2, (ComponentNode)player2TextureLeft);
    player2->addComponent(player2, (ComponentNode)player2CollisionShape);
    player2->addComponent(player2, (ComponentNode)freezSkillTimer);
    player2->addComponent(player2, (ComponentNode)freezingFX);

    destoryVector(player2Pos);
    destoryShape((Shape *)player2collisionRect);
}

void setupPlayer2(void *param)
{
    // player2
    player2TextureDown->visible = TRUE;
    player2TextureDown->setMeta((ComponentNode)player2TextureDown, "player2_texture_down");
    player2TextureUp->visible = FALSE;
    player2TextureUp->setMeta((ComponentNode)player2TextureUp, "player2_texture_up");
    player2TextureRight->visible = FALSE;
    player2TextureRight->setMeta((ComponentNode)player2TextureRight, "player2_texture_right");
    player2TextureLeft->visible = FALSE;
    player2TextureLeft->setMeta((ComponentNode)player2TextureLeft, "player2_texture_left");
    player2CollisionShape->visible = TRUE;
    player2CollisionShape->enable = TRUE;
    player2CollisionShape->setMeta((ComponentNode)player2CollisionShape, "player2_collision_shape");
    freezSkillTimer->setMeta((ComponentNode)freezSkillTimer, "freez_skill_timer");
    freezingFX->setMeta((ComponentNode)freezingFX, "freezing_fx");
    player2->vptr->update = player2Update;
}

static void player2Update(ActorNode player2, double delta)
{

    // 获取玩家在更新过过程中需要用到的组件
    charTexture *player2TextureDown = (charTexture *)(player2->getComponent(player2, "player2_texture_down"));
    charTexture *player2TextureUp = (charTexture *)(player2->getComponent(player2, "player2_texture_up"));
    charTexture *player2TextureRight = (charTexture *)(player2->getComponent(player2, "player2_texture_right"));
    charTexture *player2TextureLeft = (charTexture *)(player2->getComponent(player2, "player2_texture_left"));
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

    //获得速度后进行碰撞检查
    Vector *collisionVector;
    if (player2->isCollideWithActor(player2, room1Background))
    {
        collisionVector = player2->getCollisionVector(player2, room1Background);
        CollisionShape *icePointCollisionShape = (CollisionShape *)room1Background->getComponent(room1Background, "room1_skillpoint_ice");
        CollisionShape *lighteningPointCollisionShape = (CollisionShape *)room1Background->getComponent(room1Background, "room1_skillpoint_lightening");
        if (player2CollisionShape->isCollideWith(player2CollisionShape, icePointCollisionShape))
        {
            skillEnabled = TRUE;
        }
        else if (!player2CollisionShape->isCollideWith(player2CollisionShape, lighteningPointCollisionShape))
        {
            double collisionVectorAngle = collisionVector->getAngle(collisionVector);
            vel.rotate(&(vel), -collisionVectorAngle);
            vel.x = vel.x < 0 ? 0 : vel.x;
            vel.rotate(&(vel), collisionVectorAngle);
        }
        destoryVector(collisionVector);
    }

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

    charTexture *playerTexture;
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
    if (inmng.keyStates[VK_SPACE] && !spacePressing && skillEnabled)
    {
        spacePressing = TRUE;
        freezed = TRUE;
        freezingFX->play(freezingFX);
        freezSkillTimer->start(freezSkillTimer);
        skillEnabled = FALSE;
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

static void freezSkill()
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
