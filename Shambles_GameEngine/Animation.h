#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

namespace Shambles
{
	class Animation
	{
	public:
		IntRect uvRect;

		Animation() {};
		Animation(const Texture* texture, Vector2u _imageCount, float _switchFrameTime);

		void Update(int row, float deltaTime);
		void Update(int row, int rowStartFrame, int fowEndFrame, float deltaTime);
		void NextAnim();

	private:
		Vector2u imageCount;
		Vector2u currentImage;

		float totalTime = 0;
		float switchFrameTime;
	};
}