#pragma once

#include "Button.hpp"
#include "FontRenderer.hpp"
#include <string>

class TextButton : public Button {
    public:
        TextButton(
            Vector2 pos, Vector2 size,
            Color regular_color, Color hovered_color, Color clicked_color,
            std::string text, Color text_color, int font_size, int margin);
        void draw(FontRenderer* renderer = nullptr) override;

    private:
        std::string text;
        int         margin;
        int         font_size;
        Color       text_color;
};
