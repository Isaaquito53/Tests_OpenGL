#include "Camera.h"

#include <iostream>

#include "imgui/imgui.h"

#include <GLFW/glfw3.h>

Camera::Camera()
	: m_FPSMode(true), m_FPSModeLast(true), m_SurvivalMode(false),
	m_camSpeed(5.0f),
	m_camPos(glm::vec3(0.0f, 0.0f, 3.0f)), m_camDirection(glm::vec3(0.0f, 0.0f, 0.0f)), m_camUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_camFront(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_yaw(-90.0f), m_pitch(0.0f),
	m_lastLook(glm::vec2(960 / 2, 540 / 2)), m_sensitivity(1.0f),
	m_x(1.0f), m_v(0.0f), m_goingUp(0), m_gravity(9.8f), m_jumping(false), m_delta(0.0f)
{
}

Camera::~Camera()
{
}

void Camera::IdleMovement(float delta)
{
	m_camSpeed = 5.0f;
	float camX = sin(delta) * m_camSpeed;
	float camZ = cos(delta) * m_camSpeed;

	m_camPos = glm::vec3(camX, 0.0f, camZ);
	m_camDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	m_camUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::Walk()
{
	m_camSpeed = 0.05f;
	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_W)))
	{
		//std::cout << "W" << std::endl;
		m_camPos += m_camSpeed * m_camFront * glm::vec3(1.0f, 0.0f, 1.0f);
	}
	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_S)))
	{
		//std::cout << "S" << std::endl;
		m_camPos -= m_camSpeed * m_camFront * glm::vec3(1.0f, 0.0f, 1.0f);
	}
	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_A)))
	{
		//std::cout << "A" << std::endl;
		m_camPos -= glm::normalize(glm::cross(m_camFront, m_camUp)) * m_camSpeed;
	}
	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_D)))
	{
		//std::cout << "D" << std::endl;
		m_camPos += glm::normalize(glm::cross(m_camFront, m_camUp)) * m_camSpeed;
	}
}

void Camera::Jump()
{
	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Space)) && m_camPos.y == 1.0f)
	{
		m_v = 6.0f;
		m_goingUp = 1;
		m_jumping = true;
	}
	if (m_jumping)
	{
		m_delta += 1 / 500.0f;
		if (m_goingUp)
		{
			m_v = m_v - m_gravity * m_delta;
			if (m_v <= 0.0f)
			{
				m_goingUp = 0;
				m_v = 0.0f;
			}
		}
		m_x = -(m_gravity / 2) * (m_delta * m_delta) + m_v * m_delta * m_goingUp + m_x;
		if (m_x <= 1.0f)
		{
			m_jumping = false;
			m_x = 1.0f;
			m_delta = 0.0f;
		}
	}

	m_camPos.y = m_x;
}

void Camera::Fly()
{
	m_camSpeed = 0.05f;
	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Space)))
	{
		//std::cout << "Space" << std::endl;
		m_camPos += m_camSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
	}
	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Shift)))
	{
		//std::cout << "Space" << std::endl;
		m_camPos -= m_camSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
	}
}

void Camera::Look()
{
	if (ImGui::IsMouseDown(1))
	{
		//std::cout << "mouse clicked" << std::endl;
		float xpos = ImGui::GetIO().MousePos.x;
		float ypos = ImGui::GetIO().MousePos.y;

		float xoffset = xpos - m_lastLook.x;
		float yoffset = m_lastLook.y - ypos;
		m_lastLook.x = xpos;
		m_lastLook.y = ypos;

		xoffset *= m_sensitivity / 10;
		yoffset *= m_sensitivity / 10;

		m_yaw += xoffset;
		m_pitch += yoffset;

		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;

		m_camFront = glm::normalize(glm::vec3(
			cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
			sin(glm::radians(m_pitch)),
			sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))
		));
	}
	else
		m_lastLook = glm::vec2(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
}

void Camera::UpdateCam(float delta)
{
	if (m_FPSMode != m_FPSModeLast)
	{
		m_camPos = glm::vec3(0.0f, 0.0f, 3.0f);
		m_camDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		m_camUp = glm::vec3(0.0f, 1.0f, 0.0f);
		m_FPSModeLast = m_FPSMode;
	}

	if (!m_FPSMode)
	{
		IdleMovement(delta);
	}
	else
	{
		Walk();
		if (m_SurvivalMode)
			Jump();
		else
			Fly();
		Look();
		m_camDirection = m_camPos + m_camFront;
	}
}