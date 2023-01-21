#pragma once
#include "Pch.h"
#include <SFML/Graphics.hpp>
#include "GameObject.h"

using namespace sf;
using namespace std;

namespace Shambles
{
	class Component
	{
	public:
		GameObject* gameObject;
		bool IsActive;

		Component() {};
		virtual void AbstractNullFunc()
		{
		}

		virtual string ToString()
		{
			string className = typeid(*this).name();
			return "Im a " + className;
		}

	private:

	};
}