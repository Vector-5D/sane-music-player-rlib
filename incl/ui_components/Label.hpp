#pragma once

#include "FontRenderer.hpp"
#include <raylib.h>

class Label {
    public:
        Label(Vector2 pos, float size, Color color, std::string text);
        void draw(FontRenderer& renderer);
        void updatePos(Vector2 pos);
        void updateSize(float size);
        void updateColor(Color color);
        void updateText(std::string text);

    protected:
        Vector2     pos;
        float       size;
        Color       color;
        std::string text;
};
