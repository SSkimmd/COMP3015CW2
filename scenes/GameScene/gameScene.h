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
    ParticleEmitter fireEmitter = ParticleEmitter();
    GLuint particleTexture;
    float particleLifeTime = 5.5f;
    Grid grid;
    Random rand;
    GLuint nParticles = 5000, initVel, startTime, particles;
    glm::vec3 emitterPos = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 emitterDir = glm::vec3(-1.0f, 2.0f, 0.0f);


    float deltaTime, lastFrame;

    std::map<std::string, GLSLProgram*> shaders;
    float angle = 0.0f;
    glm::vec3 playerPosition = glm::vec3(0.0f, 0.0, 3.0f);
    glm::mat4 viewport;

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec4 lightPos = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

    struct WireframeData {
        glm::vec4 LineColour = glm::vec4(0.0f, 0.0f, 0.05f, 1.0f);
        float LineWidth = 0.1f;
    };

    WireframeData wireframeData;



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
    void InitBuffers();

    std::vector<Object*> objects;
    std::vector<GrassObject*> grass;
    std::vector<ParticleEmitter*> emitters;
    std::string currentShader;
};

#endif // SCENEBASIC_UNIFORM_H
