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

		vector<future<void>> m_Futures;
		ParticleSystem(Transformable* targetTransform, unsigned int particleCount, float radius, float _speed, Time lifeTime, Color color = Color::White) :
			m_TargetTransform(targetTransform),
			m_particles(particleCount),
			m_Speed(_speed),
			m_Radius(radius),
			m_LifeTime(lifeTime)
		{
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

		static void InitializeParticle(Particle* particle, Texture* texture, Vector2f pos, float radius, float _speed, Time lifeTime, Color* color);

		void SetEmitterTransform(Transformable& transform) override;
		void SetTexture(Texture* texture);
		Vector2f GetTargetTransform();

		void Update() override;

	protected:
		vector<Particle> m_particles;
		Transformable* m_TargetTransform;
		const Texture* m_Texture;
		float m_Speed, m_Radius;
		Time m_LifeTime;

	};
}