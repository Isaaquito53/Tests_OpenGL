#pragma once

#include "figures/Figure.h"

class Cube : public Figure
{
public:
	Cube();
	Cube(float xi);
	~Cube();

	void GetIndices(unsigned int indices[]) override;

	std::array<BatchVertex, 100> m_Vertices;
	float m_xi;
	float m_xf;

private:
	unsigned int m_nVertices;
};