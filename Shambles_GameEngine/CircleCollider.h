#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"

using namespace sf;
using namespace std;

namespace Shambles
{
	class CircleCollider : public Collider
	{
	public:
		CircleShape* rect = nullptr;
		CircleCollider() {};
		CircleCollider(CircleShape& _rect, Vector2f pos = Vector2f(0, 0), Color color = Color::White) :
			rect(&_rect)
		{
			rect->setPosition(pos);
			rect->setOrigin(rect->getRadius(), rect->getRadius());
			rect->setFillColor(color);
			shape = &_rect;
		};

	private:

	};
}