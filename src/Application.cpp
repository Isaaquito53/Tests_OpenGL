#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestSnake.h"
#include "tests/TestGoing3D.h"
#include "tests/TestGravity.h"
#include "tests/TestCollisions.h"
#include "tests/TestCollisionsInstancing.h"

int main(void)
{
    GLFWwindow* window;
    float delta, lastFrame = 0.0f, currentFrame = 0.0f;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    
    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTexture2D>("2D Texture");
        testMenu->RegisterTest<test::TestSnake>("Snake");
        testMenu->RegisterTest<test::TestGoing3D>("Going 3D");
        testMenu->RegisterTest<test::TestGravity>("Gravity");
        testMenu->RegisterTest<test::TestCollisions>("Collisions");
        testMenu->RegisterTest<test::TestCollisionsInstancing>("Collisions Instancing");

        while (!glfwWindowShouldClose(window))
        {
            int width, height;
            glfwGetWindowSize(window, &width, &height);
            GLCall(glViewport(0, 0, width, height));

            GLCall(glClearColor(0.2f, 0.5f, 0.6f, 1.0f));
            renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();
            if (currentTest)
            {
                currentFrame = glfwGetTime();
                delta = currentFrame - lastFrame;
                lastFrame = currentFrame;
                currentTest->OnUpdate(delta);
                currentTest->OnRender();
                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();
                ImGui::End();
            }
            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            currentTest->OnPickingObjects(width, height);

            glfwPollEvents();
        }
        delete currentTest;
        if (currentTest != testMenu)
            delete testMenu;
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}