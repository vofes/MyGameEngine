#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall()
{
    while (true)
    {
        GLenum error = glGetError();
        if (error == GL_NO_ERROR) return true;
        std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
    }
    
}

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("shader") != std::string::npos)
        {
            // Has found shader on that line
            if (line.find("vertex") != std::string::npos)
            {
                // set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                //set mode to fragment
                type = ShaderType::FRAGMENT;
            }   
        }
        else 
        {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    // string has to exist
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile: " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

//Copied cope from:
//https://www.glfw.org/documentation

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Needs to be after context is created!!!
    GLenum err = glewInit();

    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cout << "Error: \n" << glewGetErrorString(err) << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;


    // data
    float positions[] = {
        -0.5f,  -0.5f, // 0
         0.5f,  -0.5f, // 1
         0.5f,   0.5f, // 2
        -0.5f,   0.5f  // 3
    };

    unsigned int indices[] = {
        0, 1, 2, 
        2, 3, 0
    };

    // Generate 1 buffer and giving us back an id
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    // Selecting buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    
    //put data in that buffer
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), positions, GL_STATIC_DRAW);

    // each vertex that we have, contains data (position, texture position)
    // this variables of vertex are attributes, and we have to describe them to openGL
    // we specify: 
    // index of attribute => this is the first one => 0
    // amount of variables in this attribute => for Vector2D this is 2 floats => 2
    // type of variable => float
    // if we want to normilize it => usually not
    // offset between each attribute in bytes => probably shoud be calculated by opengl if it knows type and amount...
    // and offset in bytes from start of attribute?? will be done by macros?? in future

    
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    // and don't forget to enable it
    glEnableVertexAttribArray(0);

    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //GLClearError();

        //Simple Triangle
        /* Says that we need to draw triangle from a buffer data
        which starts at index 0 and contains 3 vectors (vertices) */
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        GLLogCall();
        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}