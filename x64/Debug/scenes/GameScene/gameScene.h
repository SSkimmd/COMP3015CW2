#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "../helper/glslprogram.h"
#include "../helper/scene.h"
#include "../input.hpp"
#include "../grass.hpp"
#include "../skybox.h"
#include "../particleEmitter.hpp"
#include "imgui/imgui.h"


class GameScene : public Scene
{
private:
    GLuint vaoHandle;
    SkyBox skybox = SkyBox();
    GLuint skyboxTexture;

    std::map<std::string, GLSLProgram*> shaders;
    float angle = 0.0f;
    glm::vec3 playerPosition = glm::vec3(0.0f, 0.0, 3.0f);
    glm::mat4 viewport;

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec4 lightPos = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

    float rotateSpeedScale = 1.0f;
    void compile();
public:
    GameScene();
    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
    void SetMatrices();
    void renderGUI();

    std::vector<Object*> objects;
    std::vector<GrassObject*> grass;
    std::string currentShader;
};

#endif // SCENEBASIC_UNIFORM_H
