#pragma once

#include "BatchVertex.h"

#include <iostream>
#include <array>

class BatchVertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	BatchVertexBuffer(unsigned int size);
	~BatchVertexBuffer();

	void Bind(std::array<BatchVertex, 100> vertices) const;
	void Unbind() const;
};