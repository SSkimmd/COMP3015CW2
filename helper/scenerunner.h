#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "scene.h"
#include "../input.hpp"
#include "../interface.hpp"
#include "glutils.h"

#include <string>
#include <iostream>

#define WIN_WIDTH 1024
#define WIN_HEIGHT 768


class SceneRunner {
private:
    GLFWwindow* window;
    int width = WIN_WIDTH, height = WIN_HEIGHT;
    bool mouseLocked = true;
    bool debug;
    int fbh, fbw;
public:
    SceneRunner(const std::string & windowTitle, int width = WIN_WIDTH, int height = WIN_HEIGHT, int samples = 4) {
        // Initialize GLFW
    // glfw: initialize and configure
    // ------------------------------
        // Initialize GLFW
        if (!glfwInit()) exit(EXIT_FAILURE);

#ifdef __APPLE__
        // Select OpenGL 4.1
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#else
        // Select OpenGL 4.6
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#endif
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        if (debug)
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        if (samples > 0) {
            glfwWindowHint(GLFW_SAMPLES, samples);
        }

        // Open the window
        window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, windowTitle.c_str(), NULL, NULL);
        if (!window) {
            std::cerr << "Unable to create OpenGL context." << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(window);

        // Get framebuffer size
        glfwGetFramebufferSize(window, &fbw, &fbh);
        glfwSetCursorPosCallback(window, OnMouseMove);

        // Load the OpenGL functions.
        if (!gladLoadGL()) { exit(-1); }

        GLUtils::dumpGLInfo();

        // Initialization
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glEnable(GL_MULTISAMPLE);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


#ifndef __APPLE__
        if (debug) {
            glDebugMessageCallback(GLUtils::debugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
            glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0,
                GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Start debugging");
        }
#endif
    }

    int run(Scene& scene) {
        scene.setDimensions(fbw, fbh);
        scene.initScene();
        scene.resize(fbw, fbh);


        // Enter the main loop
        mainLoop(window, scene);

#ifndef __APPLE__
        if (debug)
            glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 1,
                GL_DEBUG_SEVERITY_NOTIFICATION, -1, "End debug");
#endif

        // Close window and terminate GLFW
        glfwTerminate();

        // Exit program
        return EXIT_SUCCESS;
    }

    static std::string parseCLArgs(int argc, char** argv, std::map<std::string, std::string>& sceneData) {
        if (argc < 2) {
            printHelpInfo(argv[0], sceneData);
            exit(EXIT_FAILURE);
        }

        std::string recipeName = argv[1];
        auto it = sceneData.find(recipeName);
        if (it == sceneData.end()) {
            printf("Unknown recipe: %s\n\n", recipeName.c_str());
            printHelpInfo(argv[0], sceneData);
            exit(EXIT_FAILURE);
        }

        return recipeName;
    }

private:
    static void printHelpInfo(const char* exeFile, std::map<std::string, std::string>& sceneData) {
        printf("Usage: %s recipe-name\n\n", exeFile);
        printf("Recipe names: \n");
        for (auto it : sceneData) {
            printf("  %11s : %s\n", it.first.c_str(), it.second.c_str());
        }
    }
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
