#include "pch.h"
#include "Animation.h"

namespace Shambles
{
	Animation::Animation(Texture* texture, Vector2u _imageCount, float _switchFrameTime) :
		imageCount(_imageCount), switchFrameTime(_switchFrameTime)
	{
		currentImage.x = 0;

		uvRect.width = texture->getSize().x / float(imageCount.x);
		uvRect.height = texture->getSize().y / float(imageCount.y);
	}

	void Animation::Update(int row, float deltaTime)
	{
		currentImage.y = row;
		totalTime += deltaTime;

		if (totalTime >= switchFrameTime)
		{
			totalTime -= switchFrameTime;
			currentImage.x++;

			if (currentImage.x >= imageCount.x)
			{
				currentImage.x = 0;
			}
		}

		uvRect.left = currentImage.x * uvRect.width;
		uvRect.top = currentImage.y * uvRect.height;
	}

	void Animation::Update(int row, int rowStartFrame, int rowEndFrame, float deltaTime)
	{
		currentImage.y = row;
		totalTime += deltaTime;

		if (totalTime >= switchFrameTime)
		{
			totalTime -= switchFrameTime;
			currentImage.x++;

			if (currentImage.x >= rowEndFrame)
			{
				currentImage.x = rowStartFrame;
			}
		}

		uvRect.left = currentImage.x * uvRect.width;
		uvRect.top = currentImage.y * uvRect.height;
	}

	void Animation::NextAnim() // not perfect but works
	{
		totalTime = 1;
	}
}