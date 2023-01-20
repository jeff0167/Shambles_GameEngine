#pragma once
#include "Pch.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

namespace Shambles
{
	class ParticleSystemUpdate
	{
	public:
		virtual void Update() = 0;

		virtual void SetEmitterTransform(Transformable& transform) = 0;
	};
}