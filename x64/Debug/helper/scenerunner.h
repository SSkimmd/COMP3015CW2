#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "scene.h"
#include "../input.hpp"
#include "../interface.hpp"

#include <string>
#include <iostream>

#define WIN_WIDTH 1024
#define WIN_HEIGHT 768


class SceneRunner {
private:
    GLFWwindow* window;
    int width = WIN_WIDTH, height = WIN_HEIGHT;
    bool mouseLocked = true;
public:
    SceneRunner(const std::string & windowTitle, int width = WIN_WIDTH, int height = WIN_HEIGHT, int samples = 4) {
        // Initialize GLFW
    // glfw: initialize and configure
    // ------------------------------
        glfwSetErrorCallback(glfw_error_callback);
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);


        // glfw window creation
        // --------------------
        window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, FrameBufferSizeChanged);
        glfwSetCursorPosCallback(window, OnMouseMove);
        glfwSetScrollCallback(window, OnScroll);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return;
        }

        glEnable(GL_MULTISAMPLE);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    int run(Scene& window);
    void mainLoop(GLFWwindow* window, Scene& scene);
    static void FrameBufferSizeChanged(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
    static void OnMouseMove(GLFWwindow* window, double xposIn, double yposIn) {
        InputManager::get().UpdateMouse(xposIn, yposIn);
    }

    static void OnScroll(GLFWwindow* window, double xoffset, double yoffset) {

    }
    static void glfw_error_callback(int error, const char* description) {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    }
};
