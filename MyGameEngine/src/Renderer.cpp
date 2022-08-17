#include "Renderer.h"
#include <iostream>

bool GLHandelErrors(const char* function, const char* file, int line) // returns true if there is no errors
{
    bool errorCatched = false;
    while (true)
    {
        GLenum error = glGetError();
        if (error == GL_NO_ERROR) break;
        std::cout << "[OpenGL Error] (" << error << "): " << function << " \n" << file << ": " << line << std::endl;
        errorCatched = true;
    }
    return (errorCatched == false);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind(); // do I really need that?

    /* Says that we need to draw triangle from a buffer data
    which starts at index 0 and contains 3 vectors (vertices) */
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
