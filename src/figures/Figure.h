#pragma once

#include "batch/BatchVertex.h"

#include <array>

class Figure 
{
public:
	Figure() { m_nVertices = 0; };
	~Figure() {};

	std::array<BatchVertex, 100> m_Vertices;

protected:
	unsigned int m_nVertices;
};