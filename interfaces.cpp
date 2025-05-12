#include "interfaces.hpp"

void InterfaceManager::Init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    io = &ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
    std::cout << "init imgui" << std::endl;
}

void InterfaceManager::Update() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    ImGui::Begin("Debug");

    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}