#version 430 core

// Input
struct Transform 
{
    ivec2 AtlasOffset;
    ivec2 SpriteSize;
    vec2 Position;
    vec2 Size;
};

layout(std430, Binding = 0) buffer TransformSBO 
{
    Transform Transforms[];
};

uniform vec2 ScreenSize;

// Output
layout(location = 0) out vec2 TextureCoordsOut;

void main() 
{
    Transform Transform = Transforms[gl_InstanceID];

    vec2 Vertices[6] = 
    {
        Transform.Position,
        vec2(Transform.Position + vec2(0.0, Transform.Size.y)),
        vec2(Transform.Position + vec2(Transform.Size.x, 0.0)),
        vec2(Transform.Position + vec2(Transform.Size.x, 0.0)),
        vec2(Transform.Position + vec2(0.0, Transform.Size.y)),
        Transform.Position + Transform.Size
    };

    float Left = Transform.AtlasOffset.x;
    float Top = Transform.AtlasOffset.y;
    float Right = Transform.AtlasOffset.x + Transform.SpriteSize.x;
    float Bottom = Transform.AtlasOffset.y + Transform.SpriteSize.y;

    vec2 TextureCoords[6] = 
    {
        vec2(Left, Top),
        vec2(Left, Bottom),
        vec2(Right, Top),
        vec2(Right, Top),
        vec2(Left, Bottom),
        vec2(Right, Bottom),
    };

    // NORMALIZE
    vec2 VertexPos = Vertices[gl_VertexID];
    VertexPos.y = -VertexPos.y + ScreenSize.y;
    VertexPos = 2.0f * (VertexPos / ScreenSize) - 1.0f;
    gl_Position = vec4(VertexPos, 0.0f, 1.0f);

    TextureCoordsOut = TextureCoords[gl_VertexID];
}
