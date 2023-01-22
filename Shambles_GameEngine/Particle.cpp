#include "pch.h"
#include "ParticleSystem.h"
#include "Physics.h"

namespace Shambles
{
	float Umapi(float value, float istart, float istop, float ostart, float ostop)
	{
		return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
	}

	Particle::~Particle()
	{
	}

	void Particle::Update()
	{
		if (IsDead())
		{
			lifespan = Time::Zero;
		}

		int alpha = (lifespan.asSeconds() / (maxLifespan.asSeconds())) * 255;
		//dot.setFillColor(SetColor(Umapi(alpha, 255, 0, 360, 0), 1, 1, alpha));
		 
		Color c = dot.getFillColor(); // this is slow, like a lot
		c.a = alpha;
		dot.setFillColor(c); // can't simply set the alpha value
		if (lifespan.asSeconds() == 0)
		{
			lifespan = maxLifespan;
			position.x = 0;
			position.y = 0;

			startPos = GetTargetPos();
		}
		velocity += acceleration;
		position += velocity * Science.FixedUpdateMovement();
		dot.setPosition(position + startPos);

		acceleration = acceleration * 0.f;
		lifespan = milliseconds(lifespan.asMilliseconds() - Science.GetDeltaTimeMili());
	}

	void Particle::SetPosition(float x, float y)
	{
		position.x = x;
		position.y = y;

		dot.setPosition(position);
	}

	void Particle::SetPosition(Vector2f pos)
	{
		position = pos;

		dot.setPosition(position);
	}

	void Particle::SetVelocity(float x, float y)
	{
		velocity.x = x;
		velocity.y = y;
	}

	void Particle::SetMaxLifeTime(Time time)
	{
		maxLifespan = time;
	}

	void Particle::SetParticleSystem(ParticleSystem& ps)
	{
		m_ParticleSystem = &ps;
	}

	Vector2f Particle::GetTargetPos()
	{
		return m_ParticleSystem->GetTargetTransform();
	}

	void Particle::ApplyForce(Vector2f force)
	{
		acceleration += force;
	}

	Vector2f Particle::GetVelocity()
	{
		return velocity;
	}

	bool Particle::IsDead()
	{
		if (lifespan.asSeconds() < 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	Color Particle::SetColor(int hue, float sat, float colorMulti, int alpha)
	{
		hue %= 360;
		while (hue < 0) hue += 360;

		if (sat < 0.f) sat = 0.f;
		if (sat > 1.f) sat = 1.f;

		if (colorMulti < 0.f) colorMulti = 0.f;
		if (colorMulti > 1.f) colorMulti = 1.f;

		int h = hue / 60;
		float f = float(hue) / 60 - h;
		Uint8 p = (colorMulti * (1.f - sat)) * 255;
		Uint8 q = (colorMulti * (1.f - sat * f)) * 255;
		Uint8 t = (colorMulti * (1.f - sat * (1 - f))) * 255;
		Uint8 colorMultiX255 = colorMulti * 255;
	
		switch (h)
		{
		default:
		case 0:
		case 6: return sf::Color(colorMultiX255, t, p, alpha);
		case 1: return sf::Color(q, colorMultiX255, p, alpha);
		case 2: return sf::Color(p, colorMultiX255, t, alpha);
		case 3: return sf::Color(p, q, colorMultiX255, alpha);
		case 4: return sf::Color(t, p, colorMultiX255, alpha);
		case 5: return sf::Color(colorMultiX255, p, q, alpha);
		}
	}
}
