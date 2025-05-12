#pragma once
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <map>

class InputManager {
public:
	static InputManager& get() {
		static InputManager manager;
		return manager;
	}

	void Start(GLFWwindow* window);
	void Update();
	bool GetKeyDown(int key);
	bool GetKeyPressed(int key);
	void UpdateMouse(double xpos, double ypos) {
		if (!mouseLocked) return;

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);
	}
	glm::vec3 GetCameraFront() {
		return cameraFront;
	}
	void ToggleMouse() {
		if(!mouseLocked) {
			mouseLocked = true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		} else {
			mouseLocked = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
private:
	std::map<int, bool> keysDown;
	std::map<int, bool> keysPressed;
	std::map<int, bool> keysDownLastFrame;

	bool mouseLocked = true;

	bool firstMouse = true;
	float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float pitch = 0.0f;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;
	float fov = 10.0f;
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

	GLFWwindow* window;
	InputManager() = default;
	~InputManager() = default;
};