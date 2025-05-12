#include "input.hpp"
#include <iostream>

void InputManager::Start(GLFWwindow* window) {
    this->window = window;

    for (int i = 32; i < 349; i++) {
        keysDown.emplace(i, false);
        keysPressed.emplace(i, false);
        keysDownLastFrame.emplace(i, false);
    }
}

void InputManager::Update() {
    for (int i = 32; i < 349; i++) {
        if (glfwGetKey(window, i) == GLFW_PRESS)
            keysDown[i] = true;
        else
            keysDown[i] = false;

        if (keysDown[i] && !keysDownLastFrame[i])
            keysPressed[i] = true;
        else
            keysPressed[i] = false;
        keysDownLastFrame[i] = keysDown[i];
    }
}

bool InputManager::GetKeyDown(int key) {
    return keysDown[key];
}

bool InputManager::GetKeyPressed(int key) {
    return keysPressed[key];
}