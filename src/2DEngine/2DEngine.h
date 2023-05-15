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

#define ENGINE_DEBUG 1

#if ENGINE_DEBUG
#define LOG(str) {printf("\nLOG: \n"); printf(str);}while(0)
#endif

extern double GAME_TIME_TICK;

#endif