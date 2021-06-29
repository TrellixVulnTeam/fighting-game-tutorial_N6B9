#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <filesystem>

SDL_Window* sdlWindow = nullptr;
GLuint triangleVao = 0;
GLuint triangleShader = 0;

std::string load_data_from_file(const std::string& path)
{
    std::string result;

    std::ifstream fileStream(path, std::ios::in | std::ios::ate);
    if (fileStream.is_open())
    {
        int size = static_cast<int>(fileStream.tellg());
        if (size > 0)
        {
            fileStream.seekg(0, std::ios::beg);
            result.resize(size);
            fileStream.read(&result[0], size);
        }

        fileStream.close();
    }

    return result;
}

bool load_shader_from_file(const std::string& shaderPath, GLuint* outShaderId, GLuint shaderType)
{
    *outShaderId = glCreateShader(shaderType);

    std::string shaderCode = load_data_from_file(shaderPath);

    std::cout << "Compiling shader: " << shaderPath << std::endl;

    // Compile shader
    const char* shaderSourcePtr = shaderCode.c_str();
    glShaderSource(*outShaderId, 1, &shaderSourcePtr, nullptr);
    glCompileShader(*outShaderId);

    // Check for errors
    GLint result = GL_FALSE;
    glGetShaderiv(*outShaderId, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int errorLogLength;
        glGetShaderiv(*outShaderId, GL_INFO_LOG_LENGTH, &errorLogLength);
        if (errorLogLength > 0)
        {
            char* buf = new char[errorLogLength + 1];
            glGetShaderInfoLog(*outShaderId, errorLogLength, nullptr, buf);
            buf[errorLogLength] = '\0';
            std::cerr << "Failed to compile shader! Message: " << buf << std::endl;
            delete[] buf;
        }

        return false;
    }

    return true;
}

bool load_shader_program(const std::string& programPath, GLuint* outProgramId)
{
    bool success = false;

    *outProgramId = glCreateProgram();

    GLuint vertexShader;
    GLuint fragmentShader;

    std::filesystem::path shadersPath = std::filesystem::current_path() / ".." / "assets" / "shaders";
    bool vertexShaderOk = load_shader_from_file((shadersPath / (programPath + ".vs")).string(), &vertexShader, GL_VERTEX_SHADER);
    bool fragmentShaderOk = load_shader_from_file((shadersPath / (programPath + ".fs")).string(), &fragmentShader, GL_FRAGMENT_SHADER);

    if (vertexShaderOk && fragmentShaderOk)
    {
        // Link shader
        glAttachShader(*outProgramId, vertexShader);
        glAttachShader(*outProgramId, fragmentShader);
        glLinkProgram(*outProgramId);

        // Check for errors
        GLint result = GL_FALSE;
        glGetProgramiv(*outProgramId, GL_LINK_STATUS, &result);
        if (result == GL_FALSE)
        {
            int errorLogLength;
            glGetProgramiv(*outProgramId, GL_INFO_LOG_LENGTH, &errorLogLength);
            if (errorLogLength > 0)
            {
                char* buf = new char[errorLogLength + 1];
                glGetProgramInfoLog(*outProgramId, errorLogLength, nullptr, buf);
                buf[errorLogLength] = '\0';
                std::cerr << "Failed to link shader program! Message: " << buf << std::endl;
                delete[] buf;
            }
        }
        else
        {
            success = true;
            std::cout << "Shader program linked successfully: " << programPath << std::endl;
        }

        // Detach shader
        glDetachShader(*outProgramId, vertexShader);
        glDetachShader(*outProgramId, fragmentShader);
    }

    // Destroy shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return success;
}

void render_frame()
{
    glClearColor(0.f, 0.5f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw triangle
    glBindVertexArray(triangleVao);
    glUseProgram(triangleShader);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    SDL_GL_SwapWindow(sdlWindow);
}

int main()
{
    std::cout << "Program has started" << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Failed to init SDL!" << std::endl;
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    sdlWindow = SDL_CreateWindow("Fighting Tutorial", 50, 50, 1600, 900, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (sdlWindow)
    {
        SDL_GLContext glContext = SDL_GL_CreateContext(sdlWindow);
        if (!glContext)
        {
            std::cerr << "Failed to initialize OpenGL context: " << SDL_GetError() << std::endl;
            return 1;
        }
        else
        {
            glewInit();

            if (load_shader_program("triangle_shader", &triangleShader))
            {
                float triangleVertices[] = {
                        -1.f, -1.f,
                        1.f, -1.f,
                        0.f, 1.f,
                };

                glGenVertexArrays(1, &triangleVao);
                glBindVertexArray(triangleVao);

                GLuint vbo;
                glGenBuffers(1, &vbo);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);
            }
        }

        bool shutdownRequested = false;
        while (!shutdownRequested)
        {
            SDL_Event ev;
            while (SDL_PollEvent(&ev))
            {
                switch (ev.type)
                {
                    case SDL_QUIT:
                        shutdownRequested = true;
                        break;
                }
            }

            render_frame();
        }
    }

    SDL_Quit();

    return 0;
}
