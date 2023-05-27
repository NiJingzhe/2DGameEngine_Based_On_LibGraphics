#ifndef __PLAYER2_H__
#define __PLAYER2_H__

#include "2DEngine.h"


extern Vector *player2Pos;
extern ActorNode player2;
// player2Texture
extern charTexture *player2TextureDown;
extern charTexture *player2TextureUp;
extern charTexture *player2TextureRight;
extern charTexture *player2TextureLeft;
// player2CollisionShape
extern Rect *player2collisionRect;
extern CollisionShape *player2CollisionShape;
extern Timer *freezSkillTimer;
extern Audio *freezingFX;


void createPlayer2();
void setupPlayer2(void *param);


#endif