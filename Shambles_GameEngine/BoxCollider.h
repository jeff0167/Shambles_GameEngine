#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"

using namespace sf;
using namespace std;

namespace Shambles
{
	class BoxCollider : public Collider
	{
	public:
		RectangleShape* rect;
		BoxCollider() {};
		BoxCollider(RectangleShape& _rect, Vector2f pos = Vector2f(0, 0), Color color = Color::White) :
			rect(&_rect)
		{
			rect->setPosition(pos);
			rect->setOrigin(rect->getSize().x, rect->getSize().y);
			rect->setFillColor(color);
			shape = &_rect;
		};

	private:

	};
}