#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "Pch.h"

using namespace sf;
using namespace std;

namespace Shambles
{
	class SpriteRenderer : public RectangleShape, public Component
	{
	public:
		SpriteRenderer();
		SpriteRenderer(string imageFilePath, Vector2f imageSize, Vector2f imagePos, int layerNr = 5);
		SpriteRenderer(Texture& image, Vector2f imageSize, Vector2f imagePos, int layerNr = 5);
		Texture* m_Texture;

		void setSizeResetOrigin(float x, float y) { this->setSize(Vector2f(x,y)); this->setOrigin(this->getSize().x / 2, this->getSize().y / 2); };
		void setSizeResetOrigin(Vector2f size) { this->setSize(size); this->setOrigin(this->getSize().x / 2, this->getSize().y / 2); };

		int m_layerNr;

	};
}