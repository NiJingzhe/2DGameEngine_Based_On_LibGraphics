#ifndef __2D_ENGINE_H__
#define __2D_ENGINE_H__

#include "actor.h"
#include "component.h"
#include "input_manager.h"
#include "public_headers.h"
#include "shape.h"
#include "vector.h"
#include "scene.h"
#include "scenes_manager.h"

#define ENGINE_DEBUG (MEM_DEBUG || DEBUG)

#define LOG(str) {printf("\nLOG: \n"); printf(str);}while(0)
#define to255(x) ((x) / 255.0)

extern double GAME_TIME_TICK;

#endif