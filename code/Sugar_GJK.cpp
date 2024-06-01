#include "Sugar_GJK.h"

internal vec2
AveragePoint(Collider *A) 
{
    vec2 Average = {};

    for(int Index = 0; 
        Index < A->VertexCount; 
        ++Index) 
    {
        Average.x += A->Vertices[Index].x;
        Average.y += A->Vertices[Index].y;
    }
    Average.x /= A->VertexCount;
    Average.y /= A->VertexCount;

    return(Average);
}

internal int 
FurthestPoint(Collider *A, vec2 Direction) 
{ 
    int BestIndex = 0;
    real32 MaxProduct = vDot(A->Vertices[0], Direction);
    vec2 Result = {};
    for(int Point = 1; 
        Point < A->VertexCount; 
        ++Point) 
    { 
        real32 Product = vDot(A->Vertices[Point], Direction); 
        if(Product > MaxProduct) 
        {
            MaxProduct = Product;
            BestIndex = Point;
        }
    }
    return(BestIndex);
}

internal vec2
GJKSupport(Collider *A, Collider *B, vec2 Direction) 
{
    int AMax = FurthestPoint(A, Direction);
    int BMax = FurthestPoint(B, -Direction);

    return(A->Vertices[AMax] - B->Vertices[BMax]);
}

internal int
GJK(Collider *A, Collider *B) 
{ 
    vec2 Simplex[3] = {};
    vec2 Position1 = AveragePoint(A);
    vec2 Position2 = AveragePoint(B);
    vec2 Direction = Position1 - Position2; // Set the direction to the point in the Minkowski Difference

    vec2 RegionA;
    vec2 RegionB;
    vec2 RegionC;
    vec2 AO;
    vec2 AB;
    vec2 AC;
    vec2 ACPerp;
    vec2 ABPerp;

    // If Zero set to any arbitray Axis
    if((Direction.x == 0) && (Direction.y == 0)) 
    {
        Direction.x = 1.0f;
    }

    // Set the First Support as the new point of the Simplex
    Simplex[0] = GJKSupport(A, B, Direction);
    if(vDot(Simplex[0], Direction) <= 0) 
    {
        return(0);
        // Collision not possible
    }

    // Invert the vector so the next search direction is flipped towards the origin
    Direction = vInverse(Direction);

    int Index = 0;
    for(;;) 
    {
        RegionA = Simplex[++Index] = GJKSupport(A, B, Direction);
        if(vDot(RegionA, Direction) <= 0) 
        {
            return(0);
            // Collision not possible
        }

        AO = vInverse(RegionA); // From point A to Origin is likely just negative A
        
        // Simplex has 2 points, it is not a triangle yet.
        if(Index < 2) 
        {
            RegionB = Simplex[0];
            AB = RegionB - RegionA;
            Direction = TripleProduct(AB, AO, AB);
            if(vLengthSq(Direction) == 0) 
            {
                Direction = Perpendicular(AB);
            }
            continue;
        } 

        RegionB = Simplex[1];
        RegionC = Simplex[0];

        AB = RegionB - RegionA; // Normal from AB to the origin
        AC = RegionC - RegionA;
        
        ACPerp = TripleProduct(AB, AC, AC);
        if(vDot(ACPerp, AO) >= 0) 
        { 
            Direction = ACPerp;
        }
        else 
        {
            ABPerp = TripleProduct(AC, AB, AB);
            if(vDot(ABPerp, AO) < 0) 
            {
                return(1);
                //Collision
            }

            Simplex[0] = Simplex[1]; // Swap the First Element (C);
            Direction = ABPerp; // New Direction is normal to AB -> Origin
        }

        Simplex[1] = Simplex[2]; // Swap Elements
        --Index;
    }
}
