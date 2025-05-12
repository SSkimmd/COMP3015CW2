#pragma once
#include "./helper/glslprogram.h"

class ParticleEmitter {
public:
	GLSLProgram* shader = new GLSLProgram();
	void Init();
};