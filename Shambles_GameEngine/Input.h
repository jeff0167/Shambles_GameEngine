#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

namespace Shambles
{
	class Input
	{
	public:
		enum Keycode
		{
			KeyDown,
			KeyHeld,
			KeyUp
		};
		static bool GetKey(Keyboard::Key key, Keycode keykode);

	private:
		static bool GetKeyDown(Keyboard::Key key);
		static bool GetKeyHeld(Keyboard::Key key);
		static bool GetKeyUp(Keyboard::Key key);
	};
}
