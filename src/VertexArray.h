#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void AddInstancedAtt4fmat(int start);
	void AddInstancedAtt3fvec(int start);

	void Bind() const;
	void Unbind() const;
};