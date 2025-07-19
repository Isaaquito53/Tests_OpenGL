#pragma once

#include "batch/BatchVertex.h"

#include <array>

class Figure 
{
public:
	Figure() { m_nVertices = 0; };
	~Figure() {};

	virtual void GetIndices(unsigned int indices[]) {};

	std::array<BatchVertex, 100> m_Vertices;

private:
	unsigned int m_nVertices;
};