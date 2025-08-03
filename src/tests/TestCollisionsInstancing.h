#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "batch/BatchVertexBuffer.h"
#include "batch/BatchVertexBufferLayout.h"
#include "Texture.h"
#include "Camera.h"
#include "scene/Scene.h"

#include <memory>
#include <vector>

namespace test {
	
	class TestCollisionsInstancing :public Test
	{
	public:
		TestCollisionsInstancing();
		~TestCollisionsInstancing();

		void ChangeShader();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnPickingObjects(int width, int height) override;
	private:
		void CollideDetection(float delta);
		// Render vars
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<VertexBuffer> m_posVBO;
		std::unique_ptr<IndexBuffer> m_EBO;
		std::unique_ptr<BatchVertexArray> m_bVAO;
		std::unique_ptr<BatchVertexBuffer>m_bVertexBuffer;
		std::unique_ptr<BatchIndexBuffer> m_bIndexBuffer;
		std::unique_ptr<Shader> m_SingleShader;
		std::unique_ptr<Shader> m_InstancedShader;
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
		//Scene
		Scene m_scene;
		// Scene vars
		float m_step;
		float m_delta;
		bool m_play;
	};
}