#pragma once

#include "glm/glm.hpp"

class Camera
{
public:
	bool m_FPSMode;
	bool m_FPSModeLast;
	glm::vec3 m_camPos;
	glm::vec3 m_camFront;
	glm::vec3 m_camDirection;
	glm::vec3 m_camUp;
	glm::vec2 m_lastLook;
	float m_sensitivity;
	float m_yaw;
	float m_pitch;
	float m_camSpeed;
	float m_lastDelta;

	Camera();
	~Camera();

	void IdleMovement(float delta);
	void Walk();
	void Look();
	void UpdateCam(float delta);
};