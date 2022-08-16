#include "Renderer.h"
#include <iostream>

bool GLLogCall()
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