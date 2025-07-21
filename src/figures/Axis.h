#pragma once

#include "figures/Figure.h"

class Axis : public Figure
{
public:
	Axis();
	~Axis();

	void GetIndices(unsigned int indices[]) override {};

	std::array<BatchVertex, 100> m_Vertices;

private:
	unsigned int m_nVertices;
};