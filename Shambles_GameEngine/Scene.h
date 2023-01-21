#pragma once
#include "Pch.h"
#include <SFML/Graphics.hpp>
#include "GameObject.h"

#define MyScene Scene::GetInstance()

using namespace sf;
using namespace std;

namespace Shambles
{
    class Scene
    {
    public:
        Scene(Scene& other) = delete;
        void operator=(const Scene&) = delete;

        static Scene& GetInstance();

        void LoadScene();
        void LoadScene(string scenePath);
        void SaveScene();

        vector<GameObject*> m_GameObjects;
        void SetActiveScene(string& scenePath);
        void AddGameObject(GameObject& gameObject);
        void RemoveGameObject(GameObject& gameObject);
        const vector<GameObject*>& GetGameObjects();

        string GetScenePath();

    protected:
        static Scene* _Scene;
        string m_ScenePath = "Scene01"; // can't make static, get error

        Scene() {};

        void InitializeSceneGameObjects(); // after loadscene it call this to try and initialize what ever it read then we return the scene at some point

    };
}