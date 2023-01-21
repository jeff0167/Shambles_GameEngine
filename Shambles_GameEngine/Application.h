#pragma once
#include "pch.h"
#include <SFML/Graphics.hpp>
#include "Mathf.h"
#include "Debug.cpp"
#include "Canvas.h"
#include "Monobehaviour.h"
#include "Physics.h"

using namespace sf;
using namespace std;
using namespace Shambles;  //  precompiled header file: stdafx.h      before pch

int windowHeight = 1200;
int windowWidth = 1200;
RenderWindow window(VideoMode(windowWidth, windowHeight), "Shambles dragon game", Style::Close | Style::Titlebar | Style::Resize);

float deltaTime = 0;
Clock _clock;

vector<function<void()>> UpdateFunctions;
vector<function<void()>> AwakeFunctions;

namespace Shambles 
{
	class Application
	{
	public:

		void AddAwakeFunctions(function<void()> function)
		{
			AwakeFunctions.push_back(function);
		}

		void AddUpdateFunctions(function<void()> function)
		{
			UpdateFunctions.push_back(function);
		}

		__declspec(dllexport) int Start()
		{
			Renderer.AddWindow(window);
			Science.InitializePhysicsUpdate();
			Awake();

			window.setFramerateLimit(120); // this should also be changeable
			while (window.isOpen()) // checking window events
			{
				Mono.UpdateTime();
	
				DebugFrameRate(Mono._time);
				Event _event;
				while (window.pollEvent(_event))
				{
					switch (_event.type)
					{
					case Event::Closed:
						window.close();
						break;
					case Event::Resized:
						printf("New window width: %i New window heigth: %i \n", _event.size.width, _event.size.height);
					case Event::TextEntered:
						if (_event.text.unicode < 128)
						{
							printf("%c", _event.text.unicode);
						}
						break;
					}
				}

				Update();
				//void FixedUpdate(); // big hmmm

				Draw();
			}
			return 0;
		}

		void Awake() // some kind of listener, needs like a list of listeners og delegate that you would add to automaticly from the outside by simply using awake/update
		{
			for (const auto& awakeFunction : AwakeFunctions)
			{
				awakeFunction();
			}
		}

		void Update()
		{
			for (const auto& updateFunction : UpdateFunctions)
			{
				updateFunction();
			}
		}

		void Draw()
		{
			window.clear(Color(255, 204, 92));
			Renderer.DrawCanvas();
			window.display();
		}
	};
}