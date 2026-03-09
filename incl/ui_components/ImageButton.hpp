#pragma once

#include "Button.hpp"
#include <raylib.h>
#include <string>

class ImageButton : public Button {
    public:
        ImageButton(Vector2 pos, Vector2 size,
            Color regular_color, Color hovered_color, Color clicked_color,
            std::string image_path, int margin);
        ~ImageButton();
        void draw(FontRenderer* renderer = nullptr) override;
        void changeImage(std::string image_path);

    private:
        int       margin;
        Texture2D tex;
};
