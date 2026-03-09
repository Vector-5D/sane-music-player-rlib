#pragma once

#include "FontRenderer.hpp"
#include <raylib.h>
#include <functional>

class Button {
    public:
        Button(Vector2 pos, Vector2 size, Color regular_color, Color hovered_color, Color clicked_color);
        virtual void draw(FontRenderer* renderer = nullptr);
        virtual void updateState(Vector2 mouse_pos, bool is_pressed);
        void updatePos(Vector2 pos);
        void updateSize(Vector2 size);
        void onClick();
        void setOnClick(std::function<void()> on_click);

    protected:
        Vector2 pos;
        Vector2 size;
        Color   regular_color;
        Color   hovered_color;
        Color   clicked_color;
        bool    hovered;
        bool    clicked;

        // Click action
        std::function<void()> on_click;
};
