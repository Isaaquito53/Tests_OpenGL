#include "figures/RigidBody.h"

RigidBody::RigidBody()
{
	m_fig = nullptr;
	m_yi = 0.0f;
	m_yf = 0.0f;
	m_vyi = 0.0f;
	m_vyf = 0.0f;

	m_jumping = false;
	m_goingUp = 0;
	m_gravity = 0.0f;
	m_delta = 0.0f;
}

RigidBody::RigidBody(Figure* fig, float yi, float vyi)
{
	m_fig = fig;
	m_yi = yi;
	m_yf = yi;
	m_vyi = vyi;
	m_vyf = vyi;

	m_jumping = true;
	m_goingUp = 0;
	m_gravity = 9.8f;
	m_delta = 0.0f;
}

RigidBody::~RigidBody()
{
}


void RigidBody::ApplyVerticalVelocity(float v)
{
	m_vyf = v;
	m_goingUp = 1;
	m_jumping = true;
}

float RigidBody::ActGravityY()
{
	if (m_jumping)
	{
		m_delta += 1 / 500.0f;
		if (m_goingUp)
		{
			m_vyf = m_vyf - m_gravity * m_delta;
			if (m_vyf <= 0.0f)
			{
				m_goingUp = 0;
				m_vyf = m_vyi;
			}
		}
		m_yf = -(m_gravity / 2) * (m_delta * m_delta) + m_vyf * m_delta * m_goingUp + m_yf;
		if (m_yf <= 0.5f)
		{
			m_jumping = true;
			m_goingUp = 1;
			m_vyf = 10.0f;
			m_yf = m_yi;
			m_delta = 0.0f;
		}
	}

	return m_yf;
}