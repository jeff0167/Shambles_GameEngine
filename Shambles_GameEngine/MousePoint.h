#pragma once
#include "Pch.h"
#include <SFML/Graphics.hpp>

using namespace sf;

namespace Shambles
{
#define MousePos MousePoint::GetInstance()->mousePixel

	class MousePoint
	{
	public:
		RectangleShape mousePixel = RectangleShape(Vector2f(1, 1));

		MousePoint(MousePoint& other) = delete;

		void operator=(const MousePoint&) = delete;

		static MousePoint* GetInstance();

	protected:
		static MousePoint* _MousePoint;
		MousePoint() {};

	};
}