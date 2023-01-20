#include "pch.h"
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "MousePoint.h"
#include "CircleCollider.h"
#include "Monobehaviour.h"
#include "Debug.h"

namespace Shambles
{
	Scene* Scene::_Scene = nullptr;

	//Scene* Scene::GetInstance()
	//{
	//	if (_Scene == nullptr)
	//	{
	//		_Scene = new Scene();
	//	}
	//	return _Scene;
	//}

	Scene* Scene::GetInstance()
	{
		static Scene instance; // new way, and we never again need to do the if checking every time, and much cleaner
		return &instance;
	}

	vector<string> split(string s, string delimiter)
	{
		size_t pos_start = 0, pos_end, delim_len = delimiter.length();
		string token;
		vector<string> res;

		while ((pos_end = s.find(delimiter, pos_start)) != string::npos)
		{
			token = s.substr(pos_start, pos_end - pos_start);
			pos_start = pos_end + delim_len;
			res.push_back(token);
		}

		res.push_back(s.substr(pos_start));
		return res;
	}

	void Scene::LoadScene()
	{
		//ifstream file_obj(MyScene->m_ScenePath + ".txt", ios::in);
		//stringstream stream;
		//stream << file_obj.rdbuf();

		//vector<string> values = split(stream.str(), ",");

		//float x = stof(values[0]);
		//float y = stof(values[1]);

		//DebugLog(x);
		//DebugLog(y);

		//CircleCollider circle2 = CircleCollider(*new CircleShape(50, 50));
		//GameObject m_circle = GameObject(*circle2.shape, circle2, *new Rigidbody());
		//m_circle.transform->setPosition(x, y);

		//file_obj.close();
	}

	void Scene::LoadScene(string scenePath)
	{
		//ifstream file_obj(scenePath + ".txt", ios::in); // deserializing
		//stringstream stream;
		//stream << file_obj.rdbuf();

		//vector<string> values = split(stream.str(), ",");

		//float x = stof(values[0]);
		//float y = stof(values[1]);

		//DebugLog(x);
		//DebugLog(y);
	}

	void Scene::SaveScene()
	{
		//ofstream file_obj(MyScene->m_ScenePath + ".txt", std::ofstream::trunc); // trunc and no ios::app will clear all contents of the file before writing

		//Vector2f mousePos = MousePos.getPosition();
		//string name = to_string(mousePos.x) + "," + to_string(mousePos.y);

		//// to hard, to much work, you really shouldn't have to make this code yourself
		//// getting weird erros that I dont truly understand, alot of structure and complexity, just not worth the time

		////SceneSerializer s;
		////s.SerializeScene(*this);

		////file_obj << s.SerializeScene(*this);  

		//file_obj << name.c_str();
	}

	void Scene::SetActiveScene(string& scenePath)
	{
		m_ScenePath = scenePath;
	}

	void Scene::AddGameObject(GameObject& gameObject)
	{
		m_GameObjects.push_back(&gameObject);
	}

	void Scene::RemoveGameObject(GameObject& gameObject)
	{
		for (size_t i = 0; i < m_GameObjects.size(); i++)
		{
			if (m_GameObjects[i] == &gameObject)
			{
				m_GameObjects.erase(next(m_GameObjects.begin(), i), next(m_GameObjects.begin(), i + 1));
			}
		}
	}

	const vector<GameObject*>& Scene::GetGameObjects()
	{
		return m_GameObjects;
	}

	string Scene::GetScenePath()
	{
		return m_ScenePath;
	}

	void Scene::InitializeSceneGameObjects()
	{
		return;
	}
}