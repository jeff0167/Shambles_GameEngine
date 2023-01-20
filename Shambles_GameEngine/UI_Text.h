#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"

using namespace sf;
using namespace std;

namespace Shambles
{
	class UI_Text : public Component, public Text // could you somehow set it to call base in initialization?!?! tohugh it would have two different bases?
	{
	public:
		UI_Text() {};
		UI_Text(Vector2f pos, const string& _string, unsigned int characterSize = 30, sf::Color color = sf::Color::Black)
		{
			static Font t;
			if (t.loadFromFile("BebasNeue-Regular.ttf")) // remember this still gets called each time
			{
				setFont(t);
			}

			//setOrigin(pos.x / 2, pos.y / 2); // still need to figure out origin, finicky
			setFillColor(color);
			setPosition(pos);
			setString(_string); // try use scope static to get default font
			setCharacterSize(characterSize);
		};

	};
}