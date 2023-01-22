#include "pch.h"
#include "SpriteRenderer.h"

namespace Shambles
{
	SpriteRenderer::SpriteRenderer()
	{
	}
	SpriteRenderer::SpriteRenderer(string imageFilePath, Vector2f imageSize, Vector2f imagePos, int layerNr) :
		m_layerNr(layerNr)
	{
		static Texture t;
		if (t.loadFromFile(imageFilePath)) // it will load from file each time
		{
			// now that I think of it, you should only load a texture once as it is a slow task and we will just reuse the texture for whoever is using it
			// which means that there should be a system that manages loading textures if not also loading other files and if the files is already loaded
			// then just pass a reference to the one and only existing loaded file
			m_Texture = &t;
			setTexture(m_Texture);
		}
		setSize(imageSize);
		setOrigin(imageSize.x / 2, imageSize.y / 2);
		setPosition(imagePos);
	}

	SpriteRenderer::SpriteRenderer(Texture& image, Vector2f imageSize, Vector2f imagePos, int layerNr) :
		m_layerNr(layerNr)
	{
		//m_Texture = image;
		setTexture(&image);
		setSize(imageSize);
		setOrigin(imageSize.x / 2, imageSize.y / 2);
		setPosition(imagePos);
	}
}
