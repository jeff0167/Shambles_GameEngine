#include "pch.h"
#include "Input.h"

using namespace sf;
using namespace std;

namespace Shambles
{
	static bool notPressedKeys[100] = {}; // there is about a houndred keys
	static bool pressedNotReleased[100] = {}; // suppose you could do the same with mouse input?

	static bool keyEvent[3] = { false };

	bool Input::GetKey(Keyboard::Key key, Keycode keycode)
	{
		keyEvent[0] = GetKeyDown(key);
		keyEvent[1] = GetKeyHeld(key);
		keyEvent[2] = GetKeyUp(key);

		return keyEvent[keycode];
	}

	bool Input::GetKeyDown(Keyboard::Key key)
	{
		if (Keyboard::isKeyPressed(key))
		{
			pressedNotReleased[key] = true;
			if (!notPressedKeys[key])
			{
				return notPressedKeys[key] = true;
			}
		}
		return false;
	}

	bool Input::GetKeyHeld(Keyboard::Key key)
	{
		return Keyboard::isKeyPressed(key);
	}

	bool Input::GetKeyUp(Keyboard::Key key)
	{
		if (!Keyboard::isKeyPressed(key) && pressedNotReleased[key])
		{
			pressedNotReleased[key] = false;
			notPressedKeys[key] = false;
			return true;
		}
		return false;
	}
}