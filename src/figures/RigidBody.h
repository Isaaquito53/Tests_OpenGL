#pragma once

#include "figures/Figure.h"

class RigidBody 
{
public:
	RigidBody();
	RigidBody(Figure* fig, float yi, float vyi);

	~RigidBody();

	void ApplyVerticalVelocity(float v);

	float ActGravityY();

	Figure& GetFigure() { return *m_fig; };
	float& GetYi() { return m_yi; };
	float GetYf() { return m_yf; };
	float& GetVYi() { return m_vyi; };
	float GetVYf() { return m_vyf; };

	void SetYi(float yi) { m_yi = yi; };

private:
	Figure *m_fig;
	float m_yi;
	float m_yf;
	float m_vyi;
	float m_vyf;
	bool m_jumping;
	unsigned int m_goingUp;
	float m_gravity;
	float m_delta;
};