#pragma once

#include <GL/glew.h>
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) x;\
	ASSERT(GLHandelErrors(#x, __FILE__, __LINE__))

bool GLHandelErrors(const char* function, const char* file, int line);

class Renderer
{
private:

public:
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void Clear() const;
};