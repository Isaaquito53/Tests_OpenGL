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
	m_nCubes = 8;
	m_limit = 0;

	m_currentPos = new glm::vec3[m_nCubes];
	m_directionVectors = new glm::vec3[m_nCubes];

	// First collision (same v, different mass)
	RigidBody rb_Small(&m_figures[2], -5.0f, 0.5f, -5.0f, 0.0f);	
	rb_Small.SetR(1.5f);
	rb_Small.SetScale(3.0f);
	rb_Small.SetMass(3.0f);
	m_bodies.push_back(rb_Small);
	m_currentPos[0] = glm::vec3(-3.0f, 6.0f, 0.0f);
	m_directionVectors[0] = glm::vec3(1.0f, 0.0f, 0.0f);
	RigidBody rb_Bib(&m_figures[2], -5.0f, 0.5f, -5.0f, 0.0f);	
	rb_Bib.SetR(0.5f);
	rb_Bib.SetScale(1.0f);
	rb_Bib.SetMass(1.0f);
	m_bodies.push_back(rb_Bib);
	m_currentPos[1] = glm::vec3(3.0f, 6.0f, 0.0f);
	m_directionVectors[1] = glm::vec3(-1.0f, 0.0f, 0.0f);

	// Second collision (same mass, one static)
	RigidBody rb_2(&m_figures[2], -5.0f, 0.5f, -5.0f, 0.0f);	
	rb_2.SetR(0.5f);
	rb_2.SetScale(1.0f);
	rb_2.SetMass(1.0f);
	m_bodies.push_back(rb_2);
	m_currentPos[2] = glm::vec3(-3.0f, 3.0f, 0.0f);
	m_directionVectors[2] = glm::vec3(1.0f, 0.0f, 0.0f);
	RigidBody rb_3(&m_figures[2], -5.0f, 0.5f, -5.0f, 0.0f);	
	rb_3.SetR(0.5f);
	rb_3.SetScale(1.0f);
	rb_3.SetMass(1.0f);
	m_bodies.push_back(rb_3);
	m_currentPos[3] = glm::vec3(3.0f, 3.0f, 0.0f);
	m_directionVectors[3] = glm::vec3(0.0f, 0.0f, 0.0f);

	// Third collision (same mass, 1/2x vs 1x v)
	RigidBody rb_4(&m_figures[2], -5.0f, 0.5f, -5.0f, 0.0f);	
	rb_4.SetR(0.5f);
	rb_4.SetScale(1.0f);
	rb_4.SetMass(1.0f);
	m_bodies.push_back(rb_4);
	m_currentPos[4] = glm::vec3(-3.0f, 0.0f, 0.0f);
	m_directionVectors[4] = glm::vec3(1.0f, 0.0f, 0.0f);
	RigidBody rb_5(&m_figures[2], -5.0f, 0.5f, -5.0f, 0.0f);	
	rb_5.SetR(0.5f);
	rb_5.SetScale(1.0f);
	rb_5.SetMass(1.0f);
	m_bodies.push_back(rb_5);
	m_currentPos[5] = glm::vec3(3.0f, 0.0f, 0.0f);
	m_directionVectors[5] = glm::vec3(-0.5f, 0.0f, 0.0f);

	// Fourth collision (same mass, same direction, 1/2x vs 1x v)
	RigidBody rb_6(&m_figures[2], -5.0f, 0.5f, -5.0f, 0.0f);	
	rb_6.SetR(0.5f);
	rb_6.SetScale(1.0f);
	rb_6.SetMass(1.0f);
	m_bodies.push_back(rb_6);
	m_currentPos[6] = glm::vec3(-3.0f, -2.0f, 0.0f);
	m_directionVectors[6] = glm::vec3(1.0f, 0.0f, 0.0f);
	RigidBody rb_7(&m_figures[2], -5.0f, 0.5f, -5.0f, 0.0f);	
	rb_7.SetR(0.5f);
	rb_7.SetScale(1.0f);
	rb_7.SetMass(1.0f);
	m_bodies.push_back(rb_7);
	m_currentPos[7] = glm::vec3(0.0f, -2.0f, 0.0f);
	m_directionVectors[7] = glm::vec3(0.5f, 0.0f, 0.0f);
}

void Scene::SetLights()
{
	Light light = {
		0.1f,						// ambientStrenght
		1.0f,						// diffuseStrenght
		0.5f,						// specularStrenght
		5,							// shininessLevel;
		Vec3 {1.0f, 1.0f, 1.0f},	// color;
		Vec3 {1.2f, 1.0f, 2.0f},	// position;
		&m_figures[2]				// fig;
	};
	m_lights.push_back(light);
}

void Scene::SetInstancingAttributes()
{
	
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

void Scene::PreparePhongFigures(Shader& figureShader, glm::mat4 proj, glm::mat4 view, Camera cam, 
	bool textureOn, int bodyIndex)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, m_currentPos[bodyIndex]);
	model = glm::scale(model, glm::vec3(m_bodies[bodyIndex].GetScale()));

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
	float e = 0.9f;
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
				if (glm::distance(currentPos[i], m_currentPos[j] + m_directionVectors[j] * delta) <= m_bodies[i].GetR() + m_bodies[j].GetR())
				{
					glm::vec3 va1 =	m_directionVectors[i];
					glm::vec3 vb1 =	m_directionVectors[j];
					glm::vec3 vcom = ((m_bodies[i].GetMass() * va1) + (m_bodies[j].GetMass() * vb1)) / (m_bodies[i].GetMass() + m_bodies[j].GetMass());
					directionVectors[i] = (1+e)*vcom - e*va1;
					
					break;
				}
			}
		}
	}
	m_directionVectors = directionVectors;
	m_currentPos = currentPos;
}