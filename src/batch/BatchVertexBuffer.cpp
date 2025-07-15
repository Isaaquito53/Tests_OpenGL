#include "BatchVertexBuffer.h"

#include "BatchRenderer.h"

float x2 = 150.0f, y2 = 150.0f;

BatchVertexBuffer::BatchVertexBuffer(unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(BatchVertex) * size, nullptr, GL_DYNAMIC_DRAW));

}

BatchVertexBuffer::~BatchVertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void BatchVertexBuffer::Bind(std::array<BatchVertex, 100> vertices) const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(BatchVertex), vertices.data());
}

void BatchVertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}