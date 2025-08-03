#pragma once

#include "Test.h"

#include "batch/BatchVertexBuffer.h"
#include "batch/BatchVertexBufferLayout.h"
#include "Texture.h"
#include "Camera.h"
#include "scene/figures/RigidBody.h"
#include "scene/figures/Cube.h"
#include "scene/figures/Floor.h"
#include "scene/figures/Axis.h"

#include <memory>
#include <vector>

namespace test {

	class TestCollisions :public Test
	{
	public:
		TestCollisions();
		~TestCollisions();

		void ChangeShader();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnPickingObjects(int width, int height) override;
	private:
		void DrawCubes();
		// Render vars
		std::unique_ptr<BatchVertexArray> m_VAO;
		std::unique_ptr<BatchVertexBuffer>m_VertexBuffer;
		std::unique_ptr<BatchIndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Shader> m_ShaderLight;
		std::unique_ptr<Shader> m_ShaderAxis;
		std::unique_ptr<Shader> m_ShaderPicking;
		// Shader vars
		bool m_textureShader;
		bool m_lastShader;
		// Texture vars
		Texture m_Texture;
		Texture m_TextureFloor;
		Texture m_diffuseMap;
		Texture m_specularMap;
		// Transform mat vars
		glm::mat4 m_Proj, m_View;
		// Cam vars
		Camera m_cam;
		// Light vars
		float m_ambientStrenght;
		float m_diffuseStrenght;
		float m_specularStrenght;
		int m_shininessLevel;
		// Objects
		Cube m_Cube;
		Floor m_Floor;
		Axis m_Axis;
		RigidBody m_RBody1;
		// Scene vars
		float m_step;
		float m_delta;
		bool m_play;
		int m_nCubes;
		int m_limit;
		std::vector<glm::vec3> m_cubePositions;
		std::vector<bool> m_cubeCollision;
	};
}