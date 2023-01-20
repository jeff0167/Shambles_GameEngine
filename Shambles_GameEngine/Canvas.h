#pragma once
#include <SFML/Graphics.hpp>

#define Renderer Canvas::GetInstance()

using namespace sf;
using namespace std;

namespace Shambles
{
    class Canvas
    {
    public:
        Canvas(Canvas& other) = delete;

        void operator=(const Canvas&) = delete;

        RenderTarget* window;
        static Canvas& GetInstance();

        void DrawCanvas();
        void AddDrawable(Drawable& drawable, int layerNr = 5);
        void RemoveDrawable(Drawable& _drawable);
        const vector<vector<Drawable*>>& GetDrawables();
        void AddWindow(RenderTarget& renderTarget);

        vector<vector<Drawable*>> drawablesLayers = { {},{},{},{},{},{},{},{},{},{} };

        void ChangeDrawableLayer(Drawable& _drawable, int layerNr);

    protected:

        Canvas() {};

    };
}