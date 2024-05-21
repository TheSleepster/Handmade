#include "Sugar.h"
#include "Sugar_Math.h"
#include "../data/deps/OpenGL/GLL.h"

struct glContext 
{
    GLuint ProgramID; 
};

struct Transform 
{
    vec2 pos;
    vec2 size;
};

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

internal void
InitializeOpenGLRenderer(BumpAllocator *TransientStorage)
{
    LoadGLFunctions();
    glDebugMessageCallback(&OpenGLDebugCallback, nullptr);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_DEBUG_OUTPUT);
    
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    int FileSize = 0;
    char *VertexShader = ReadEntireFileBA("../code/shader/BasicVertexShader.glsl", &FileSize, TransientStorage);
    char *FragmentShader = ReadEntireFileBA("../code/shader/BasicFragmentShader.glsl", &FileSize, TransientStorage);
    Assert(VertexShader, "Failed to load the Vertex Shader!\n");
    Assert(FragmentShader, "Failed to load the Fragment Shader!\n");

    glShaderSource(VertexShaderID, 1, &VertexShader, 0);
    glShaderSource(FragmentShaderID, 1, &FragmentShader, 0);

    glCompileShader(VertexShaderID);
    glCompileShader(FragmentShaderID);
    // NOTE : Local scope to test for the successful compilation of the Fragment shader
    {
        int Success;
        char ShaderLog[2048] = {};

        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Success);
        if(!Success) 
        {
            glGetShaderInfoLog(VertexShaderID, 2048, 0, ShaderLog);
            Assert(false, "Failed to compile the Vertex Shader!\n");
        }
    }

    // NOTE : Local scope to test for the successful compilation of the Fragment shader
    {
        int Success;
        char ShaderLog[2048] = {};

        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Success);
        if(!Success) 
        {
            glGetShaderInfoLog(FragmentShaderID, 2048, 0, ShaderLog);
            Assert(false, "Failed to compile the Fragment Shader!\n");
        }
    }
    glContext glContext = {};

    glContext.ProgramID = glCreateProgram();
    glAttachShader(glContext.ProgramID, VertexShaderID);
    glAttachShader(glContext.ProgramID, FragmentShaderID);
    glLinkProgram(glContext.ProgramID);

    // NOTE : Local scope to test for the successful compilation of the Program
    {
        int Success;
        char ProgramLog[2048] = {};

        glGetProgramiv(glContext.ProgramID, GL_LINK_STATUS, &Success);
        if(!Success) 
        {
            glGetProgramInfoLog(glContext.ProgramID, 2048, 0, ProgramLog);
            Assert(false, "Failed to compile the Fragment Shader!\n");
        }
    }

    glDetachShader(glContext.ProgramID, VertexShaderID);
    glDetachShader(glContext.ProgramID, FragmentShaderID);
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);

    glUseProgram(glContext.ProgramID);
}
