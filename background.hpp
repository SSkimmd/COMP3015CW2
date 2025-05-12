#pragma once
#include <string>
#include <glad/glad.h>
#include "glm/glm.hpp"
#include "texture.h"

class Background {
public:
	void Create(std::string file);
	void Render();
	//GLuint texture = Texture::loadCubeMap("media/texture/cube/pisa/pisa");
private:
};