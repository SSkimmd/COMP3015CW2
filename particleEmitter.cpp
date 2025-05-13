#include "particleEmitter.hpp"

void ParticleEmitter::Init() {
	texture = Texture::loadTexture("./media/texture/bluewater.png");

	shader->use();
	shader->setUniform("ParticleTex", 0);
	shader->setUniform("ParticleLifeTime", particleLifeTime);
	shader->setUniform("ParticleSize", 0.05f);
	shader->setUniform("Gravity", glm::vec3(0.0f, -0.2f, 0.0f));
	shader->setUniform("EmitterPos", emitterPos);

	flat->use();
	flat->setUniform("Color", glm::vec4(0.4f, 0.4f, 0.4f, 0.4f));
}


void ParticleEmitter::Render() {
}