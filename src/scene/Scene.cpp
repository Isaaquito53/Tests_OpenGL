#include "scene/Scene.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Include all the figures:
#include "scene/figures/Axis.h"
#include "scene/figures/Cube.h"
#include "scene/figures/Floor.h"

void Scene::SetAxis()
{
	Axis axis;
	m_figures.push_back(axis);
}

void Scene::SetFloor()
{
	Floor floor;
	m_figures.push_back(floor);
}

void Scene::SetFigures()
{
	Cube cube;
	m_figures.push_back(cube);
}

void Scene::SetBodies()
{
	RigidBody rb(&m_figures[2], -5.0f, 0.5f, -5.0f, 0.0f);	// Instanced renderin cubes
	m_bodies.push_back(rb);
	RigidBody rb_central(&m_figures[2], -5.0f, 0.5f, -5.0f, 0.0f);	// Central cube
	rb_central.SetR(1.5f);
	rb_central.SetScale(3.0f);
	m_bodies.push_back(rb_central);
}

void Scene::SetLights()
{
	Light light = {
		0.1f,						// ambientStrenght
		1.0f,						// diffuseStrenght
		0.5f,						// specularStrenght
		5,							//shininessLevel;
		Vec3 {1.0f, 1.0f, 1.0f},	//color;
		Vec3 {1.2f, 1.0f, 2.0f},	//position;
		&m_figures[2]				//fig;
	};
	m_lights.push_back(light);
}

void Scene::SetInstancingAttributes()
{
	m_nCubes = 60;
	m_limit = 10;

	// Generate instancing attributes
	unsigned int circles = 10;
	m_currentPos = new glm::vec3[m_nCubes];
	m_directionVectors = new glm::vec3[m_nCubes];
	srand(0);
	for (unsigned int k = 1; k <= circles; k++)
	{
		for (unsigned int i = 0; i < m_nCubes / circles; i++)
		{
			m_currentPos[i + (k - 1) * m_nCubes / circles] = glm::vec3(
				k * m_limit * cos(glm::radians(i * 360.0f / (m_nCubes / circles))),		// px
				0.0f,																	// py
				k * m_limit * sin(glm::radians(i * 360.0f / (m_nCubes / circles))));	// pz

			glm::vec3 dir = glm::vec3(
				-m_currentPos[i + (k - 1) * m_nCubes / circles].x / k,      // vx
				0.0f,														// vy
				-m_currentPos[i + (k - 1) * m_nCubes / circles].z / k);     // vz
			m_directionVectors[i + (k - 1) * m_nCubes / circles] = dir;
		}
	}
}

void Scene::CreateScene()
{
	SetAxis();
	SetFloor();
	SetFigures();

	SetBodies();

	SetLights();

	SetInstancingAttributes();
}

void Scene::PrepareAxis(Shader& axisShader, glm::mat4 proj, glm::mat4 view)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(5.0f));

	glm::mat4 mvp = proj * view * model;
	axisShader.Bind();
	axisShader.SetUniformMat4f("u_MVP", mvp);
}

void Scene::PrepareLights(Shader& lightShader, glm::mat4 proj, glm::mat4 view)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, 
		glm::vec3(m_lights[0].m_position.x, m_lights[0].m_position.y, m_lights[0].m_position.z));
	model = glm::scale(model, glm::vec3(0.2f));

	glm::mat4 mvp = proj * view * model;
	lightShader.Bind();
	lightShader.SetUniformMat4f("u_MVP", mvp);
}

void Scene::PrepareInstancedPhongFigures(Shader& figureShader, glm::mat4 proj, glm::mat4 view, Camera cam,
	bool textureOn, Texture diffuseMap, Texture specularMap)
{
	figureShader.Bind();
	figureShader.SetUniformMat4f("u_model", glm::mat4(1.0f));
	figureShader.SetUniformMat4f("u_projection", proj);
	figureShader.SetUniformMat4f("u_view", view);
	figureShader.SetUniform3f("u_viewPos", cam.m_camPos.x, cam.m_camPos.y, cam.m_camPos.z);
	if (!textureOn)
	{
		figureShader.SetUniform1f("u_ambientStrenght", m_lights[0].m_ambientStrenght);
		figureShader.SetUniform1f("u_diffuseStrenght", m_lights[0].m_diffuseStrenght);
		figureShader.SetUniform1f("u_specularStrenght", m_lights[0].m_specularStrenght);
		figureShader.SetUniform1f("u_shininess", pow(2, m_lights[0].m_shininessLevel));
		figureShader.SetUniform1i("u_nCubes", m_nCubes);
		for (unsigned int i = 0; i < m_nCubes; i++)
		{
			figureShader.SetUniform3f("u_currentPoss[" + std::to_string(i) + "]",
				m_currentPos[i].x, m_currentPos[i].y, m_currentPos[i].z);
		}
	}
	else
	{
		figureShader.SetUniform3f("light.ambient", m_lights[0].m_ambientStrenght, m_lights[0].m_ambientStrenght, m_lights[0].m_ambientStrenght);
		figureShader.SetUniform3f("light.diffuse", m_lights[0].m_diffuseStrenght, m_lights[0].m_diffuseStrenght, m_lights[0].m_diffuseStrenght);
		figureShader.SetUniform3f("light.specular", m_lights[0].m_specularStrenght, m_lights[0].m_specularStrenght, m_lights[0].m_specularStrenght);
		figureShader.SetUniform1f("material.shininess", pow(2, m_lights[0].m_shininessLevel));
		diffuseMap.Bind(2);
		specularMap.Bind(3);
	}
}

void Scene::PreparePhongFigures(Shader& figureShader, glm::mat4 proj, glm::mat4 view, Camera cam, bool textureOn)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(m_bodies[1].GetScale()));

	glm::mat4 mvp = proj * view * model;
	figureShader.Bind();
	figureShader.SetUniformMat4f("u_MVP", mvp);
	figureShader.SetUniformMat4f("u_Model", model);
	figureShader.SetUniform3f("u_viewPos", cam.m_camPos.x, cam.m_camPos.y, cam.m_camPos.z);
	figureShader.SetUniform1f("u_ambientStrenght", m_lights[0].m_ambientStrenght);
	figureShader.SetUniform1f("u_diffuseStrenght", m_lights[0].m_diffuseStrenght);
	figureShader.SetUniform1f("u_specularStrenght", m_lights[0].m_specularStrenght);
	figureShader.SetUniform1f("u_shininess", pow(2, m_lights[0].m_shininessLevel));
}

void Scene::CollideDetection(float delta)
{
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
				if (glm::distance(currentPos[i], m_currentPos[j] + m_directionVectors[j] * delta) <= 2*m_bodies[0].GetR())
				{
					directionVectors[i] = -m_directionVectors[i];
					break;
				}
			}
		}

		if (glm::distance(currentPos[i], glm::vec3(0.0f)) <= m_bodies[0].GetR() + m_bodies[1].GetR())
			directionVectors[i] = -m_directionVectors[i];
	}
	m_directionVectors = directionVectors;
	m_currentPos = currentPos;
}