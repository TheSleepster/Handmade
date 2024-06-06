#include "Sugar_Intrinsics.h"
#include "Sugar.h"
#include "SugarAPI.h"
#include "win32_Sugar.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../data/deps/stbimage/stb_image.h"
#include "../data/deps/OpenGL/GLL.h"


// TODO : Make these "GetLastWriteTime" functions platform agnostic

internal void
LoadTexture(const char *Filepath, GameState *State) 
{
    glActiveTexture(GL_TEXTURE0);
    int Width, Height, Channels;
    char *Result = (char *)stbi_load(Filepath, &Width, &Height, &Channels, 4); 
    if(Result) 
    {
        State->glContext.TextureTimestamp = Win32GetLastWriteTime(Filepath);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, Width, Height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, Result);
        stbi_image_free(Result);
    }
}

internal GLuint 
LoadShader(int ShaderType, const char *Filepath, GameMemory *GameMemory) 
{
    int FileSize = 0;
    GLuint ShaderID = {}; 

    char *Shader = ReadEntireFileBA(Filepath, &FileSize, &GameMemory->TransientStorage); 
    if(Shader) 
    {
        ShaderID = glCreateShader(ShaderType);
        glShaderSource(ShaderID, 1, &Shader, 0);
        glCompileShader(ShaderID);
        {
            int Success;
            char ShaderLog[2048] = {};

            glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Success);
            if(!Success) 
            {
                glGetShaderInfoLog(ShaderID, 2048, 0, ShaderLog);
                Assert(false, ShaderLog);
            }
        }
    }
    else 
    {
        MessageBoxA(0, "Failed to load Shader Code!\n", "WGLOpenGL Issues", MB_ABORTRETRYIGNORE); 
    }
    return(ShaderID);
}

internal void
OpenGLRender(GameState *State, Win32_WindowData *WindowData) 
{
    FILETIME NewTextureWriteTime = Win32GetLastWriteTime(State->glContext.TextureDataFilepath);
    if(CompareFileTime(&NewTextureWriteTime, &State->glContext.TextureTimestamp) != 0) 
    {
        LoadTexture(State->glContext.TextureDataFilepath, State);
    }
    
    FILETIME TimeStampVertex = Win32GetLastWriteTime(State->glContext.VertexShaderFilepath); 
    FILETIME TimeStampFragment = Win32GetLastWriteTime(State->glContext.FragmentShaderFilepath); 
    if(CompareFileTime(&TimeStampVertex, &State->glContext.ShaderTimestamp) != 0||
       CompareFileTime(&TimeStampFragment, &State->glContext.ShaderTimestamp) != 0) 
    { 
        GLuint VertexShaderID = 
            LoadShader(GL_VERTEX_SHADER, State->glContext.VertexShaderFilepath, &State->GameMemory);
        GLuint FragmentShaderID = 
            LoadShader(GL_FRAGMENT_SHADER, State->glContext.FragmentShaderFilepath, &State->GameMemory);

        glAttachShader(State->glContext.ProgramID, VertexShaderID);
        glAttachShader(State->glContext.ProgramID, FragmentShaderID);
        glLinkProgram(State->glContext.ProgramID);

        glDetachShader(State->glContext.ProgramID, VertexShaderID);
        glDetachShader(State->glContext.ProgramID, FragmentShaderID);
        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);

        TimeStampVertex = Win32GetLastWriteTime(State->glContext.VertexShaderFilepath); 
        TimeStampFragment = Win32GetLastWriteTime(State->glContext.FragmentShaderFilepath); 
        State->glContext.ShaderTimestamp = maxFiletime(TimeStampVertex, TimeStampFragment);
    }

    glClearColor(1.0f, 0.1f, 1.0f, 1.0f);
    glClearDepth(0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, WindowData->WindowWidth, WindowData->WindowHeight);

    // SCREEN SIZE UNIFORM
    vec2 ScreenSize = {(real32)WindowData->WindowWidth, (real32)WindowData->WindowHeight};
    glUniform2fv(State->glContext.ScreenSizeID, 1, &ScreenSize.x);

    // PROJECTION MATRIX UNIFORM
    // 320 / 180
    State->RenderData->PlayerCamera.ProjectionMatrix = 
        CreateOrthographicMatrix(State->RenderData->PlayerCamera.Position.x - State->RenderData->PlayerCamera.Viewport.x,
                                 State->RenderData->PlayerCamera.Position.x + State->RenderData->PlayerCamera.Viewport.x,
                                 State->RenderData->PlayerCamera.Position.y - State->RenderData->PlayerCamera.Viewport.y,
                                 State->RenderData->PlayerCamera.Position.y + State->RenderData->PlayerCamera.Viewport.y);

    glUniformMatrix4fv
        (State->glContext.ProjectionMatrixID, 1, GL_FALSE, (const GLfloat *)&State->RenderData->PlayerCamera.ProjectionMatrix.Elements[0][0]);

    glBufferSubData
        (GL_SHADER_STORAGE_BUFFER, 0, sizeof(Transform) * State->RenderData->TransformCount, State->RenderData->Transforms);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, State->RenderData->TransformCount);
    State->RenderData->TransformCount = 0;
    SwapBuffers(WindowData->WindowDC);
}

internal void APIENTRY
OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, 
                    GLsizei length, const GLchar *message, const void *userparam) 
{
    if(severity == GL_DEBUG_SEVERITY_LOW||
       severity == GL_DEBUG_SEVERITY_MEDIUM||
       severity == GL_DEBUG_SEVERITY_HIGH) 
    {
        Assert(false, message);
    }
    else 
    {
        Trace(message);
    }
}

// TODO : Clean this up. Idrk how you can though.

internal void
InitializeOpenGLRenderer(GameState *State)
{
    LoadGLFunctions();

    State->glContext.VertexShaderFilepath = "res/shaders/BasicVertexShader.glsl";
    State->glContext.FragmentShaderFilepath = "res/shaders/BasicFragmentShader.glsl";
    State->glContext.TextureDataFilepath = "res/textures/TextureAtlas.png";

    glDebugMessageCallback(&OpenGLDebugCallback, nullptr);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_DEBUG_OUTPUT);

    // LOAD A SHADER
    GLuint VertexShaderID = 
        LoadShader(GL_VERTEX_SHADER, State->glContext.VertexShaderFilepath, &State->GameMemory);
    GLuint FragmentShaderID = 
        LoadShader(GL_FRAGMENT_SHADER, State->glContext.FragmentShaderFilepath, &State->GameMemory);

    FILETIME TimeStampVertex = Win32GetLastWriteTime(State->glContext.VertexShaderFilepath); 
    FILETIME TimeStampFragment = Win32GetLastWriteTime(State->glContext.FragmentShaderFilepath); 
    State->glContext.ShaderTimestamp = maxFiletime(TimeStampVertex, TimeStampFragment);
    // END OF LOADING

    State->glContext.ProgramID = glCreateProgram();
    glAttachShader(State->glContext.ProgramID, VertexShaderID);
    glAttachShader(State->glContext.ProgramID, FragmentShaderID);
    glLinkProgram(State->glContext.ProgramID);

    // NOTE : Local scope to test for the successful compilation of the Program
    {
        int Success;
        char ProgramLog[2048] = {};

        glGetProgramiv(State->glContext.ProgramID, GL_LINK_STATUS, &Success);
        if(!Success) 
        {
            glGetProgramInfoLog(State->glContext.ProgramID, 2048, 0, ProgramLog);
            Assert(false, ProgramLog); 
        }
    }

    glDetachShader(State->glContext.ProgramID, VertexShaderID);
    glDetachShader(State->glContext.ProgramID, FragmentShaderID);
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // NOTE : Testing Texture Rendering
    int Width, Height, Channels;
    char *Data = (char *)stbi_load(State->glContext.TextureDataFilepath, &Width, &Height, &Channels, 4); 
    Assert(Data, "Failed to get the TextureAtlas' data!\n");

    // NOTE : This is similar to creating a WGL context.
    // - You create a texture
    // - You Bind to it
    // - You Assign some attributes to it
    // - Use it
    {
        glGenTextures(1, &State->glContext.TextureID);
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, State->glContext.TextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, Width, Height,
                0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
        stbi_image_free(Data);
    }

    glGenBuffers(1, &State->glContext.TransformSBOID);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, State->glContext.TransformSBOID);

    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Transform) * MAX_TRANSFORMS, 
            State->RenderData->Transforms, GL_DYNAMIC_DRAW);

    // UNIFORM IDs
    State->glContext.ScreenSizeID = 
        glGetUniformLocation(State->glContext.ProgramID, "ScreenSize");

    State->glContext.ProjectionMatrixID = 
        glGetUniformLocation(State->glContext.ProgramID, "ProjectionMatrix");

    glEnable(GL_FRAMEBUFFER_SRGB);
    glDisable(0x809D); // Disable multisampling
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);

    glUseProgram(State->glContext.ProgramID);
}
