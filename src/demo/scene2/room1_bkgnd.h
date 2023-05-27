#ifndef __ROOM1_BKGND_H__
#define __ROOM1_BKGND_H__
#include "2DEngine.h"

extern ActorNode room1Background;
extern picTexture *room1ground;
extern CollisionShape *room1WallCollisionList[3];
extern CollisionShape *room1SkillPointCollisionList[2];

void createRoom1Background();
void setupRoom1Background(void *param);

#endif // !__ROOM1_BKGND_H__