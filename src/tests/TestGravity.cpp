#include "TestGravity.h"

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

    TestGravity::TestGravity()
        // initialitze projection and view matrix
        : m_Proj(glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f))),
        m_textureShader(true), m_lastShader(true),
        m_ambientStrenght(0.1f), m_diffuseStrenght(1.0f), m_specularStrenght(0.5f), m_shininessLevel(5),
        m_Texture("./res/textures/FreeSky.png", WrappingClampEdge),
        m_TextureFloor("./res/textures/PS_Logo.png", WrappingRepeat),
        m_diffuseMap("./res/textures/container2.png", WrappingRepeat),
        m_specularMap("./res/textures/container2_specular.png", WrappingRepeat),
        m_Cube(), m_Floor(), m_Axis(), 
        m_RBody1(&m_Cube, -5.0f, 0.5f, -5.0f, 0.0f), m_RBody2(&m_Cube, -5.0f, 2.0f, -5.0f, 0.0f),
        m_gravity(9.8f), m_step(1.0f), m_delta(0.0f), m_play(false)
    {
        m_RBody1.ApplyVerticalVelocity(0.0f);
        m_RBody2.ApplyVerticalVelocity(0.0f);

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
        layout.Push<float>(3); // Pos
        layout.Push<float>(4); // Color
        layout.Push<float>(2); // TexCoords
        layout.Push<float>(3); // Norm
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        // IBO stuff
        m_IndexBuffer = std::make_unique<BatchIndexBuffer>(indices, MaxIndexCount);

        // Main Shaders stuff
        m_Shader = std::make_unique<Shader>("./res/shaders/Material.shader");
        m_Shader->Bind();

        // setting up some uniforms
        m_Shader->SetUniform3f("light.position", 1.2f, 1.0f, 2.0f);
        m_Shader->SetUniform1i("material.diffuse", 2); // 2 is the slot
        m_Shader->SetUniform1i("material.specular", 3); // 3 is the slot

        // Light Shader stuff
        m_ShaderLight = std::make_unique<Shader>("./res/shaders/LightGoing3D.shader");
        m_ShaderLight->Bind();

        // Axis Shader stuff
        m_ShaderAxis = std::make_unique<Shader>("./res/shaders/OnlyColor.shader");
        m_ShaderAxis->Bind();

        // Picking objects Shader stuff
        m_ShaderPicking = std::make_unique<Shader>("./res/shaders/Picking.shader");
        m_ShaderPicking->Bind();
    }

    TestGravity::~TestGravity()
    {
        m_VertexBuffer->Unbind();
        m_IndexBuffer->Unbind();
        m_VAO->Unbind();
        m_Shader->Unbind();
        m_ShaderLight->Unbind();
        m_Texture.Unbind();
    }

    void TestGravity::ChangeShader()
    {
        if (!m_textureShader)
        {
            // Shaders stuff
            m_Shader = std::make_unique<Shader>("./res/shaders/Cel.shader");
            Shader shader();
            m_Shader->Bind();

            m_Shader->SetUniform3f("u_lightColor", 1.0f, 1.0f, 1.0f);
            m_Shader->SetUniform3f("u_lightPos", 1.2f, 1.0f, 2.0f);
        }
        else
        {
            // Shaders stuff
            m_Shader = std::make_unique<Shader>("./res/shaders/Material.shader");
            Shader shader();
            m_Shader->Bind();

            // setting up some uniforms
            m_Shader->SetUniform3f("light.position", 1.2f, 1.0f, 2.0f);
            m_Shader->SetUniform1i("material.diffuse", 2); // 2 is the slot
            m_Shader->SetUniform1i("material.specular", 3); // 3 is the slot
        }
        m_lastShader = m_textureShader;
    }

    void TestGravity::OnUpdate(float deltaTime)
    {
        m_cam.UpdateCam(deltaTime);
    }

    void TestGravity::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        BatchRenderer renderer;

        if (m_lastShader != m_textureShader)
            ChangeShader();

        m_View = glm::lookAt(m_cam.m_camPos, m_cam.m_camDirection, m_cam.m_camUp);

        // Draw Axis
        m_VertexBuffer->Bind(m_Axis.m_Vertices);
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(5.0f));

            glm::mat4 mvp = m_Proj * m_View * model;
            m_ShaderAxis->Bind();
            m_ShaderAxis->SetUniformMat4f("u_MVP", mvp);

            m_VAO->Bind();

            renderer.DrawAxis();
        }

        // Draw Cube1
        m_VertexBuffer->Bind(m_RBody1.GetFigure().m_Vertices);
        {
            // update model, projection and view matrices
            if (m_play)
                m_RBody1.UpdateCoords(10.0f);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(m_RBody1.GetXi(), m_RBody1.GetYf(), m_RBody1.GetZi()));

            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            m_Shader->SetUniformMat4f("u_Model", model);
            m_Shader->SetUniform3f("u_viewPos", m_cam.m_camPos.x, m_cam.m_camPos.y, m_cam.m_camPos.z);
            if (!m_textureShader)
            {
                m_Shader->SetUniform1f("u_ambientStrenght", m_ambientStrenght);
                m_Shader->SetUniform1f("u_diffuseStrenght", m_diffuseStrenght);
                m_Shader->SetUniform1f("u_specularStrenght", m_specularStrenght);
                m_Shader->SetUniform1f("u_shininess", pow(2, m_shininessLevel));
            }
            else
            {
                m_Shader->SetUniform3f("light.ambient", m_ambientStrenght, m_ambientStrenght, m_ambientStrenght);
                m_Shader->SetUniform3f("light.diffuse", m_diffuseStrenght, m_diffuseStrenght, m_diffuseStrenght);
                m_Shader->SetUniform3f("light.specular", m_specularStrenght, m_specularStrenght, m_specularStrenght);
                m_Shader->SetUniform1f("material.shininess", pow(2, m_shininessLevel));
            }

            m_diffuseMap.Bind(2);
            m_specularMap.Bind(3);

            m_VAO->Bind();

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

        // Draw Cube2
        m_VertexBuffer->Bind(m_RBody2.GetFigure().m_Vertices);
        {
            // update model, projection and view matrices
            if (m_play)
                m_RBody2.UpdateCoords(10.0f);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(m_RBody2.GetXi(), m_RBody2.GetYf(), m_RBody2.GetZi()));

            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            m_Shader->SetUniformMat4f("u_Model", model);
            m_Shader->SetUniform3f("u_viewPos", m_cam.m_camPos.x, m_cam.m_camPos.y, m_cam.m_camPos.z);
            if (!m_textureShader)
            {
                m_Shader->SetUniform1f("u_ambientStrenght", m_ambientStrenght);
                m_Shader->SetUniform1f("u_diffuseStrenght", m_diffuseStrenght);
                m_Shader->SetUniform1f("u_specularStrenght", m_specularStrenght);
                m_Shader->SetUniform1f("u_shininess", pow(2, m_shininessLevel));
            }
            else
            {
                m_Shader->SetUniform3f("light.ambient", m_ambientStrenght, m_ambientStrenght, m_ambientStrenght);
                m_Shader->SetUniform3f("light.diffuse", m_diffuseStrenght, m_diffuseStrenght, m_diffuseStrenght);
                m_Shader->SetUniform3f("light.specular", m_specularStrenght, m_specularStrenght, m_specularStrenght);
                m_Shader->SetUniform1f("material.shininess", pow(2, m_shininessLevel));
            }

            m_diffuseMap.Bind(2);
            m_specularMap.Bind(3);

            m_VAO->Bind();

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

        float dist = glm::distance(glm::vec3(m_RBody1.GetXi(), m_RBody1.GetYf(), m_RBody1.GetZi()),
            glm::vec3(m_RBody2.GetXi(), m_RBody2.GetYf(), m_RBody2.GetZi()));

        if (dist <= m_RBody1.GetR()*2)
        {
            m_RBody1.m_onAir = false;
            m_RBody2.m_onAir = false;
            //std::cout << "Touché" << std::endl;
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
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(10.0f));

            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            m_Shader->SetUniformMat4f("u_Model", model);
            m_Shader->SetUniform3f("u_viewPos", m_cam.m_camPos.x, m_cam.m_camPos.y, m_cam.m_camPos.z);
            if (!m_textureShader)
            {
                m_Shader->SetUniform1f("u_ambientStrenght", m_ambientStrenght);
                m_Shader->SetUniform1f("u_diffuseStrenght", m_diffuseStrenght);
                m_Shader->SetUniform1f("u_specularStrenght", m_specularStrenght);
                m_Shader->SetUniform1f("u_shininess", pow(2, m_shininessLevel));
            }
            else
            {
                m_Shader->SetUniform3f("light.ambient", m_ambientStrenght, m_ambientStrenght, m_ambientStrenght);
                m_Shader->SetUniform3f("light.diffuse", m_diffuseStrenght, m_diffuseStrenght, m_diffuseStrenght);
                m_Shader->SetUniform3f("light.specular", m_specularStrenght, m_specularStrenght, m_specularStrenght);
                m_Shader->SetUniform1f("material.shininess", pow(2, m_shininessLevel));
            }

            m_diffuseMap.Bind(2);
            m_specularMap.Bind(3);

            m_VAO->Bind();

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

        m_delta += m_step / 120.0f;
    }

    void TestGravity::OnImGuiRender()
    {
        if (ImGui::TreeNode("SCENE PARAMETERS:"))
        {
            ImGui::Checkbox("Play/Pause", &m_play);
            ImGui::SliderFloat("Cube initial Y Pos", &m_RBody1.GetYi(), 0.0f, 50.0f);
            ImGui::Text("Current Cube Y Pos: %.3f", m_RBody1.GetYf());
            ImGui::SliderFloat("Cube initial Y Velocity", &m_RBody1.GetVYi(), 0.0f, 50.0f);
            ImGui::Text("Current Cube Y Velocity: %.3f", m_RBody1.GetVYf());
            ImGui::TreePop();
        }

        if(ImGui::TreeNode("CAM PARAMETERS:"))
        {
            ImGui::SliderFloat("Sensitivity", &m_cam.m_sensitivity, 0.1f, 2.0f);
            ImGui::Checkbox("FPS Mode", &m_cam.m_FPSMode);
            ImGui::Checkbox("Survival FPS Mode", &m_cam.m_SurvivalMode);

            if (ImGui::TreeNode("CAM COORDS:"))
            {
                ImGui::Text("Cam position: x: %.3f, y: %.3f, z: %.3f", m_cam.m_camPos.x, m_cam.m_camPos.y, m_cam.m_camPos.z);
                ImGui::Text("Cam looking at: x: %.3f, y: %.3f, z: %.3f", m_cam.m_camDirection.x, m_cam.m_camDirection.y, m_cam.m_camDirection.z);
                ImGui::TreePop();
            }

            ImGui::TreePop();
        }

        if(ImGui::TreeNode("TEXTURE PARAMETERS:"))
        {
            ImGui::Checkbox("Texture Shader", &m_textureShader);
            ImGui::TreePop();
        }

        if(ImGui::TreeNode("LIGHT PARAMETERS:"))
        {
            ImGui::SliderFloat("Ambient", &m_ambientStrenght, 0.0f, 1.0f);
            ImGui::SliderFloat("Diffuse", &m_diffuseStrenght, 0.0f, 1.0f);
            ImGui::SliderFloat("Specular", &m_specularStrenght, 0.0f, 1.0f);
            ImGui::SliderInt("Specular shineness level", &m_shininessLevel, 0, 8);
            ImGui::TreePop();
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    void TestGravity::OnPickingObjects(int width, int height)
    {
        Vec4 cubeColor = { 1.0f, 0.0f, 0.0f, 1.0f };
        Vec4 lightColor = { 0.9f, 0.0f, 0.0f, 1.0f };

        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        BatchRenderer renderer;

        m_View = glm::lookAt(m_cam.m_camPos, m_cam.m_camDirection, m_cam.m_camUp);

        // Draw Cube1
        m_VertexBuffer->Bind(m_RBody1.GetFigure().m_Vertices);
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(m_RBody1.GetXi(), m_RBody1.GetYf(), -5.0f));

            glm::mat4 mvp = m_Proj * m_View * model;
            m_ShaderPicking->Bind();
            m_ShaderPicking->SetUniformMat4f("u_MVP", mvp);
            m_ShaderPicking->SetUniform4f("u_Color", cubeColor.r, cubeColor.g, cubeColor.b, cubeColor.a);

            m_VAO->Bind();

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_ShaderPicking);
        }

        // Draw Light Cube
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(1.2f, 2.0f, 2.0f));
            model = glm::scale(model, glm::vec3(0.2f));

            glm::mat4 mvp = m_Proj * m_View * model;
            m_ShaderPicking->Bind();
            m_ShaderPicking->SetUniformMat4f("u_MVP", mvp);
            m_ShaderPicking->SetUniform4f("u_Color", lightColor.r, lightColor.g, lightColor.b, lightColor.a);

            m_VAO->Bind();

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_ShaderPicking);
        }

        if (ImGui::IsMouseClicked(0))
        {
            GLfloat pixel;
            ImVec2 mousePos = ImGui::GetIO().MousePos;
            GLint x_coord = mousePos.x;
            GLint y_coord = height - mousePos.y;

            GLCall(glReadPixels(x_coord, y_coord, 1, 1, GL_RED, GL_FLOAT, &pixel));
            //std::cout << "Pixel value: " << pixel << std::endl;

            if (pixel >= cubeColor.r)
            {
                m_RBody1.SetYi(0.0f);
            }
            else if (pixel <= lightColor.r + 0.01f && pixel >= lightColor.r - 0.01f)
            {
                m_diffuseStrenght = ((int)m_diffuseStrenght + 1) % 2;
                m_specularStrenght = ((int)m_specularStrenght + 1) % 2;
            }
        }
    }
}