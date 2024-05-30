#pragma once

#include "../Sugar_Intrinsics.h"
#include <stdint.h>
#include <math.h>

typedef float real32;
typedef double real64;
typedef int32_t int32;
typedef int64_t int64;

// FLOAT VECTOR 2

struct vec2 
{
    real32 x;
    real32 y;
};

// OPERATOR OVERLOADING

inline vec2 
operator-(vec2 A, vec2 B)
{
    vec2 Result = {};
    Result.x = A.x - B.x;
    Result.y = A.y - B.y;

    return(Result);
}

inline vec2 
operator-(vec2 A) 
{
    vec2 Result = {};
    Result.x = -A.x;
    Result.y = -A.y;

    return(Result);
}

inline vec2 
operator+(vec2 A, vec2 B) 
{
    vec2 Result = {};
    Result.x = A.x + B.x;
    Result.y = A.y + B.y;

    return(Result);
}

inline vec2 
operator*(vec2 A, vec2 B) 
{
    vec2 Result = {};
    Result.x = A.x * B.x;
    Result.y = A.y * B.y;

    return(Result);
}

inline vec2 
operator*(vec2 A, real32 B) 
{
    vec2 Result = {};
    Result.x = A.x * B;
    Result.y = A.y * B;

    return(Result);
}

// FLOAT OPERATIONS

inline real32 
fSquare(real32 A) 
{
    real32 Result = A * A;
    return(Result);
}

inline real32
vDot(vec2 A, vec2 B) 
{
    real32 Result = A.x*B.x + A.y*B.y;
    return(Result);
}

inline real32
vCross(vec2 A, vec2 B) 
{
    real32 Result = A.x*B.y - A.y*B.x;
    return(Result);
}

inline real32 
vLengthSq(vec2 A) 
{
    real32 Result = vDot(A, A);
    return(Result);
}

inline real32
VLength(vec2 A) 
{
    real32 Result = sqrtf(vLengthSq(A));
}

inline vec2
VNormalize(vec2 A) 
{ 
    vec2 Result = {};
    real32 Length = VLength(A);
    Result.x = A.x / Length;
    Result.y = A.y / Length;

    return(Result);
}

inline real32
Lerp(real32 A, real32 B, real32 T) 
{
    return(A + (B - A) * T);
}

inline vec2
VLerp(vec2 A, vec2 B, real32 T) 
{
    vec2 Result = {};
    Result.x = Lerp(A.x, B.x, T);
    Result.y = Lerp(A.y, B.y, T);

    return(Result);
}

inline real32 
maxf32(real32 a, real32 b) 
{
    if(a > b) 
    {
        return(a);
    }
    return(b);
}

inline real32 
minf32(real32 a, real32 b) 
{
    if(a < b) 
    {
        return(a);
    }
    return(b);
}

inline vec2
Perpendicular(vec2 A) 
{
    vec2 Result = {A.y, -A.x};
    return(Result);
}

inline vec2
vInverse(vec2 A) 
{
    vec2 Result = {-A.x, -A.y};
    return(Result);
}

inline vec2 
TripleProduct(vec2 A, vec2 B, vec2 C) 
{ 
    vec2 Result;

    real32 AC = (A.x*C.x) + (A.y*C.y);
    real32 BC = (B.x*C.x) + (B.y*C.y);
    Result.x = (B.x*AC) - (A.x*BC);
    Result.y = (B.y*AC) - (A.y*BC);

    return(Result);
}

// INTEGER VECTOR 2

struct ivec2 
{
    int x;
    int y;
};

// OPERATOR OVERLOADING

inline ivec2 
operator-(ivec2 A , ivec2 B)
{
    ivec2 Result = {};
    Result.x = A.x - B.x;
    Result.y = A.y - B.y;

    return(Result);
}

inline ivec2 
operator+(ivec2 A, ivec2 B) 
{
    ivec2 Result = {};
    Result.x = A.x + B.x;
    Result.y = A.y + B.y;

    return(Result);
}

inline ivec2 
operator*(ivec2 A, ivec2 B) 
{
    ivec2 Result = {};
    Result.x = A.x * B.x;
    Result.y = A.y * B.y;

    return(Result);
}

inline ivec2 
operator*(ivec2 A, real32 B) 
{
    ivec2 Result = {};
    Result.x = int32(A.x * B);
    Result.y = int32(A.y * B);

    return(Result);
}

inline ivec2 
operator/(ivec2 A, ivec2 B) 
{
    ivec2 Result = {};
    Result.x = A.x / B.x;
    Result.y = A.y / B.y;

    return(Result);
}

// INTEGER OPERATIONS 

inline int64
maxi64(int64 a, int64 b) 
{
    if(a > b) 
    {
        return(a);
    }
    return(b);
}

inline int32 
iSquare (int32 A) 
{
    int32 Result = A * A;
    return(Result);
}

inline ivec2
iVLerp(ivec2 A, ivec2 B, real32 T) 
{
    ivec2 Result = {};
    Result.x = int32(Lerp((real32)A.x, (real32)B.x, T));
    Result.y = int32(Lerp((real32)A.y, (real32)B.y, T));

    return(Result);
}
