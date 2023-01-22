#include "pch.h"
#include <SFML/Graphics.hpp>
#include "Physics.h"
#include "Rigidbody.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "Monobehaviour.h"
#include "Mathf.h"

using namespace Shambles;
using namespace sf;
using namespace std;

namespace Shambles
{
	void AddForce(Rigidbody* rb, Vector2f force);
	bool Collision(Collider& first, Collider& second);
	bool BoxXBox(BoxCollider& first, BoxCollider& second);
	bool CircleXCircle(CircleCollider& first, CircleCollider& second);
	bool CircleXBox(CircleCollider& first, BoxCollider& second);
	Physics& m_Physics = Science; // caching it's reference to itself for the static functions to use, real pretty

	Physics* Physics::_physics = nullptr;

	Physics& Physics::GetInstance()
	{
		if (_physics == nullptr)
		{
			_physics = new Physics();
			_physics->rigidbodies.reserve(100);
			_physics->colliders.reserve(100);
			_physics->particleSystems.reserve(10);
		}
		return *_physics;
	}

	void Physics::InitializePhysicsUpdate()
	{
		thread t = std::thread([this] { this->PhysicsUpdate(); });
		t.detach();
	}

	void Physics::PhysicsUpdate()
	{
		chrono::time_point<chrono::system_clock, chrono::duration<double, chrono::system_clock::period>> step;

		while (true)
		{
			step = chrono::system_clock::now() + PhysicsTimeStep;

			UpdateTime();
			ParticleUpdate();
			PhysicsMovementUpdate();
			PhysicsCollisionUpdate();
			this_thread::sleep_until(step);
		}
	}

	void Physics::AddParticleSystem(ParticleSystemUpdate& _particleSystem)
	{
		particleSystems.emplace_back(&_particleSystem);
	}

	void Physics::AddRigidbody(Rigidbody& _rigidbody)
	{
		rigidbodies.emplace_back(&_rigidbody);
	}

	void Physics::RemoveRigidbody(Rigidbody& _rigidbody)
	{
		for (size_t i = 0; i < rigidbodies.size(); i++)
		{
			if (rigidbodies[i] == &_rigidbody)
			{
				rigidbodies.erase(next(rigidbodies.begin(), i), next(rigidbodies.begin(), i + 1));
			}
		}
	}

	const vector<Rigidbody*>& Physics::GetRigidbodies()
	{
		return rigidbodies;
	}

	void Physics::PhysicsMovementUpdate()
	{
		float finalDeltaSpeed = FixedUpdateMovement();

		for (const auto& rigidbody : rigidbodies)
		{
			rigidbody->transform->move(rigidbody->velocity * finalDeltaSpeed);

			// Hmm should perhaps have it so you have a list of all objects that use gravity, so you dont do the check
			if (rigidbody->useGravity) AddForce(rigidbody, gravity * finalDeltaSpeed);
		}
	}

	void Physics::AddCollider(Collider& _collider)
	{
		colliders.emplace_back(&_collider);
	}

	void Physics::RemoveCollider(Collider& _collider)
	{
		for (size_t i = 0; i < colliders.size(); i++)
		{
			if (colliders[i] == &_collider)
			{
				colliders.erase(next(colliders.begin(), i), next(colliders.begin(), i + 1));
			}
		}
	}

	const vector<Collider*>& Physics::GetColliders()
	{
		return colliders;
	}

	void Physics::ParticleUpdate()
	{
		for (const auto& particleSystem : particleSystems)
		{
			particleSystem->Update();
		}
	}

	void Physics::PhysicsCollisionUpdate()
	{
		//DebugLog(colliders.size());
		vector<Collider*> tempColliderList = colliders;

		for (size_t i = 0; i < tempColliderList.size(); i++)
		{
			for (size_t j = 0; j < tempColliderList.size(); j++)
			{
				if (i == j) continue;
				//DebugLog("Calling cool, dynamic");
				if (Collision(*colliders[i], *colliders[j]))
				{
					// hmm wonder if this is something heavy execution when the OnCollision func is empty
					auto d = colliders[i]->OnCollision;
					if (d != nullptr)
					{
						d(colliders[j]);
					}
					d = colliders[j]->OnCollision;
					if (d != nullptr)
					{
						d(colliders[i]);
					}
					//DebugLog("Collision");
				}
			}
			tempColliderList.erase(next(tempColliderList.begin(), i), next(tempColliderList.begin(), i + 1));
		}
	}

	void AddForce(Rigidbody* rb, Vector2f force)
	{
		rb->velocity += force;
	}

	static bool Collision(Collider& first, Collider& second) // can't catch and ignore access violation
	{
		string firstName = typeid(first).name(); // really wish I knew how to check for null here of the type
		string secondName = typeid(second).name();

		Vector2f d(0, 0);

		if (firstName.find("Box") != string::npos)
		{
			d.x = 1;
		}

		if (secondName.find("Box") != string::npos)
		{
			d.y = 1;
		}

		if (firstName.find("Circle") != string::npos)
		{
			d.x = 2;
		}

		if (secondName.find("Circle") != string::npos)
		{
			d.y = 2;
		}

		if (d.x == 1 && d.y == 1) // box x box
		{
			return BoxXBox(static_cast<BoxCollider&>(first), static_cast<BoxCollider&>(second)); // dynamic cast is slower because it has to check the type and will return null if false
		}
		if (d.x == 2 && d.y == 2) // circle x circle
		{
			return CircleXCircle(static_cast<CircleCollider&>(first), static_cast<CircleCollider&>(second));
		}
		if ((d.x == 2 && d.y == 1)) // circle x box
		{
			return CircleXBox(static_cast<CircleCollider&>(first), static_cast<BoxCollider&>(second));
		}
		if ((d.x == 1 && d.y == 2))
		{
			return CircleXBox(static_cast<CircleCollider&>(second), static_cast<BoxCollider&>(first));
		}
		return false;
	}

	// cleans up the code a bit, though not sure if it would impact the performance
	float Physics::FixedUpdateMovement()
	{
		return (float)m_DeltaSpeed * m_PhysicsDeltaTime;
	}

	static bool BoxXBox(BoxCollider& first, BoxCollider& second) // box x box
	{
		double xi, xj; // we would also want to not move when colliding with static objects
		xi = (first.rigidbody != NULL) ? first.rigidbody->Magnitude() : 0;
		xj = (second.rigidbody != NULL) ? second.rigidbody->Magnitude() : 0;

		if (first.rect->getGlobalBounds().intersects(second.rect->getGlobalBounds())) // we are not able to use offset, should the gameObject's transform be an offset of the shape instead?!?
		{
			//if (xi == 0 || xj == 0) return true;
			if (xi > xj)
			{
				// this might need some updates values, or other values, seems to work alright atm
				//second.rigidbody->transform->move(first.rigidbody->velocity * m_Physics.FixedUpdateMovement());
			}
			//else first.rigidbody->transform->move(second.rigidbody->velocity * m_Physics.FixedUpdateMovement());
			//DebugLog("bound intersect");
			return true;
		}
		return false;
	}
	static bool CircleXCircle(CircleCollider& first, CircleCollider& second) // circle x circle
	{
		double xi, xj;
		xi = (first.rigidbody != NULL) ? first.rigidbody->Magnitude() : 0;
		xj = (second.rigidbody != NULL) ? second.rigidbody->Magnitude() : 0;

		double distance = sqrt(pow((first.rect->getPosition().x + first.offsetPos.x) -
			(second.rect->getPosition().x + second.offsetPos.x), 2) + pow((first.rect->getPosition().y + first.offsetPos.y) - (second.rect->getPosition().y + second.offsetPos.y), 2));

		if (distance < abs(first.rect->getRadius() + second.rect->getRadius()))
		{
			//if (xi == 0 || xj == 0) return true;
			if (xi > xj)
			{
				second.rigidbody->transform->move(first.rigidbody->velocity * m_Physics.FixedUpdateMovement());
			}
			else first.rigidbody->transform->move(second.rigidbody->velocity * m_Physics.FixedUpdateMovement());
			//DebugLog("Circle collision");
			return true;
		}
		return false;
	}
	static bool CircleXBox(CircleCollider& first, BoxCollider& second) // circle x box
	{
		double xi, xj; // we would also want to not move when colliding with static objects
		xi = (first.rigidbody != NULL) ? first.rigidbody->Magnitude() : 0;
		xj = (second.rigidbody != NULL) ? second.rigidbody->Magnitude() : 0;

		if (first.rect->getGlobalBounds().intersects(second.rect->getGlobalBounds()))
		{
			//if (xi == 0 || xj == 0) return true;

			if (xi > xj)
			{
				second.rigidbody->transform->move(first.rigidbody->velocity * m_Physics.FixedUpdateMovement());
			}
			else first.rigidbody->transform->move(second.rigidbody->velocity * m_Physics.FixedUpdateMovement());
			//DebugLog("bound intersect");
			return true;
		}
		//DebugLog("bound not intersect");
		return false;
	}
}