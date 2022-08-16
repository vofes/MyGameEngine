#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

//Copied cope from:
//https://www.glfw.org/documentation

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* fps limitation to V-SYNC*/
    glfwSwapInterval(1);

    // Needs to be after context is created!!!
    GLenum err = glewInit();

    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cout << "Error: \n" << glewGetErrorString(err) << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        // data
        float positions[] = {
            -0.5f,  -0.5f, 0.0f, 0.0f, // 0
             0.5f,  -0.5f, 1.0f, 0.0f, // 1
             0.5f,   0.5f, 1.0f, 1.0f, // 2
            -0.5f,   0.5f, 0.0f, 1.0f // 3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        // 1 - source alpha = destination alpha
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        // each vertex that we have, contains data (position, texture position)
        // this variables of vertex are attributes, and we have to describe them to openGL
        // we specify: 
        // index of attribute => this is the first one => 0
        // amount of variables in this attribute => for Vector2D this is 2 floats => 2
        // type of variable => float
        // if we want to normilize it => usually not
        // offset between each attribute in bytes => probably shoud be calculated by opengl if it knows type and amount...
        // and offset in bytes from start of attribute?? will be done by macros?? in future
        // and don't forget to enable it


        // Index buffer
        IndexBuffer ib(indices, 6);
        ib.Bind();

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        Texture texture("res/textures/transperentTest.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);
        

        float timer = 0.0f;

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;
        

        

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */

            renderer.Clear();
            renderer.Draw(va, ib, shader);
            GLHandelErrors();


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        shader.Unbind();
    }

    glfwTerminate();
    return 0;
}