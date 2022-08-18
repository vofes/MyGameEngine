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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
             -50.0f, -50.0f, 0.0f, 0.0f, // 0
              50.0f, -50.0f, 1.0f, 0.0f, // 1
              50.0f,  50.0f, 1.0f, 1.0f, // 2
             -50.0f,  50.0f, 0.0f, 1.0f // 3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        // 1 - source alpha = destination alpha
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

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

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));


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

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
        ImGui::StyleColorsDark();

        glm::vec3 translationA(200.0f, 200.0f, 0.0f);
        glm::vec3 translationB(400.0f, 200.0f, 0.0f);
        
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {

            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            shader.Bind();

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model; // opengl = column matrics, therefore needs to be multiplied in "right to left" order
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model; // opengl = column matrics, therefore needs to be multiplied in "right to left" order
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            {
                ImGui::Begin("Hello, world!");
                ImGui::SliderFloat3("TranslationA", &translationA.x, 0.0f, 960.0f);
                ImGui::SliderFloat3("TranslationB", &translationB.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        shader.Unbind();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}