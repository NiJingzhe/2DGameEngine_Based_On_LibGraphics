#ifndef __PLAYER1_H__
#define __PLAYER1_H__
#include "2DEngine.h"

// 创建Player
extern Vector *playerPos;
extern ActorNode player;
// playerTexture
extern charTexture *playerTextureDown;
extern charTexture *playerTextureUp;
extern charTexture *playerTextureRight;
extern charTexture *playerTextureLeft;
// playerCollisionShape
extern Rect *collisionRect;
extern CollisionShape *playerCollisionShape;
extern Circle *targetCircle;
extern CollisionShape *playerDashTargetShape;
extern Vector *stripPos;
extern Rect *stripeRect;
extern CollisionShape *playerDashPowerStrip;
extern Audio *bulletTimeSound;

extern bool freezed;

void createPlayer1();
void setupPlayer1(void *param);


#endif