#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"

using namespace sf;
using namespace std;

namespace Shambles
{
	class UI_Button : public Component, public RectangleShape // should be possible to change shape
	{
	public:
		UI_Button() {};
		UI_Button(Vector2f pos, string& _string, unsigned int characterSize = 30)
		{
			setPosition(pos);
		};

	};
}