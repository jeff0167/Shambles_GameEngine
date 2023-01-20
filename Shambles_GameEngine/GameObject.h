#pragma once
#include "Pch.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

namespace Shambles
{
	class Component;
	class GameObject
	{
	protected:

	public:
		GameObject() {};
		~GameObject();
		GameObject(Text& drawShape);
		GameObject(Shape& drawShape);
		GameObject(Shape& drawShape, Component& _component);
		GameObject(Shape& drawShape, Component& _component, Component& _component2);
		GameObject(Shape& drawShape, Component& _component, Component& _component2, Component& _component3);
		GameObject(Shape& drawShape, const vector<Component*>& _components);

		Transformable* transform; // suppose you could even use this as an id, would really just want to use the memory adress of the gameobject as an identifier, but no, that is terrible hard
		vector<Component*> components;

		void AddComponent(Component& component);
		void RemoveComponent(Component& component);
		const vector<Component*>& GetComponents();

		template <class T>
		T* GetComponent(T type) // to be even better is should only allow classes that inherits from component
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

	private:
		Component* GetComponentType(Component& component);

	};
}