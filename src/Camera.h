#pragma once

#include "glm/glm.hpp"

class Camera
{
public:
	// Modes
	bool m_FPSMode;
	bool m_FPSModeLast;
	bool m_SurvivalMode;
	// Cam vectors
	glm::vec3 m_camPos;
	glm::vec3 m_camFront;
	glm::vec3 m_camDirection;
	glm::vec3 m_camUp;
	// Look stuff
	glm::vec2 m_lastLook;
	float m_sensitivity;
	float m_yaw;
	float m_pitch;
	float m_camSpeed;
	// Survival stuff
	float m_v;
	float m_x;
	bool m_jumping;
	unsigned int m_goingUp;
	float m_gravity;
	float m_delta;


	Camera();
	~Camera();

	void IdleMovement(float delta);
	void Walk();
	void Jump();
	void Fly();
	void Look();
	void UpdateCam(float delta);
};