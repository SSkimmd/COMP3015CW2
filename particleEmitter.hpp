#pragma once
#include "./helper/glslprogram.h"
#include "../particleutils.h"
#include "../grid.h"
#include "../texture.h"

class ParticleEmitter {
public:
	void Init();
	void Render();
	GLSLProgram* shader;
	GLSLProgram* flat;
	GLuint texture;
	float particleLifeTime = 5.5f;
	Grid grid;
	Random rand;
	GLuint nParticles = 5000, initVel, startTime, particles;
	glm::vec3 emitterPos = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 emitterDir = glm::vec3(-1.0f, 2.0f, 0.0f);
};