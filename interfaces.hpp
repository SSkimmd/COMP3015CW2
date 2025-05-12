#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include <iostream>

class InterfaceManager {
public:
	static InterfaceManager& get() {
		static InterfaceManager manager;
		return manager;
	}

	InterfaceManager() = default;
	~InterfaceManager() = default;
	void Update();
	void Init(GLFWwindow* window);
private:
	ImGuiIO* io;
};