#include "pch.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "Physics.h"
#include "Canvas.h"
#include "SceneWindow.h"
#include "Collider.h"
#include "Debug.h"

using namespace sf;
using namespace std;

namespace Shambles
{
	GameObject::GameObject(Text& drawShape) :
		transform(&drawShape),
		components(vector<Component*>())
	{
		SceneManager->AddGameObject(*this); // we also need to remove from the scene hiearchy again
		Renderer.AddDrawable(drawShape);
	}

	GameObject::GameObject(Shape& drawShape) :
		transform(&drawShape),
		components(vector<Component*>())
	{
		SceneManager->AddGameObject(*this); // we also need to remove from the scene hiearchy again
		Renderer.AddDrawable(drawShape);
	}

	GameObject::GameObject(Shape& drawShape, Component& _component) :
		transform(&drawShape)
	{
		SceneManager->AddGameObject(*this);
		Renderer.AddDrawable(drawShape);
		AddComponent(_component);
	}

	GameObject::GameObject(Shape& drawShape, Component& _component, Component& _component2) :
		transform(&drawShape)
	{
		SceneManager->AddGameObject(*this);
		Renderer.AddDrawable(drawShape);
		AddComponent(_component);
		AddComponent(_component2);
	}

	GameObject::GameObject(Shape& drawShape, Component& _component, Component& _component2, Component& _component3) :
		transform(&drawShape)
	{
		SceneManager->AddGameObject(*this);
		Renderer.AddDrawable(drawShape);
		AddComponent(_component);
		AddComponent(_component2);
		AddComponent(_component3);
	}

	GameObject::GameObject(Shape& drawShape, const vector<Component*>& _components) :
		transform(&drawShape)
	{
		for (size_t i = 0; i < _components.size(); i++)
		{
			AddComponent(*_components[i]);
		}
		SceneManager->AddGameObject(*this);
		Renderer.AddDrawable(drawShape);
	}

	void GameObject::AddComponent(Component& _component)
	{
		//DebugLog(typeid(_component).name());
		string classType = typeid(_component).name();

		if (classType == "class Shambles::Rigidbody")
		{
			dynamic_cast<Rigidbody&>(_component).transform = transform; // Must manually cast by each class type, super anoying

			auto cc = GetComponent(Collider());
			if (cc != nullptr) cc->rigidbody = &dynamic_cast<Rigidbody&>(_component);

			Science->AddRigidbody(dynamic_cast<Rigidbody&>(_component));
		}
		else if (classType == "class Shambles::CircleCollider" || classType == "class Shambles::BoxCollider") // check if it contains collider in string
		{
			dynamic_cast<Collider&>(_component).rigidbody = GetComponent(Rigidbody());

			Science->AddCollider(dynamic_cast<Collider&>(_component));
		}
		else if (classType == "class Shambles::ParticleSystem")
		{
			auto particleSystem = &dynamic_cast<ParticleSystemUpdate&>(_component);
			particleSystem->SetEmitterTransform(*transform);
			Science->AddParticleSystem(*particleSystem);
		}
		components.push_back(&_component);
		_component.gameObject = this;
	}

	Component* GameObject::GetComponentType(Component& _component)
	{
		string classType = typeid(_component).name();

		if (classType == "class Rigidbody")
		{
			return &(dynamic_cast<Rigidbody&>(_component));
		}
		else if (classType == "class CircleCollider" || classType == "class BoxCollider")
		{
			return &(dynamic_cast<Collider&>(_component));
		}
		return nullptr;
	}

	void GameObject::RemoveComponent(Component& _component) // sure it's no longer in it's list but the component still exist in memory and has shares the same info with gameObject as before
	{
		string removeComponentType = typeid(_component).name();
		for (size_t i = 0; i < components.size(); i++) // it needs to use templates as well to remove components by type and certainly not purely by the memory adress
		{
			string currentComponentType = typeid(*components[i]).name();
			DebugLog("to remove: " + removeComponentType + " current: " + currentComponentType);
			if (removeComponentType == currentComponentType)
			{
				components.erase(next(components.begin(), i), next(components.begin(), i + 1));
			}
		}
	}

	const vector<Component*>& GameObject::GetComponents()
	{
		return components;
	}

	GameObject::~GameObject()
	{
		//DebugLog(typeid(this).name());
		// gameObject should delete all it's related components and references
	}
}