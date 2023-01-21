#pragma once
#include <SFML/Graphics.hpp>
#include <future>
#include <mutex>
#include "Canvas.h"
#include "Monobehaviour.h"
#include "Component.h"
#include "ParticleSystemUpdate.h"
#include "Particle.h"
#include <vector>

using namespace sf;
using namespace std;

namespace Shambles
{
	class ParticleSystem : public Component, public ParticleSystemUpdate
	{
	public:
		ParticleSystem() {};

		//static mutex m_Mutex;

		static void InitializeParticle(Particle* particle, Texture* texture, Vector2f pos, float radius, float _speed, Time lifeTime, Color* color) // don't need to lock with mutex here
		{
			particle->dot.setTexture(texture);
			float size = (rand() % 3) * radius;
			particle->dot.setRadius(size);
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

		vector<future<void>> m_Futures;
		ParticleSystem(Transformable* targetTransform, unsigned int particleCount, float radius, float _speed, Time lifeTime, Color color = Color::White) :
			m_TargetTransform(targetTransform),
			m_particles(particleCount),
			m_Speed(_speed),
			m_Radius(radius),
			m_LifeTime(lifeTime)
		{
			//auto d = Mono.Timer();

			static Texture t;
			if (t.loadFromFile("ParticleDefault.png"))
			{
				m_Texture = &t;
			}

			for (auto& particle : m_particles)
			{
				Renderer.AddDrawable(particle.dot, 9);
				particle.SetParticleSystem(*this);
				m_Futures.push_back(async(launch::async, InitializeParticle, &particle, &t, m_TargetTransform->getPosition(), radius, _speed, lifeTime, &color));
			}
		}

		void SetEmitterTransform(Transformable& transform) override
		{
			m_TargetTransform = &transform;
		}
		Vector2f GetTargetTransform()
		{
			if (m_TargetTransform != nullptr) return m_TargetTransform->getPosition();
			return Vector2f(0, 0);
		}

		void Update() override
		{
			for (auto& particle : m_particles)
			{
				//async(launch::async, [&particle](){return particle.Update();}); // using async without static func, though not suited for this specifi case
				particle.Update();
			}
		}

	protected:
		vector<Particle> m_particles;
		Transformable* m_TargetTransform;
		const Texture* m_Texture;
		float m_Speed, m_Radius;
		Time m_LifeTime;

	};
}