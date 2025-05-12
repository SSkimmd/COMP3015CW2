#pragma once
#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "../helper/glslprogram.h"
#include "../objmesh.h"
#include "../texture.h"

#include <iostream>

class Object {
public:
	Object(std::string modelFile = "", std::string textureFile = "") {
		model = ObjMesh::load(modelFile.c_str());

		if(!textureFile.empty()) {
			std::cout << "loaded texture: " + textureFile << std::endl;
			texture = Texture::loadTexture(textureFile.c_str());
			std::cout << texture << std::endl;
		}
	}

	std::unique_ptr<ObjMesh> model;
	GLuint texture;


	glm::vec3 GetScale() {
		return scale;
	}

	glm::vec3 SetScale(glm::vec3 newScale) {
		scale = newScale;
		return scale;
	}


	glm::vec3 GetPosition() {
		return position;
	}

	glm::vec3 SetPosition(glm::vec3 newPosition) {
		position = newPosition;
		return position;
	}

	glm::vec3 UpdatePosition(glm::vec3 newPosition) {
		position += newPosition;
		return position;
	}


	glm::vec3 GetRotation() {
		return rotation;
	}

	glm::vec3 UpdateRotation(glm::vec3 newPosition) {
		rotation += newPosition;
		return rotation;
	}

	glm::vec3 SetRotation(glm::vec3 newRotation) {
		rotation = newRotation;
		return rotation;
	}
private:
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};