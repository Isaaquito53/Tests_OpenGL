#include "TestCollisionsInstancing.h"

#include "batch/BatchRenderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <array>

namespace test {

    // initialize global variables (like buffer sizes or some dynamic content buffers (positions, etc...))
    const size_t indexInstanceCount = 6 * 6;
    const size_t MaxCount = 1000;
    const size_t MaxVertexCount = MaxCount * 6;     // Support for MaxCount Cubes (6 vertices per cube)
    const size_t MaxIndexCount = MaxCount * 6 * 6;  // Support for MaxCount Cubes (6 index per face)

    TestCollisionsInstancing::TestCollisionsInstancing()
        // initialitze projection and view matrix
        : m_Proj(glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f))),
        m_textureShader(false), m_lastShader(false),
        m_ambientStrenght(0.1f), m_diffuseStrenght(1.0f), m_specularStrenght(0.5f), m_shininessLevel(5),
        m_Texture("./res/textures/FreeSky.png", WrappingClampEdge),
        m_TextureFloor("./res/textures/PS_Logo.png", WrappingRepeat),
        m_diffuseMap("./res/textures/container2.png", WrappingRepeat),
        m_specularMap("./res/textures/container2_specular.png", WrappingRepeat),
        m_Cube(), m_Floor(), m_Axis(),
        m_RBody1(&m_Cube, -5.0f, 0.5f, -5.0f, 0.0f),
        m_step(1.0f), m_delta(0.0f), m_play(false), m_nCubes(100), m_limit(5), m_cubePositions()
    {
        // initialize index buffer
        unsigned int offset = 0;
        unsigned int indicesInstance[indexInstanceCount];
        for (size_t i = 0; i < indexInstanceCount; i += 6)
        {
            indicesInstance[i + 0] = 0 + offset;
            indicesInstance[i + 1] = 1 + offset;
            indicesInstance[i + 2] = 2 + offset;

            indicesInstance[i + 3] = 2 + offset;
            indicesInstance[i + 4] = 3 + offset;
            indicesInstance[i + 5] = 0 + offset;

            offset += 4;
        }

        offset = 0;
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

        // set states (Enable/Disable)
        GLCall(glEnable(GL_BLEND));
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // Gen VBO, EBO and VAO for the cube
        m_VAO = std::make_unique<VertexArray>();
        m_VBO = std::make_unique<VertexBuffer>(&m_Cube.m_Vertices[0], m_Cube.m_Vertices.size() * sizeof(BatchVertex));


        // Add normal attributes
        VertexBufferLayout layoutInstanced;
        layoutInstanced.Push<float>(3); // Pos (0)
        layoutInstanced.Push<float>(4); // Color (1)
        layoutInstanced.Push<float>(2); // TexCoords (2)
        layoutInstanced.Push<float>(3); // Norm (3)
        m_VAO->AddBuffer(*m_VBO, layoutInstanced);

        // VBO and VAO stuff
        m_bVAO = std::make_unique<BatchVertexArray>();
        m_bVertexBuffer = std::make_unique<BatchVertexBuffer>(MaxVertexCount);
        BatchVertexBufferLayout layout;
        layout.Push<float>(3); // Pos
        layout.Push<float>(4); // Color
        layout.Push<float>(2); // TexCoords
        layout.Push<float>(3); // Norm
        m_bVAO->AddBuffer(*m_bVertexBuffer, layout);

        // IBO stuff
        m_bIndexBuffer = std::make_unique<BatchIndexBuffer>(indices, MaxIndexCount);

        // Generate instancing attributes
        unsigned int circles = 10;
        m_currentPos = new glm::vec3[m_nCubes];
        m_directionVectors = new glm::vec3[m_nCubes];
        srand(0);
        for (unsigned int k = 1; k <= circles; k++)
        {
            for (unsigned int i = 0; i < m_nCubes/circles; i++)
            {
                m_currentPos[i+(k-1)*m_nCubes/circles] = glm::vec3(
                    k*m_limit * cos(glm::radians(i * 360.0f / (m_nCubes / circles))),           // px
                    0.0f,                                                                       // py
                    k*m_limit * sin(glm::radians(i * 360.0f / (m_nCubes / circles))));          // pz

                glm::vec3 dir = glm::vec3(
                    -m_currentPos[i + (k - 1) * m_nCubes / circles].x/k,       // vx
                    0.0f,                       // vy
                    -m_currentPos[i + (k - 1) * m_nCubes / circles].z/k);      // vz
                m_directionVectors[i + (k - 1) * m_nCubes / circles] = dir;
            }
        }

        /*for (unsigned int i = 0; i < m_nCubes / 3; i++)
        {
            m_currentPos[i] = glm::vec3(
                m_limit * cos(glm::radians(i * 360.0f / (m_nCubes / 3))),             // px
                0.0f,                                                           // py
                m_limit * sin(glm::radians(i * 360.0f / (m_nCubes / 3))));            // pz

            glm::vec3 dir = glm::vec3(
                -m_currentPos[i].x,                         // vx
                0.0f,                                       // vy
                -m_currentPos[i].z);                        // vz
            m_directionVectors[i] = dir;
        }

        for (unsigned int i = m_nCubes / 3; i < 2*m_nCubes / 3; i++)
        {
            m_currentPos[i] = glm::vec3(
                2 * m_limit * cos(glm::radians(i * 360.0f / (m_nCubes / 3))),             // px
                0.0f,                                                           // py
                2 * m_limit * sin(glm::radians(i * 360.0f / (m_nCubes / 3))));            // pz

            glm::vec3 dir = glm::vec3(
                -m_currentPos[i].x / 2,                         // vx
                0.0f,                                       // vy
                -m_currentPos[i].z / 2);                        // vz
            m_directionVectors[i] = dir;
        }

        for (unsigned int i = 2*m_nCubes / 3; i < m_nCubes; i++)
        {
            m_currentPos[i] = glm::vec3(
                3 * m_limit * cos(glm::radians(i * 360.0f / (m_nCubes / 3))),             // px
                0.0f,                                                           // py
                3 * m_limit * sin(glm::radians(i * 360.0f / (m_nCubes / 3))));            // pz

            glm::vec3 dir = glm::vec3(
                -m_currentPos[i].x / 3,                         // vx
                0.0f,                                       // vy
                -m_currentPos[i].z / 3);                        // vz
            m_directionVectors[i] = dir;
        }*/

        /*
        m_currentPos[0] = glm::vec3(
                3.0f,               // px
                0.0f,               // py
                0.0f);              // pz
        m_currentPos[1] = glm::vec3(
                -3.0f,              // px
                0.0f,               // py
                0.0f);              // pz

        glm::vec3 dir = glm::vec3(
            -3.0f,                        // Dx
            0.0f,                        // Dy
            0.0f);                       // Dz
        m_directionVectors[0] = dir;
        dir = glm::vec3(
            3.0f,                        // Dx
            0.0f,                        // Dy
            0.0f);                       // Dz
        m_directionVectors[1] = dir;
        */

        m_posVBO = std::make_unique<VertexBuffer>(&m_currentPos[0], m_nCubes * sizeof(glm::vec3));
        m_VAO->AddInstancedAtt3fvec(4); // instance model matrix (4)

        m_EBO = std::make_unique<IndexBuffer>(indicesInstance, indexInstanceCount);

        // Main Shaders stuff
        m_Shader = std::make_unique<Shader>("./res/shaders/InstancingPhong.shader");
        m_Shader->Bind();

        m_Shader->SetUniform3f("u_lightColor", 1.0f, 1.0f, 1.0f);
        m_Shader->SetUniform3f("u_lightPos", 1.2f, 1.0f, 2.0f);

        // Light Shader stuff
        m_ShaderLight = std::make_unique<Shader>("./res/shaders/LightGoing3D.shader");
        m_ShaderLight->Bind();

        // Axis Shader stuff
        m_ShaderAxis = std::make_unique<Shader>("./res/shaders/OnlyColor.shader");
        m_ShaderAxis->Bind();
    }

    TestCollisionsInstancing::~TestCollisionsInstancing()
    {
        m_VAO->Unbind();
        m_Shader->Unbind();
        m_ShaderLight->Unbind();
        m_Texture.Unbind();
    }

    void TestCollisionsInstancing::ChangeShader()
    {
        if (!m_textureShader)
        {
            // Shaders stuff
            m_Shader = std::make_unique<Shader>("./res/shaders/InstancingPhong.shader");
            m_Shader->Bind();

            m_Shader->SetUniform3f("u_lightColor", 1.0f, 1.0f, 1.0f);
            m_Shader->SetUniform3f("u_lightPos", 1.2f, 1.0f, 2.0f);
        }
        else
        {
            // Shaders stuff
            m_Shader = std::make_unique<Shader>("./res/shaders/InstancingMaterial.shader");
            Shader shader();
            m_Shader->Bind();

            // setting up some uniforms
            m_Shader->SetUniform3f("light.position", 1.2f, 1.0f, 2.0f);
            m_Shader->SetUniform1i("material.diffuse", 2); // 2 is the slot
            m_Shader->SetUniform1i("material.specular", 3); // 3 is the slot
        }
        m_lastShader = m_textureShader;
    }

    void TestCollisionsInstancing::OnUpdate(float deltaTime)
    {
        m_cam.UpdateCam(deltaTime);
        if (m_play)
        {
            CollideDetection(deltaTime);
        }
    }

    void TestCollisionsInstancing::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        BatchRenderer renderer;

        if (m_lastShader != m_textureShader)
            ChangeShader();

        m_View = glm::lookAt(m_cam.m_camPos, m_cam.m_camDirection, m_cam.m_camUp);

        // Draw Axis
        m_bVertexBuffer->Bind(m_Axis.m_Vertices);
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(5.0f));

            glm::mat4 mvp = m_Proj * m_View * model;
            m_ShaderAxis->Bind();
            m_ShaderAxis->SetUniformMat4f("u_MVP", mvp);

            m_bVAO->Bind();

            renderer.DrawAxis();
        }

        // Draw Cube instances
        {
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_model", glm::mat4(1.0f));
            m_Shader->SetUniformMat4f("u_projection", m_Proj);
            m_Shader->SetUniformMat4f("u_view", m_View);
            m_Shader->SetUniform3f("u_viewPos", m_cam.m_camPos.x, m_cam.m_camPos.y, m_cam.m_camPos.z);
            if (!m_textureShader)
            {
                m_Shader->SetUniform1f("u_ambientStrenght", m_ambientStrenght);
                m_Shader->SetUniform1f("u_diffuseStrenght", m_diffuseStrenght);
                m_Shader->SetUniform1f("u_specularStrenght", m_specularStrenght);
                m_Shader->SetUniform1f("u_shininess", pow(2, m_shininessLevel));
                m_Shader->SetUniform1f("u_delta", m_delta);
                m_Shader->SetUniform1i("u_nCubes", m_nCubes);
                for (unsigned int i = 0; i < m_nCubes; i++)
                {
                    m_Shader->SetUniform3f("u_currentPoss[" + std::to_string(i) + "]", 
                        m_currentPos[i].x, m_currentPos[i].y, m_currentPos[i].z);
                }
            }
            else
            {
                m_Shader->SetUniform3f("light.ambient", m_ambientStrenght, m_ambientStrenght, m_ambientStrenght);
                m_Shader->SetUniform3f("light.diffuse", m_diffuseStrenght, m_diffuseStrenght, m_diffuseStrenght);
                m_Shader->SetUniform3f("light.specular", m_specularStrenght, m_specularStrenght, m_specularStrenght);
                m_Shader->SetUniform1f("material.shininess", pow(2, m_shininessLevel));
                m_diffuseMap.Bind(2);
                m_specularMap.Bind(3);
            }

            m_VAO->Bind();

            GLCall(glDrawElementsInstanced(GL_TRIANGLES, indexInstanceCount, GL_UNSIGNED_INT, nullptr, m_nCubes));
        }

        m_bVertexBuffer->Bind(m_RBody1.GetFigure().m_Vertices);
        // Draw Light Cube
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(1.2f, 2.0f, 2.0f));
            model = glm::scale(model, glm::vec3(0.2f));

            glm::mat4 mvp = m_Proj * m_View * model;
            m_ShaderLight->Bind();
            m_ShaderLight->SetUniformMat4f("u_MVP", mvp);

            m_bVAO->Bind();

            renderer.Draw(*m_bVAO, *m_bIndexBuffer, *m_ShaderLight);
        }

        if (m_play)
        {
            m_delta += m_step / 100.0f;
        }
    }

    void TestCollisionsInstancing::CollideDetection(float delta)
    {
        bool collided = false;
        glm::vec3* currentPos = new glm::vec3[m_nCubes];
        glm::vec3* directionVectors = new glm::vec3[m_nCubes];
        for (unsigned int i = 0; i < m_nCubes; i++)
        {
            directionVectors[i] = m_directionVectors[i];
            currentPos[i] = m_currentPos[i] + m_directionVectors[i] * delta;
            for (unsigned int j = 0; j < m_nCubes; j++)
            {
                if (i != j)
                {
                    if (glm::distance(currentPos[i], m_currentPos[j] + m_directionVectors[j] * delta) <= 1)
                    {
                        directionVectors[i] = -m_directionVectors[i];
                        break;
                    }
                }
            }
        }
        m_directionVectors = directionVectors;
        m_currentPos = currentPos;
        m_posVBO = std::make_unique<VertexBuffer>(&currentPos[0], m_nCubes * sizeof(glm::vec3));
        m_VAO->AddInstancedAtt3fvec(4); // instance model matrix (4, 5, 6, 7)
    }

    void TestCollisionsInstancing::OnImGuiRender()
    {
        if (ImGui::TreeNode("SCENE PARAMETERS:"))
        {
            ImGui::Checkbox("Play/Pause", &m_play);
            ImGui::SliderInt("Cube amount: ", &m_nCubes, 0, 1000000);
            ImGui::SliderInt("Limit: ", &m_limit, 1, 100);
            ImGui::SliderFloat("Velocity", &m_step, 0.0f, 10.0f);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("CAM PARAMETERS:"))
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

        if (ImGui::TreeNode("TEXTURE PARAMETERS:"))
        {
            ImGui::Checkbox("Texture Shader", &m_textureShader);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("LIGHT PARAMETERS:"))
        {
            ImGui::SliderFloat("Ambient", &m_ambientStrenght, 0.0f, 1.0f);
            ImGui::SliderFloat("Diffuse", &m_diffuseStrenght, 0.0f, 1.0f);
            ImGui::SliderFloat("Specular", &m_specularStrenght, 0.0f, 1.0f);
            ImGui::SliderInt("Specular shineness level", &m_shininessLevel, 0, 8);
            ImGui::TreePop();
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    void TestCollisionsInstancing::OnPickingObjects(int width, int height)
    {
    }
}