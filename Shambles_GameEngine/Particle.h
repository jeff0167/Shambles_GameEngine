#pragma once
#include "Pch.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

namespace Shambles
{
	class ParticleSystem;
	class Particle
	{
	public:
		CircleShape dot;
		Color* m_Color;

		Particle() {};
		~Particle();

		void SetVelocity(float x, float y);
		void SetPosition(float x, float y);
		void SetPosition(Vector2f pos);
		void SetMaxLifeTime(Time time);
		Vector2f GetTargetPos();
		void Update();
		void ApplyForce(Vector2f);
		Vector2f GetVelocity();
		void SetParticleSystem(ParticleSystem& ps);
		bool IsDead();
		Color SetColor(int, float, float, int);
		Time lifespan, maxLifespan;

	protected:
		Vector2f velocity;
		Vector2f acceleration;
		Vector2f position, startPos;
		ParticleSystem* m_ParticleSystem;

	};
}