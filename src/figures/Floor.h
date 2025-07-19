#pragma once

#include "figures/Figure.h"

class Floor : public Figure
{
public:
	Floor();
	~Floor();

	void GetIndices(unsigned int indices[]) override;

	std::array<BatchVertex, 100> m_Vertices;

private:
	unsigned int m_nVertices;
};