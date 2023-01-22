#pragma once
#include <SFML/Graphics.hpp>
#include "Rigidbody.h"
#include "Collider.h"
#include "ParticleSystem.h"

#define Science Physics::GetInstance()

using namespace sf;
using namespace std;

namespace Shambles
{
	class Physics
	{
	public:
		void Physcis(Physics& other) = delete;

		void operator=(const Physics&) = delete;

		static Physics& GetInstance();

		void InitializePhysicsUpdate();

		vector<Rigidbody*> rigidbodies;
		const vector<Rigidbody*>& GetRigidbodies();
		void AddRigidbody(Rigidbody& _rigidbody);
		void RemoveRigidbody(Rigidbody& _rigidbody);

		vector<Collider*> colliders;
		const vector<Collider*>& GetColliders();
		void AddCollider(Collider& _collider);
		void RemoveCollider(Collider& _collider);

		vector<ParticleSystemUpdate*> particleSystems;
		const vector<ParticleSystemUpdate*>& GetParticleSystems();
		void AddParticleSystem(ParticleSystemUpdate& _particleSystem);
		void RemoveParticleSystem(ParticleSystemUpdate& _particleSystem);

		const double m_DeltaSpeed = 10.0;
		float m_PhysicsDeltaTime;

		void SetGravity(Vector2f value)
		{
			gravity = value;
		}

		Int32 GetDeltaTimeMili()
		{
			return m_Time.asMilliseconds();
		}

		float FixedUpdateMovement();

	protected:
		static Physics* _physics;
		const chrono::duration<double> PhysicsTimeStep = 0.005s; // 0.02 for 50 times pr sec, 0.008333 for 120 times pr sec
		Time m_Time;
		Clock m_Clock;
		Vector2f gravity = Vector2f(0, 9.81);

		void UpdateTime()
		{
			m_Time = m_Clock.getElapsedTime();
			m_Clock.restart();
			m_PhysicsDeltaTime = m_Time.asSeconds();
		}

		Physics() {};

		void PhysicsUpdate();
		void ParticleUpdate();
		void PhysicsMovementUpdate();
		void PhysicsCollisionUpdate();

	};
}