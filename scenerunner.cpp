#include "../helper/scenerunner.h"


int SceneRunner::run(Scene& scene) {
    scene.setDimensions(width, height);
    scene.initScene();
    scene.resize(width, height);

    this->mainLoop(window, scene);
    glfwTerminate();

    // Exit program
    return EXIT_SUCCESS;
}

void SceneRunner::mainLoop(GLFWwindow* window, Scene& scene) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
    InputManager::get().Start(window);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        InputManager::get().Update();
        scene.update(glfwGetTime());
        scene.render();
        ImGui::NewFrame();
        scene.renderGUI();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
}
