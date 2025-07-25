#pragma once

#include "figures/Figure.h"

class RigidBody 
{
public:
	RigidBody();
	RigidBody(Figure* fig, float xi, float yi, float zi, float vyi);

	~RigidBody();

	void ApplyVerticalVelocity(float v);
	void ActGravityY();

	void UpdateCoords(float v);

	Figure& GetFigure() { return *m_fig; };
	float GetXi() { return m_xi; };
	float& GetYi() { return m_yi; };
	float GetYf() { return m_yf; };
	float GetZi() { return m_zi; };
	float& GetVYi() { return m_vyi; };
	float GetVYf() { return m_vyf; };
	float GetR() { return m_boundingR; };

	void SetXi(float xi) { m_xi = xi; };
	void SetYi(float yi) { m_yi = yi; };

	bool m_onAir;
private:
	Figure *m_fig;
	float m_xi;
	float m_yi;
	float m_yf;
	float m_zi;
	float m_vyi;
	float m_vyf;
	
	unsigned int m_goingUp;
	float m_gravity;
	float m_delta;
	float m_boundingR;
};