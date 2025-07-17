#include "TestSnake.h"

#include "batch/BatchRenderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <array>

namespace test {

    float x = 0.0f, y = 0.0f;
    unsigned int lenSnake = 0;

    const size_t MaxCount = 1000;
    const size_t MaxVertexCount = MaxCount * 4;
    const size_t MaxIndexCount = MaxCount * 6;
    Vec3 positionsSnake[MaxVertexCount];

    static BatchVertex* Create(BatchVertex* target, float sx, float sy)
    {
        // Setting up Apple
        target->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
        target->Position = positionsSnake[0];
        target++;
        target->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
        target->Position = positionsSnake[1];
        target++;
        target->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
        target->Position = positionsSnake[2];
        target++;
        target->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
        target->Position = positionsSnake[3];
        target++;

        // Setting up Snake head
        float r = 0.5f; float g = 0.3f; float b = 0.8f; float a = 1.0f;
        float size = 50.0f;

        target->Position = { sx, sy, 0.0f };                  positionsSnake[4] = { sx, sy, 0.0f };
        target->Color = { r, g, b, a };
        target++;
        target->Position = { sx + size, sy, 0.0f };           positionsSnake[5] = { sx + size, sy, 0.0f };
        target->Color = { r, g, b, a };
        target++;
        target->Position = { sx + size, sy + size, 0.0f };    positionsSnake[6] = { sx + size, sy + size, 0.0f };
        target->Color = { r, g, b, a };
        target++;
        target->Position = { sx, sy + size, 0.0f };           positionsSnake[7] = { sx, sy + size, 0.0f };
        target->Color = { r, g, b, a };
        target++;

        // Setting up Snake tail
        if (lenSnake > 0)
        {
            float x1 = sx, y1 = sy;
            float x2 = positionsSnake[4 * 2].x, y2 = positionsSnake[4 * 2].y;
            for (unsigned int i = 0; i < lenSnake; i++)
            {
                float diffX = x2 - x1;
                float diffY = y2 - y1;
                float lenVec = sqrt(diffX * diffX + diffY * diffY);
                x2 = x1 + 50.0f * (diffX / lenVec);
                y2 = y1 + 50.0f * (diffY / lenVec);
                size -= 5;

                target->Position = { x2, y2, 0.0f };                  positionsSnake[4 * (i + 2)] = { x2, y2, 0.0f };
                target->Color = { r, g, b, a };
                target++;
                target->Position = { x2 + size, y2, 0.0f };           positionsSnake[4 * (i + 2) + 1] = { x2 + size, y2, 0.0f };
                target->Color = { r, g, b, a };
                target++;
                target->Position = { x2 + size, y2 + size, 0.0f };    positionsSnake[4 * (i + 2) + 2] = { x2 + size, y2 + size, 0.0f };
                target->Color = { r, g, b, a };
                target++;
                target->Position = { x2, y2 + size, 0.0f };           positionsSnake[4 * (i + 2) + 3] = { x2, y2 + size, 0.0f };
                target->Color = { r, g, b, a };
                target++;

                x1 = x2;
                y1 = y2;
                x2 = positionsSnake[4 * (i + 3)].x;
                y2 = positionsSnake[4 * (i + 3)].y;
            }
        }

        return target;
    }

    TestSnake::TestSnake()
        : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_translationA(1, 1, 0)
    {
        for (unsigned int i = 0; i < MaxVertexCount; i++)
            positionsSnake[i] = { 0.0f, 0.0f, 0.0f };

        unsigned int offset = 0;

        unsigned int indices[MaxIndexCount];
        for (size_t i = 0; i < MaxIndexCount; i += 6)
        {
            indices[i + 0] = 0 + offset;
            indices[i + 1] = 1 + offset;
            indices[i + 2] = 2 + offset;

            indices[i + 3] = 2 + offset;
            indices[i + 4] = 3 + offset;
            indices[i + 5] = 0 + offset;

            offset += 4;
        }

        GLCall(glEnable(GL_BLEND));
        GLCall(glDisable(GL_DEPTH_TEST));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_Shader = std::make_unique<Shader>("./res/shaders/Snake.shader");
        m_VAO = std::make_unique<BatchVertexArray>();
        m_VertexBuffer = std::make_unique<BatchVertexBuffer>(MaxVertexCount);
        BatchVertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(4);
        layout.Push<float>(2);
        layout.Push<float>(3);

        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<BatchIndexBuffer>(indices, MaxIndexCount);

        Shader shader();
        m_Shader->Bind();
    }

    TestSnake::~TestSnake()
    {
        m_VertexBuffer->Unbind();
        m_IndexBuffer->Unbind();
        m_VAO->Unbind();
        m_Shader->Unbind();
    }

    // Classe Game
    bool TestSnake::SnakeEatApple()
    {
        if (positionsSnake[4].x > positionsSnake[0].x && positionsSnake[4].x < positionsSnake[1].x)
        {
            if (positionsSnake[4].y < positionsSnake[0].y && positionsSnake[4].y > positionsSnake[3].y)
                return true;

        }

        if (abs(positionsSnake[4].x - positionsSnake[0].x) < 50 && abs(positionsSnake[4].y - positionsSnake[0].y) < 50)
            return true;
        return false;

        if (positionsSnake[4].x < positionsSnake[2].x && positionsSnake[4].y > positionsSnake[2].y)
            return true;
        if (positionsSnake[5].x > positionsSnake[3].x && positionsSnake[5].y > positionsSnake[3].y)
            return true;
        if (positionsSnake[6].x > positionsSnake[0].x && positionsSnake[6].y < positionsSnake[0].y)
            return true;
        if (positionsSnake[7].x < positionsSnake[1].x && positionsSnake[7].y < positionsSnake[1].y)
            return true;
        return false;
    }

    // Classe Game
    void TestSnake::SnakeGrow()
    {
        if (SnakeEatApple())
        {
            float size = 50.0f;
            float ax = rand() % 960;
            float ay = rand() % 540;

            positionsSnake[0] = { ax, ay, 0.0f };
            positionsSnake[1] = { ax + size, ay, 0.0f };
            positionsSnake[2] = { ax + size, ay + size, 0.0f };
            positionsSnake[3] = { ax, ay + size, 0.0f };

            lenSnake++;

            std::cout << "Score: " << lenSnake << std::endl;
        }
    }

    void TestSnake::OnUpdate(float deltaTime)
    {
    }

    void TestSnake::OnRender()
    {
        if (lenSnake < 10)
            SnakeGrow();
        // from here
        if (x > ImGui::GetIO().MousePos.x)
            x = x - 2;
        else
            x = x + 2;

        if (y < 540.0f - ImGui::GetIO().MousePos.y)
            y = y + 2;
        else
            y = y - 2;

        std::array<BatchVertex, 100> vertices;
        BatchVertex* buffer = vertices.data();
        buffer = Create(buffer, x - 25, y - 25);

        m_VertexBuffer->Bind(vertices);

        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        BatchRenderer renderer;

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            //proj = glm::ortho(-2.0f + cos(r), 2.0f + cos(r), -1.5f + sin(r), 1.5f + sin(r), -1.0f, 1.0f);
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
    }

    void TestSnake::OnImGuiRender()
    {
        ImGui::SliderFloat("Translation X", &x, 0.0f, 960.0f);
        ImGui::SliderFloat("Translation Y", &y, 0.0f, 540.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}