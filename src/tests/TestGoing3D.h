#pragma once

#include "Test.h"

#include "batch/BatchVertexBuffer.h"
#include "batch/BatchVertexBufferLayout.h"
#include "Texture.h"
#include "Camera.h"
#include "figures/Cube.h"
#include "figures/Floor.h"

#include <memory>

namespace test {
	
	class TestGoing3D :public Test
	{
	public:
		TestGoing3D();
		~TestGoing3D();

		void ChangeShader();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		bool m_textureShader;
		bool m_lastShader;

		std::unique_ptr<BatchVertexArray> m_VAO;
		std::unique_ptr<BatchVertexBuffer>m_VertexBuffer;
		std::unique_ptr<BatchIndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Shader> m_ShaderLight;
		Texture m_Texture;
		Texture m_TextureFloor;
		Camera m_cam;

		glm::mat4 m_Proj, m_View;

		float m_ambientStrenght;
		float m_diffuseStrenght;
		float m_specularStrenght;
		int m_shininessLevel;

		Cube m_Cube;
		Floor m_Floor;
	};
}