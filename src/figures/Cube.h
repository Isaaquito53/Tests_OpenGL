#pragma once

#include "figures/Figure.h"

class Cube : public Figure
{
public:
	Cube();
	~Cube();

	void GetIndices(unsigned int indices[]) override;

	std::array<BatchVertex, 100> m_Vertices;

private:
	unsigned int m_nVertices;
};