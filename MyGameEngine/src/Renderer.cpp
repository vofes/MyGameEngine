#include "Renderer.h"
#include <iostream>

bool GLHandelErrors()
{
    bool errorCatched = false;
    while (true)
    {
        GLenum error = glGetError();
        if (error == GL_NO_ERROR) return true;
        std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
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
    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}
