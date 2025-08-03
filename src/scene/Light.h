#pragma once

#include "scene/figures/Figure.h"

struct Light {
	float m_ambientStrenght;
	float m_diffuseStrenght;
	float m_specularStrenght;
	int m_shininessLevel;
	Vec3 m_color;
	Vec3 m_position;
	Figure* m_fig;
};