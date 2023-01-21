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

		bool IsActive;
		// need a tagging system(Enum?) and perhaps a layer system so you wont have to do layers purely by numbers
		Transformable* transform; // suppose you could even use this as an id, would really just want to use the memory adress of the gameobject as an identifier, but no, that is terrible hard
		vector<Component*> components;
		const string_view ToString() { return "game object"; };

		void AddComponent(Component& component);
		void RemoveComponent(Component& component);
		const vector<Component*>& GetComponents();

		// can I move this to cpp now?
		template <class T> 
		T* GetComponent(T type); // to be even better is should only allow classes that inherits from component

	private:
		Component* GetComponentType(Component& component);

	};
}