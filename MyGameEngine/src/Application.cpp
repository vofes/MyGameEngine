#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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


    // Generate 1 buffer and giving us back an id
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    // Selecting buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // specify data
    float positions[6] = {
        -0.5f,  -0.5f,
        -0.0f,   0.5f,
         0.5f,   -0.5f
    };
    //put data in that buffer
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    // each vertex that we have, contains data (position, texture position)
    // this variables of vertex are attributes, and we have to describe them to openGL
    // we specify: 
    // index of attribute => this is the first one => 0
    // amount of variables in this attribute => for Vector2D this is 2 floats => 2
    // type of variable => float
    // if we want to normilize it => usually not
    // offset between each attribute in bytes => probably shoud be calculated by opengl if it knows type and amount...
    // and offset in bytes from start of attribute?? will be done by macros?? in future

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    // and don't forget to enable it
    glEnableVertexAttribArray(0);

    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //Simple Triangle
        /* Says that we need to draw triangle from a buffer data
        which starts at index 0 and contains 3 vectors (vertices) */
        glDrawArrays(GL_TRIANGLES, 0, 3);
        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}