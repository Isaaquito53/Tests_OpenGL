#pragma once

#include "BatchVertexBuffer.h"

#include <iostream>
#include <array>

class BatchVertexBufferLayout;

class BatchVertexArray
{
private:
	unsigned int m_RendererID;
public:
	BatchVertexArray();
	~BatchVertexArray();

	void AddBuffer(const BatchVertexBuffer& vb, const BatchVertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};