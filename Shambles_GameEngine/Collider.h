#pragma once
#include "Pch.h"
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "Rigidbody.h"

using namespace sf;
using namespace std;

namespace Shambles
{
    class Collider : public Component
    {
    public:
        Vector2f offsetPos = Vector2f(0, 0);
        Shape* shape;
        Rigidbody* rigidbody;
        function<void(Collider* other)> OnCollision;

        Collider() {};

    private:

    };
}