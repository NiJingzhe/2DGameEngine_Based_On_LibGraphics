#ifndef __PUBLIC_HEADERS_H__
#define __PUBLIC_HEADERS_H__
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include <math.h>

#define DEBUG   0

#define fill(x) StartFilledRegion(x)
#define endfill EndFilledRegion()
#define getww GetWindowWidth()
#define getwh GetWindowHeight()

#define PI 3.1415926535
#define toDeg(x) ((x)/57.3)
#define toRad(x) ((x)/PI * 180)

#endif

