#pragma once

#include <GL/glew.h>

#include "BatchVertexArray.h"
#include "BatchIndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLBatchClearError();\
x;\
ASSERT(GLBatchLogCall(#x, __FILE__, __LINE__))

void GLBatchClearError();
bool GLBatchLogCall(const char* function, const char* file, int line);

class BatchRenderer
{
public:
	void Clear() const;
	void Draw(const BatchVertexArray& va, const BatchIndexBuffer& ib, const Shader& shader) const;
	void DrawAxis() const;
};