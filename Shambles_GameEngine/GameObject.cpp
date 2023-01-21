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

	void GameObject::AddComponent(Component& _component) ///TODO remember to add UI elements and spriteRenderer
	{
		//DebugLog(typeid(_component).name());
		string classType = typeid(_component).name();

		if (classType == "class Shambles::Rigidbody") // use switch? or divide it into function calls
		{
			dynamic_cast<Rigidbody&>(_component).transform = transform; // Must manually cast by each class type, super anoying

			auto cc = GetComponent(Collider());
			if (cc != nullptr) cc->rigidbody = &dynamic_cast<Rigidbody&>(_component);

			Science.AddRigidbody(dynamic_cast<Rigidbody&>(_component));
		}
		else if (classType == "class Shambles::CircleCollider" || classType == "class Shambles::BoxCollider") // check if it contains collider in string
		{
			dynamic_cast<Collider&>(_component).rigidbody = GetComponent(Rigidbody());

			Science.AddCollider(dynamic_cast<Collider&>(_component));
		}
		else if (classType == "class Shambles::ParticleSystem")
		{
			auto particleSystem = &dynamic_cast<ParticleSystemUpdate&>(_component);
			particleSystem->SetEmitterTransform(*transform);
			Science.AddParticleSystem(*particleSystem);
		}
		components.emplace_back(&_component);
		_component.gameObject = this;
	}

	Component* GameObject::GetComponentType(Component& _component)
	{
		string classType = typeid(_component).name();

		if (classType == "class Rigidbody")
		{
			return &(dynamic_cast<Rigidbody&>(_component));
		}
		else if (classType == "class Shambles::CircleCollider" || classType == "class Shambles::BoxCollider")
		{
			return &(dynamic_cast<Collider&>(_component));
		}
		return nullptr;
	}

	void GameObject::RemoveComponent(Component& _component) /// TODO this needs to work, just like add it needs to do things based on the component and remove all references and be DELETED
	{
		string removeComponentType = typeid(_component).name();
		for (size_t i = 0; i < components.size(); i++) 
		{
			string currentComponentType = typeid(*components[i]).name();
			DebugLog("to remove: " + removeComponentType + " current: " + currentComponentType);
			if (removeComponentType == currentComponentType)
			{
				components.erase(next(components.begin(), i), next(components.begin(), i + 1));
			}
		}
	}

	template<class T>
	inline T* GameObject::GetComponent(T type)
	{
		for (auto component : components)
		{
			if (typeid(*component).name() == typeid(type).name()) // *this->components[i]).name(), remember the * in front!!
			{
				return &dynamic_cast<T&>(*component);
			}
		}

		string typeName = typeid(type).name();
		if (!typeName.find("Collider")) return nullptr;

		for (auto component : components) // is this even necessarcy!? ok say you just want a collider, not a specifik one, then you would do this, yes
		{
			string s = typeid(*component).name();
			if (s.find("Collider") != string::npos)	return &dynamic_cast<T&>(*component);
		}
		return nullptr;
	}

	const vector<Component*>& GameObject::GetComponents()
	{
		return components;
	}

	GameObject::~GameObject()
	{
		for (auto component : components)
		{
			RemoveComponent(*component);
		}
		//DebugLog(typeid(*this).name());
		// gameObject should delete all it's related components and references
	}
}