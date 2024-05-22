#pragma once

#if SUGAR_SLOW

#define Assert(Expression, Message) if(!(Expression)) {OutputDebugStringA(Message); DebugBreak();}
#define Trace(Message) {OutputDebugStringA(Message);}
#define WIN32_LEAN_AND_MEAN
#define EXTRA_LEAN
#include <windows.h>
#include "Win32_Sugar.h"
#include "SugarAPI.h"

#else

#define Assert(Expression, Message)

#endif

#define Kilobytes(Value) ((uint64)(Value) * 1024)
#define Megabytes(Value) ((uint64)Kilobytes(Value) * 1024)
#define Gigabytes(Value) ((uint64)Megabytes(Value) * 1024)

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#define global_variable static
#define local_persist static
#define internal static

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef float real32;
typedef double real64;
