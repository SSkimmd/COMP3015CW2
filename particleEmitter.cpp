#include "particleEmitter.hpp"

void ParticleEmitter::Init() {
	texture = Texture::loadTexture("./media/texture/bluewater.png");
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