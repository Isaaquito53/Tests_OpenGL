#include "BatchRenderer.h"

#include <iostream>

void GLBatchClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLBatchLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function
            << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

void BatchRenderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void BatchRenderer::Draw(const BatchVertexArray& va, const BatchIndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}