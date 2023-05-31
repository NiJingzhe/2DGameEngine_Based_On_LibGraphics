#include "room1_bkgnd.h"

ActorNode room1Background;
picTexture *room1ground;
CollisionShape *room1WallCollisionList[3];
CollisionShape *room1SkillPointCollisionList[2];

void createRoom1Background()
{
    Vector *backgroundPos = newVector(getww / 2, getwh / 2);
    room1Background = newActor("room1_background", backgroundPos);
    room1ground = newPicTexture("./res/scene2/room1.bmp", backgroundPos, getww, getwh);
    destoryVector(backgroundPos);

    Vector *icePointPos = newVector(getww / 10, getwh / 8);
    Circle *icePoint = newCircle(icePointPos, 0, 0.5, FALSE, "Cyan", 1);
    room1SkillPointCollisionList[0] = newCollisionShape((Shape *)icePoint);
    destoryShape((Shape *)icePoint);
    destoryVector(icePointPos);

    Vector *lighteningPointPos = newVector(getww * 17.6 / 20, getwh * 16.5 / 20);
    Circle *lighteningPoint = newCircle(lighteningPointPos, 0, 0.5, FALSE, "Red", 1);
    room1SkillPointCollisionList[1] = newCollisionShape((Shape *)lighteningPoint);
    destoryVector(lighteningPointPos);
    destoryShape((Shape *)lighteningPoint);

    Vector *wallPos = newVector(getww * 9.95 / 20, getwh * 7 / 20);
    Rect *wall1Rect = newRect(wallPos, 0, 6.8, 1.0, FALSE, "Green", 1);
    room1WallCollisionList[0] = newCollisionShape((Shape *)wall1Rect);
    destoryShape((Shape *)wall1Rect);
    destoryVector(wallPos);

    wallPos = newVector(getww * 16.6 / 20, getwh * 8.52 / 20);
    Rect *wall2Rect = newRect(wallPos, 0, 1.1, 4.8, FALSE, "Green", 1);
    room1WallCollisionList[1] = newCollisionShape((Shape *)wall2Rect);
    destoryShape((Shape *)wall2Rect);
    destoryVector(wallPos);

    wallPos = newVector(getww * 3.24 / 20, getwh * 13.355 / 20);
    Circle *wall3Circle = newCircle(wallPos, 0, 1.22, FALSE, "Green", 1);
    room1WallCollisionList[2] = newCollisionShape((Shape *)wall3Circle);
    destoryShape((Shape *)wall3Circle);
    destoryVector(wallPos);

    room1Background->addComponent(room1Background, (ComponentNode)room1ground);
    room1Background->addComponent(room1Background, (ComponentNode)room1SkillPointCollisionList[0]);
    room1Background->addComponent(room1Background, (ComponentNode)room1SkillPointCollisionList[1]);
    room1Background->addComponent(room1Background, (ComponentNode)room1WallCollisionList[0]);
    room1Background->addComponent(room1Background, (ComponentNode)room1WallCollisionList[1]);
    room1Background->addComponent(room1Background, (ComponentNode)room1WallCollisionList[2]);
}

void setupRoom1Background(void *param)
{
    room1ground->visible = TRUE;
    room1ground->setMeta((ComponentNode)room1ground, "room1_ground");
    room1SkillPointCollisionList[0]->visible = FALSE;
    room1SkillPointCollisionList[0]->setMeta((ComponentNode)room1SkillPointCollisionList[0], "room1_skillpoint_ice");
    room1SkillPointCollisionList[1]->visible = FALSE;
    room1SkillPointCollisionList[1]->setMeta((ComponentNode)room1SkillPointCollisionList[1], "room1_skillpoint_lightening");
    room1WallCollisionList[0]->visible = TRUE;
    room1WallCollisionList[0]->enable = TRUE;
    room1WallCollisionList[0]->setMeta((ComponentNode)room1WallCollisionList[0], "room1_wall");
    room1WallCollisionList[1]->visible = TRUE;
    room1WallCollisionList[1]->enable = TRUE;
    room1WallCollisionList[1]->setMeta((ComponentNode)room1WallCollisionList[1], "room1_wall");
    room1WallCollisionList[2]->visible = TRUE;
    room1WallCollisionList[2]->enable = TRUE;
    room1WallCollisionList[2]->setMeta((ComponentNode)room1WallCollisionList[2], "room1_wall");
}
