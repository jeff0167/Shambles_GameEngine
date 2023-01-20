#pragma once
#include "Pch.h"
#include <SFML/Graphics.hpp>
#include "Scene.h"

#define SceneManager SceneWindow::GetInstance()

using namespace sf;

namespace Shambles
{
    class SceneWindow
    {
    public:
        SceneWindow(SceneWindow& other) = delete;

        void operator=(const SceneWindow&) = delete;

        static SceneWindow* GetInstance();

        vector<GameObject*> m_GameObjects;

        void DebugInfo();
        void LoadScene();
        void LoadScene(string scenePath);
        void SetActiveScene(string scenePath);
        void AddGameObject(GameObject& gameObject);
        void RemoveGameObject(GameObject& gameObject);
        const vector<GameObject*>& GetGameObjects();

        void DisplaySceneWindow(RenderWindow& window);

    protected:
        static SceneWindow* _SceneWindow;
        Scene* m_Scene;
        SceneWindow() {};

    };
}