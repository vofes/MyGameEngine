#include "TestTexture2D.h"
#include "Renderer.h"




#include "imgui/imgui.h"

namespace test {
    TestTexture2D::TestTexture2D() 
        : m_translationA(200.0f, 200.0f, 0.0f), 
        m_translationB(400.0f, 200.0f, 0.0f), 
        m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), 
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
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

        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VAO = std::make_unique<VertexArray>();
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
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
        m_IBO = std::make_unique<IndexBuffer>(indices, 6);
        m_IBO->Bind();

        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Shader->Bind();

        m_Texture = std::make_unique<Texture>("res/textures/transperentTest.png");
        m_Shader->SetUniform1i("u_Texture", 0);
        m_Texture->Bind();
	}

	TestTexture2D::~TestTexture2D()
	{
        m_VAO->Unbind();
        m_VertexBuffer->Unbind();
        m_IBO->Unbind();
        m_Shader->Unbind();
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
        Renderer renderer;
        m_Shader->Bind();
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);
            glm::mat4 mvp = m_Proj * m_View * model; // opengl = column matrics, therefore needs to be multiplied in "right to left" order
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationB);
            glm::mat4 mvp = m_Proj * m_View * model; // opengl = column matrics, therefore needs to be multiplied in "right to left" order
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }
	}

	void TestTexture2D::OnImGuiRender()
	{
        ImGui::SliderFloat3("m_translationA", &m_translationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("m_translationB", &m_translationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}