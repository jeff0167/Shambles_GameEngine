#include "pch.h"
#include "ParticleSystem.h"

//static mutex m_Mutex; // don't need to lock with mutex here

void Shambles::ParticleSystem::InitializeParticle(Particle* particle, Texture* texture, Vector2f pos, float radius, float _speed, Time lifeTime, Color* color)
{
	particle->dot.setTexture(texture);
	float size = (rand() % 3) * radius;
	particle->dot.setSize(Vector2f(size, size));
	particle->dot.setOrigin(size / 2, size / 2);
	float angle = (rand() % 360) * 3.14f / 180.f;
	double speed = (rand() % 50) * 0.01 * _speed + 0.05;
	particle->SetVelocity(cos(angle) * speed, sin(angle) * speed);
	particle->SetMaxLifeTime(milliseconds((rand() % 100) * 2.0f + lifeTime.asMilliseconds()));
	particle->lifespan = particle->maxLifespan;
	particle->SetPosition(pos);
	particle->m_Color = color;
	particle->dot.setFillColor(*particle->m_Color);
	//lock_guard<mutex> lock(m_Mutex); // Get lower performance if used and isn't needed to make it work it seems
}

void Shambles::ParticleSystem::SetEmitterTransform(Transformable& transform)
{
	m_TargetTransform = &transform;
}

void Shambles::ParticleSystem::SetTexture(Texture* texture)
{
	m_Texture = texture;
	for (auto& particle : m_particles)
	{
		particle.dot.setTexture(m_Texture);
	}
}

Vector2f Shambles::ParticleSystem::GetTargetTransform()
{
	if (m_TargetTransform != nullptr) return m_TargetTransform->getPosition();
	return Vector2f(0, 0);
}

void Shambles::ParticleSystem::Update()
{
	for (auto& particle : m_particles)
	{
		//async(launch::async, [&particle](){return particle.Update();}); // using async without static func, though not suited for this specifi case
		particle.Update();
	}
}
