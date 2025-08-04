#pragma once

#include <memory>
#include <vector>

#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "scene/Light.h"
#include "scene/figures/RigidBody.h"

class Scene {
public:
	Scene() {};
	~Scene() {};

	void SetAxis();
	void SetFloor();
	void SetFigures();
	void SetBodies();
	void SetLights();
	void SetInstancingAttributes();
	void CreateScene();

	void PrepareAxis(Shader & axisShader, glm::mat4 proj, glm::mat4 view);
	void PrepareLights(Shader & lightShader, glm::mat4 proj, glm::mat4 view);
	void PrepareInstancedPhongFigures(Shader& figureShader, glm::mat4 proj, glm::mat4 view, Camera cam,
		bool textureOn, Texture diffuseMap, Texture specularMap);
	void PreparePhongFigures(Shader& figureShader, glm::mat4 proj, glm::mat4 view, Camera cam, 
		bool textureOn, int bodyIndex);

	void CollideDetection(float delta);

	std::vector<RigidBody> m_bodies;
	std::vector<Figure> m_figures;
	std::vector<Light> m_lights;
	// Instancing stuff
	int m_nCubes;
	int m_limit;
	glm::vec3* m_currentPos;
	glm::vec3* m_directionVectors;
};