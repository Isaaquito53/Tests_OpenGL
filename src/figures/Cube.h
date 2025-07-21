#pragma once

#include "figures/Figure.h"

class Cube : public Figure
{
public:
	Cube();
	Cube(float xi, float vi);
	~Cube();

	void GetIndices(unsigned int indices[]) override;

	std::array<BatchVertex, 100> m_Vertices;
	float m_xi;
	float m_xf;
	float m_vi;
	float m_vf;

private:
	unsigned int m_nVertices;
};