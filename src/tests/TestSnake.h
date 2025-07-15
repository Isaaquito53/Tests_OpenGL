#pragma once

#include "Test.h"

#include "batch/BatchVertexBuffer.h"
#include "batch/BatchVertexBufferLayout.h"

#include <memory>

namespace test {
	
	class TestSnake :public Test
	{
	public:
		TestSnake();
		~TestSnake();

		//bool SnakeEatApple();
		//void SnakeGrow();
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr<BatchVertexArray> m_VAO;
		std::unique_ptr<BatchVertexBuffer>m_VertexBuffer;
		std::unique_ptr<BatchIndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_translationA;
	};
}