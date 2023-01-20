#include "pch.h"
#include <SFML/Graphics.hpp>
#include "SceneWindow.h"
#include "Canvas.h"
#include "Monobehaviour.h"
#include "CircleCollider.h"
#include "Debug.h"
#include "UI_Text.h"
#include "MousePoint.h"

using namespace sf;
using namespace std;

namespace Shambles
{
	static void Draw();
	static void MouseCreation();
	static void CreateDefaultSceneWindowObjects();

	RenderWindow* _window;

	UI_Text text;
	Font font;

	bool m_DisplayMenu = false;

	SceneWindow* SceneWindow::_SceneWindow = nullptr;

	SceneWindow* SceneWindow::GetInstance()
	{
		if (_SceneWindow == nullptr)
		{
			_SceneWindow = new SceneWindow();
		}
		return _SceneWindow;
	}

	void SceneWindow::AddGameObject(GameObject& gameObject)
	{
		//m_GameObjects.push_back(&gameObject);
		// save to scene to serialize or could just send the scenewindows gameObjects
		MyScene->AddGameObject(gameObject);
	}

	void SceneWindow::RemoveGameObject(GameObject& gameObject)
	{
		for (size_t i = 0; i < m_GameObjects.size(); i++)
		{
			if (m_GameObjects[i] == &gameObject)
			{
				m_GameObjects.erase(next(m_GameObjects.begin(), i), next(m_GameObjects.begin(), i + 1));
			}
		}
	}

	const vector<GameObject*>& SceneWindow::GetGameObjects()
	{
		return m_GameObjects;
	}

	// suppose the sceneWindow should hold an instance of the current scene to be manipulated with and can be changed out
	// the sandbox would need to tell which scene needs to be loaded
	// a scene class may or may not have a scene text file to load
	// suppose you could make a instance of a scene and tell it/ write the code of what it should hold
	// the scene would then be your work space
	// you somehow need a entry point into the gameloop?
	// or perhaps the gameloop already has all it's functions that it needs and you just subscribe things to those events to be called

	void SceneWindow::DisplaySceneWindow(RenderWindow& _windowRef)
	{
		_window = &_windowRef;
		CreateDefaultSceneWindowObjects();

		_window->setFramerateLimit(120);
		while (_window->isOpen())
		{
			Mono.UpdateTime();

			//DebugFrameRate(Mono->_time);
			Event _event;
			while (_window->pollEvent(_event))
			{
				switch (_event.type)
				{
				case Event::Closed:
					_window->close();
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

			MouseCreation();
			Draw();
		}
	}

	// as a developer you would just say on mouse event and that would 
	// be called or added as a event for the mouse and will be triggered automaticly

	static void MouseCreation()
	{
		Vector2f mousePos = (Vector2f)Mouse::getPosition(*_window); // can't i manually set it so you don't ever have to touch it!??!
		MousePos.setPosition(mousePos.x, mousePos.y);
		if (Mouse::isButtonPressed(Mouse::Left)) // check all clickable objects for their pos, oh no, another thing that needs to keep track of a list of objects
		{
			if (text.getGlobalBounds().intersects(MousePos.getGlobalBounds()))
			{
				MyScene->SaveScene();
				//DebugLog("Clicked on menu"); // now do what the text tells you!
			}
			m_DisplayMenu = false;
		}
		if (Mouse::isButtonPressed(Mouse::Right)) // spawn menu at mouse pos, here you can make a gameObject
		{
			m_DisplayMenu = true;
			text.setPosition((mousePos.x), (mousePos.y));
		}
		if (!m_DisplayMenu)
		{
			//text.setPosition((mousePos.x) + 10000, (mousePos.y) + 10000);
		}
	}

	static void Draw()
	{
		_window->clear(Color(255, 204, 92));
		Renderer.DrawCanvas();
		_window->display();
	}

	void SceneWindow::DebugInfo()
	{
		// write debug info in game/Scene window
	}

	void SceneWindow::LoadScene()
	{
		// we load the curent scene
		// we take all it's objects and add them?!?
		// well if they are created within the scene or perhaps sceneSerializer then that will create the objects and the objects when created are automaticly added to the scenemanager hiearchy
		// so the scene just need to deserialize and create the objects
		m_Scene->LoadScene();
	}

	void SceneWindow::LoadScene(string scenePath)
	{
		m_Scene->LoadScene(scenePath);
	}

	void SceneWindow::SetActiveScene(string scenePath)
	{
		SceneManager->SetActiveScene(scenePath);
	}

	static void CreateDefaultSceneWindowObjects()
	{
		if (!font.loadFromFile("BebasNeue-Regular.ttf"))
		{
			DebugLog("Could not load font");
		}
		text = UI_Text(Vector2f(200, 200), "Menu", 50);
		text.setFont(font);
		text.setOrigin((text.getString().getSize() * text.getCharacterSize()) / 5.0f, text.getCharacterSize() / 2); // roughly the center
		text.setFillColor(Color::Black);

		GameObject g = GameObject(text);
	}
}