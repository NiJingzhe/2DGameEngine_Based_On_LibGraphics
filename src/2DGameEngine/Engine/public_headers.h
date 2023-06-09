#ifndef __PUBLIC_HEADERS_H__
#define __PUBLIC_HEADERS_H__
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <malloc.h>
#include <time.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "config.h"

#include <math.h>
#pragma comment(lib,"winmm.lib")

#define MEM_DEBUG MEM_DEBUG_MODE
#define MOUSE_DEBUG MOUSE_POS_DEBUG
#define DEBUG_MODE DEBUG || MEM_DEBUG || MOUSE_POS_DEBUG

#define fill(x) StartFilledRegion(x)
#define endfill EndFilledRegion()
#define getww GetWindowWidth()
#define getwh GetWindowHeight()

#define PI 3.1415926535
#define toDeg(x) ((x)/57.3)
#define toRad(x) ((x)/PI * 180)

extern int MEM_BLOCK_NUM;


#endif

