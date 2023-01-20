#pragma once
#include <SFML/Graphics.hpp>
//#include "GameObject.h"
#include "Debug.h"

#define Mono Monobehaviour::GetInstance()
#define CreateObject(x, y) Instantiate(x,y)

using namespace sf;
using namespace std;

namespace Shambles
{
	struct MeasureTime
	{
		chrono::time_point<chrono::steady_clock> start;
		MeasureTime()
		{
			start = chrono::high_resolution_clock::now();
		}
		~MeasureTime()
		{
			DebugLog(to_string(chrono::duration<float>(chrono::high_resolution_clock::now() - start).count() * 1000.0f) + "ms");
		}
	};

	class Monobehaviour
	{
	public:
		Monobehaviour(Monobehaviour& other) = delete;

		void operator=(const Monobehaviour&) = delete;

		static Monobehaviour& GetInstance();

		//static GameObject Instantiate(GameObject gameObject);
		//static GameObject Instantiate(GameObject gameObject, Transformable transform);

		//static void Destroy(GameObject* gameObject);

		static void Invoke(function<void()> function, float callDelay);

		Time _time;
		Clock _clock;
		float DeltaTime;

		MeasureTime Timer()
		{
			return MeasureTime();
		}

		void UpdateTime()
		{
			_time = _clock.getElapsedTime();
			_clock.restart();
			DeltaTime = _time.asSeconds();
		}

		float GetDeltaTimeMili()
		{
			return _time.asMilliseconds();
		}

	private:
		static void DelayedCall(function<void()> function, float callDelay);

		Monobehaviour() {};
	};
}