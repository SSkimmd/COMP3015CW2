#include "gameScene.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

using std::string;
using std::cerr;
using std::endl;
using glm::vec3;
using glm::mat4;


GameScene::GameScene() : angle(0.0f) {
    GameScene::objects.push_back(new Object("./media/plane_fixed.obj", "./media/plane.jpg"));
    GameScene::objects.push_back(new Object("./media/spot/spot_triangulated.obj", "./media/spot/spot_texture.png"));
    GameScene::objects.push_back(new Object("./media/snow_fixed.obj", "./media/grey.png"));
    skyboxTexture = Texture::loadCubeMap("./media/texture/cube/custom/custom");
    particleTexture = Texture::loadTexture("./media/texture/bluewater.png");
}

void GameScene::initScene() {
    std::cout << "compiling" << std::endl;
    compile();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    objects[0]->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));
    objects[1]->SetScale(glm::vec3(.225f, .225f, .225f));

    //plane
    objects[0]->SetPosition(glm::vec3(2.0f, -.72f, .4f));
    objects[0]->SetRotation(glm::vec3(0.0f, 45.0f, 0.0f));

    //cow
    objects[1]->SetPosition(glm::vec3(-1.375f, -.70f, 0.0f));
    objects[1]->SetRotation(glm::vec3(0.0f, 200.0f, 0.0f));

    //environment
    objects[2]->SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));

    view = glm::lookAt(
        glm::vec3(0.0f, 4.0f, 7.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    //projection = glm::perspective(glm::radians(50.0f), (float)width / (float)height, 0.1f, 100.0f);
    projection = mat4(1.0f);

    //pbr shader
    GLSLProgram* shader_basic = shaders["basic"];
    shader_basic->use();
    shader_basic->setUniform("Light.L", glm::vec3(1.0f));
    shader_basic->setUniform("Light.La", glm::vec3(0.05f));
    shader_basic->setUniform("Material.Kd", 0.7f, 0.7f, 0.7f);
    shader_basic->setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
    shader_basic->setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
    shader_basic->setUniform("Material.Shininess", 100.0f);
    shader_basic->setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    GLSLProgram* shader_pbr = shaders["pbr"];
    shader_pbr->use();
    shader_pbr->setUniform("Light[0].L", glm::vec3(45.0f));
    shader_pbr->setUniform("Light[0].Position", view * lightPos);
    shader_pbr->setUniform("Light[1].L", glm::vec3(0.3f));
    shader_pbr->setUniform("Light[1].Position", glm::vec4(0.0f, 0.15f, -1.0f, 0.0f));
    shader_pbr->setUniform("Light[2].L", glm::vec3(45.0f));
    shader_pbr->setUniform("Light[2].Position", view * glm::vec4(7.0f, 3.0f, 7.0f, 1.0f));

    GLSLProgram* shader_wireframe = shaders["wireframe"];
    shader_wireframe->use();
    shader_wireframe->setUniform("Line.Width", 0.1f);
    shader_wireframe->setUniform("Line.Color", glm::vec4(0.0f, 0.0f, 0.05f, 1.0f));
    shader_wireframe->setUniform("Light.L", glm::vec3(1.0f));
    shader_wireframe->setUniform("Light.La", glm::vec3(0.05f));
    shader_wireframe->setUniform("Material.Kd", 0.7f, 0.7f, 0.7f);
    shader_wireframe->setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
    shader_wireframe->setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
    shader_wireframe->setUniform("Material.Shininess", 50.0f);
    shader_wireframe->setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));


    InitBuffers();
    shaders["particle"]->use();
    shaders["particle"]->setUniform("ParticleTex", 0);
    shaders["particle"]->setUniform("ParticleLifeTime", particleLifeTime);
    shaders["particle"]->setUniform("ParticleSize", 0.05f);
    shaders["particle"]->setUniform("Gravity", glm::vec3(0.0f, -0.2f, 0.0f));
    shaders["particle"]->setUniform("EmitterPos", emitterPos);

    currentShader = "basic";
}

void GameScene::renderGUI() {
    ImGui::Begin("test");
    if(ImGui::CollapsingHeader("Objects")) {
        ImGui::Indent();
        for (int i = 0; i < objects.size(); i++) {
            ImGui::Text("Object");
        }
        ImGui::Unindent();
    }
    if(ImGui::CollapsingHeader("Emitters")) {
        ImGui::Indent();
        for (int i = 0; i < emitters.size(); i++) {
            ImGui::Text("Emitter");
        }
        ImGui::Unindent();
    }
    if(ImGui::CollapsingHeader("Shaders")) {
        ImGui::Indent();
        for (auto const& x : shaders) {
            std::string name = x.first;

            if (name == "skybox" || name == "flat" || name == "particle") { continue; }
            if(ImGui::Button(name.c_str())) {
                currentShader = name;
                shaders[currentShader]->use();
            }

            std::string nameHeader = name + " settings";
            if(ImGui::CollapsingHeader(nameHeader.c_str())) {
                if (name == "wireframe") {
                    ImGui::InputFloat("Line Width", &wireframeData.LineWidth);
                    ImGui::InputFloat("Line Color R", &wireframeData.LineColour.x);
                    ImGui::InputFloat("Line Color G", &wireframeData.LineColour.y);
                    ImGui::InputFloat("Line Color B", &wireframeData.LineColour.z);
                    ImGui::InputFloat("Line Color A", &wireframeData.LineColour.a);
                }
            }
        }
        ImGui::Unindent();
    }
    ImGui::End();
}

void GameScene::compile() {
	try {
        
        GLSLProgram* shader_basic = new GLSLProgram();
        shader_basic->compileShader("shader/basic.vert");
        shader_basic->compileShader("shader/basic.frag");
        //shader_basic->compileShader("shader/basic.gs");
        shader_basic->link();

        GLSLProgram* shader_pbr = new GLSLProgram();
        shader_pbr->compileShader("shader/pbr.vert");
        shader_pbr->compileShader("shader/pbr.frag");
        //shader_basic->compileShader("shader/basic.gs");
        shader_pbr->link();

        
        GLSLProgram* shader_wireframe = new GLSLProgram();
        shader_wireframe->compileShader("shader/wireframe.vert");
        shader_wireframe->compileShader("shader/wireframe.frag");
        shader_wireframe->compileShader("shader/wireframe.gs");
        shader_wireframe->link();

        GLSLProgram* shader_skybox = new GLSLProgram();
        shader_skybox->compileShader("shader/skybox.vert");
        shader_skybox->compileShader("shader/skybox.frag");
        shader_skybox->link();

        GLSLProgram* shader_particle = new GLSLProgram();
        shader_particle->compileShader("shader/particles/particles.vert");
        shader_particle->compileShader("shader/particles/particles.frag");
        shader_particle->link();

        GLSLProgram* shader_flat = new GLSLProgram();
        shader_flat->compileShader("shader/particles/solid.vert");
        shader_flat->compileShader("shader/particles/solid.frag");
        shader_flat->link();

        shaders["basic"] = shader_basic;
        shaders["pbr"] = shader_pbr;
        shaders["wireframe"] = shader_wireframe;
        shaders["skybox"] = shader_skybox;
        shaders["flat"] = shader_flat;
        shaders["particle"] = shader_particle;
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void GameScene::update(float t) {
    float currentFrame = static_cast<float>(t);
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    if (InputManager::get().GetKeyDown(GLFW_KEY_W))
        cameraPos += cameraSpeed * cameraFront;
    if (InputManager::get().GetKeyDown(GLFW_KEY_S))
        cameraPos -= cameraSpeed * cameraFront;
    if (InputManager::get().GetKeyDown(GLFW_KEY_A))
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (InputManager::get().GetKeyDown(GLFW_KEY_D))
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;


    if(InputManager::get().GetKeyPressed(GLFW_KEY_ESCAPE)) {
        InputManager::get().ToggleMouse();
    }
}

void GameScene::SetMatrices() {
    if (currentShader == "pbr") {
        shaders[currentShader]->setUniform("Light[0].Position", view * lightPos);
        shaders[currentShader]->setUniform("Material.Roughness", 0.43f);
        shaders[currentShader]->setUniform("Material.Metal", 1);
        shaders[currentShader]->setUniform("Material.Color", glm::vec3(1.0f, 0.71f, 0.29f));

        glm::mat4 mv = view * model;
        shaders[currentShader]->setUniform("ModelViewMatrix", mv);
        shaders[currentShader]->setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        shaders[currentShader]->setUniform("MVP", projection * mv);
    }
    if(currentShader == "basic") {
        glm::mat4 mv = view * model;
        shaders[currentShader]->setUniform("ModelViewMatrix", mv);
        shaders[currentShader]->setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        shaders[currentShader]->setUniform("MVP", projection * mv);
    }
    if (currentShader == "wireframe") {
        glm::mat4 mv = view * model;
        shaders[currentShader]->setUniform("Line.Width", wireframeData.LineWidth);
        shaders[currentShader]->setUniform("Line.Color", glm::vec4(
            wireframeData.LineColour.x, 
            wireframeData.LineColour.y, 
            wireframeData.LineColour.z, 
            wireframeData.LineColour.a)
        );
        shaders[currentShader]->setUniform("Light.L", glm::vec3(1.0f));
        shaders[currentShader]->setUniform("Light.La", glm::vec3(0.05f));
        shaders[currentShader]->setUniform("Material.Kd", 0.7f, 0.7f, 0.7f);
        shaders[currentShader]->setUniform("Material.Ka", 0.2f, 0.2f, 0.2f);
        shaders[currentShader]->setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
        shaders[currentShader]->setUniform("Material.Shininess", 50.0f);
        shaders[currentShader]->setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        shaders[currentShader]->setUniform("ModelViewMatrix", mv);
        shaders[currentShader]->setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
        shaders[currentShader]->setUniform("MVP", projection * mv);
        shaders[currentShader]->setUniform("ViewportMatrix", viewport);
    }
}

void GameScene::render() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cameraFront = InputManager::get().GetCameraFront();
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    projection = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 100.0f);

    
    for (int i = 0; i < objects.size(); i++) {
        shaders[currentShader]->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, objects[i]->texture);

        model = mat4(1.0f);
        model = glm::translate(model, objects[i]->GetPosition());
        model = glm::rotate(model, glm::radians(objects[i]->GetRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(objects[i]->GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(objects[i]->GetRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, objects[i]->GetScale());

        SetMatrices();
        objects[i]->model->render();
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    shaders["skybox"]->use();
    model = mat4(1.0f);
    glm::mat4 mv = view * model;
    shaders["skybox"]->setUniform("ModelViewMatrix", mv);
    shaders["skybox"]->setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    shaders["skybox"]->setUniform("MVP", projection * mv);
    skybox.render();

    /*
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    shaders["particle"]->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE, particleTexture);

    model = mat4(1.0f);
    mv = view * model;
    shaders["particle"]->setUniform("ModelViewMatrix", mv);
    shaders["particle"]->setUniform("MVP", projection * mv);
    shaders["particle"]->setUniform("ProjectionMatrix", projection);
    shaders["particle"]->setUniform("Time", time);
    glBindVertexArray(particles);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticles);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);*/
}


void GameScene::InitBuffers() {
    /*
    glGenBuffers(1, &initVel);
    glGenBuffers(1, &startTime);
    int size = nParticles * sizeof(float);
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glBufferData(GL_ARRAY_BUFFER, size * 3, 0, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);

    glm::mat3 emitterBasis = ParticleUtils::makeArbitraryBasis(emitterDir);
    glm::vec3 v(0.0f);

    float velocity, theta, phi;
    std::vector<GLfloat> data(nParticles * 3);

    for (uint32_t i = 0; i < nParticles; i++) {
        theta = glm::mix(0.0f, glm::pi<float>() / 20.0f, rand.nextFloat());
        phi = glm::mix(0.0f, glm::two_pi<float>(), rand.nextFloat());
        v.x = sinf(theta) * cosf(phi);
        v.y = sinf(theta);
        v.z = sinf(theta) * sinf(phi);
        velocity = glm::mix(1.25f, 1.5f, rand.nextFloat());
        v = glm::normalize(emitterBasis * v) * velocity;
        data[3 * i] = v.x;
        data[3 * i + 1] = v.y;
        data[3 * i + 2] = v.z;
    }

    glBindBuffer(GL_BUFFER, initVel);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size * 3, data.data());
    float rate = particleLifeTime / nParticles;

    for (int i = 0; i < nParticles; i++) {
        data[i] = rate * i;
    }

    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glGenVertexArrays(1, &particles);
    glBindVertexArray(particles);

    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 0);
    glBindVertexArray(0);
    */
}





void GameScene::resize(int w, int h) {
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(50.0f), (float)width / (float)height, 0.3f, 100.0f);
    viewport = mat4(
        glm::vec4(w / 2.0f, 0.0f, 0.0f, 0.0f),
        glm::vec4(0.0f, h / 2.0f, 0.0f, 0.0f),
        glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
        glm::vec4(w / 2.0f + 0, h / 2.0f + 0, 0.0f, 0.0f)
    );
}
