#include "TestGoing3D.h"

#include "batch/BatchRenderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <array>

namespace test {

    // initialize global variables (like buffer sizes or some dynamic content buffers (positions, etc...))
    const size_t MaxCount = 1000;
    const size_t MaxVertexCount = MaxCount * 4;
    const size_t MaxIndexCount = MaxCount * 6;

    float delta = 0.0f;
    float step = 0.0f;

    TestGoing3D::TestGoing3D()
        // initialitze projection and view matrix
        : m_Proj(glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f))),
        m_textureShader(true), m_lastShader(true),
        m_ambientStrenght(0.1f), m_diffuseStrenght(1.0f), m_specularStrenght(0.5f), m_shininessLevel(5),
        m_Texture("./res/textures/FreeSky.png", WrappingClampEdge),
        m_TextureFloor("./res/textures/PS_Logo.png", WrappingRepeat),
        m_Cube(), m_Floor()
    {
        // initialize index buffer
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
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // VBO and VAO stuff
        m_VAO = std::make_unique<BatchVertexArray>();
        m_VertexBuffer = std::make_unique<BatchVertexBuffer>(MaxVertexCount);
        BatchVertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(4);
        layout.Push<float>(2);
        layout.Push<float>(3);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        // IBO stuff
        m_IndexBuffer = std::make_unique<BatchIndexBuffer>(indices, MaxIndexCount);

        // Shaders stuff
        m_Shader = std::make_unique<Shader>("./res/shaders/Going3DTexture.shader");
        Shader shader();
        m_Shader->Bind();

        // setting up some uniforms
        m_Shader->SetUniform1i("u_Texture", 0); // 0 is the slot

        m_ShaderLight = std::make_unique<Shader>("./res/shaders/LightGoing3D.shader");
        Shader shader();
        m_ShaderLight->Bind();
    }

    TestGoing3D::~TestGoing3D()
    {
        m_VertexBuffer->Unbind();
        m_IndexBuffer->Unbind();
        m_VAO->Unbind();
        m_Shader->Unbind();
        m_ShaderLight->Unbind();
        m_Texture.Unbind();
    }

    void TestGoing3D::ChangeShader()
    {
        if (!m_textureShader)
        {
            // Shaders stuff
            m_Shader = std::make_unique<Shader>("./res/shaders/Going3D.shader");
            Shader shader();
            m_Shader->Bind();

            m_Shader->SetUniform3f("u_lightColor", 1.0f, 1.0f, 1.0f);
            m_Shader->SetUniform3f("u_lightPos", 1.2f, 1.0f, 2.0f);
        }
        else
        {
            // Shaders stuff
            m_Shader = std::make_unique<Shader>("./res/shaders/Going3DTexture.shader");
            Shader shader();
            m_Shader->Bind();

            // setting up some uniforms
            m_Shader->SetUniform1i("u_Texture", 0); // 0 is the slot
        }
        m_lastShader = m_textureShader;
    }

    void TestGoing3D::OnUpdate(float deltaTime)
    {
    }

    void TestGoing3D::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        BatchRenderer renderer;

        if (m_lastShader != m_textureShader)
            ChangeShader();

        m_cam.UpdateCam(delta);
        m_View = glm::lookAt(m_cam.m_camPos, m_cam.m_camDirection, m_cam.m_camUp);

        m_VertexBuffer->Bind(m_Cube.m_Vertices);
        m_Texture.Bind();
        // Draw Cube1
        {
            // update model, projection and view matrices
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, delta * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            m_Shader->SetUniformMat4f("u_Model", model);
            if (!m_textureShader)
            {
                m_Shader->SetUniform3f("u_viewPos", m_cam.m_camPos.x, m_cam.m_camPos.y, m_cam.m_camPos.z);
                m_Shader->SetUniform1f("u_ambientStrenght", m_ambientStrenght);
                m_Shader->SetUniform1f("u_diffuseStrenght", m_diffuseStrenght);
                m_Shader->SetUniform1f("u_specularStrenght", m_specularStrenght);
                m_Shader->SetUniform1f("u_shininess", pow(2, m_shininessLevel));
            }

            m_VAO->Bind();

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
        // Draw Cube2
        {
            // update model, projection and view matrices
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, 5.0f, -10.0f));
            model = glm::rotate(model, delta * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            m_Shader->SetUniformMat4f("u_Model", model);
            if (!m_textureShader)
            {
                m_Shader->SetUniform3f("u_viewPos", m_cam.m_camPos.x, m_cam.m_camPos.y, m_cam.m_camPos.z);
                m_Shader->SetUniform1f("u_ambientStrenght", m_ambientStrenght);
                m_Shader->SetUniform1f("u_diffuseStrenght", m_diffuseStrenght);
                m_Shader->SetUniform1f("u_specularStrenght", m_specularStrenght);
                m_Shader->SetUniform1f("u_shininess", pow(2, m_shininessLevel));
            }

            m_VAO->Bind();

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
        // Draw Light Cube
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(1.2f, 2.0f, 2.0f));
            model = glm::scale(model, glm::vec3(0.2f));

            glm::mat4 mvp = m_Proj * m_View * model;
            m_ShaderLight->Bind();
            m_ShaderLight->SetUniformMat4f("u_MVP", mvp);

            m_VAO->Bind();

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_ShaderLight);
        }
        // Draw Floor
        m_VertexBuffer->Bind(m_Floor.m_Vertices);
        m_TextureFloor.Bind();
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            m_Shader->SetUniformMat4f("u_Model", model);
            if (!m_textureShader)
            {
                m_Shader->SetUniform3f("u_viewPos", m_cam.m_camPos.x, m_cam.m_camPos.y, m_cam.m_camPos.z);
                m_Shader->SetUniform1f("u_ambientStrenght", m_ambientStrenght);
                m_Shader->SetUniform1f("u_diffuseStrenght", m_diffuseStrenght);
                m_Shader->SetUniform1f("u_specularStrenght", m_specularStrenght);
                m_Shader->SetUniform1f("u_shininess", pow(2, m_shininessLevel));
            }

            m_VAO->Bind();

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

        delta += step / 100.0f;
    }

    void TestGoing3D::OnImGuiRender()
    {
        ImGui::Text("SCENE PARAMETERS:");
        ImGui::SliderFloat("Velocity", &step, 0.0f, 100.0f);
        ImGui::Text("CAM PARAMETERS:");
        ImGui::SliderFloat("Sensitivity", &m_cam.m_sensitivity, 0.1f, 2.0f);
        ImGui::Checkbox("FPS Mode", &m_cam.m_FPSMode);
        ImGui::Text("TEXTURE PARAMETERS:");
        ImGui::Checkbox("Texture Shader", &m_textureShader);
        ImGui::Text("LIGHT PARAMETERS:");
        ImGui::SliderFloat("Ambient", &m_ambientStrenght, 0.0f, 1.0f);
        ImGui::SliderFloat("Diffuse", &m_diffuseStrenght, 0.0f, 1.0f);
        ImGui::SliderFloat("Specular", &m_specularStrenght, 0.0f, 1.0f);
        ImGui::SliderInt("Specular shineness level", &m_shininessLevel, 0, 8);
        ImGui::Text("CAM COORDS:");
        ImGui::Text("Cam position: x: %.3f, y: %.3f, z: %.3f", m_cam.m_camPos.x, m_cam.m_camPos.y, m_cam.m_camPos.z);
        ImGui::Text("Cam looking at: x: %.3f, y: %.3f, z: %.3f", m_cam.m_camFront.x, m_cam.m_camFront.y, m_cam.m_camFront.z);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}