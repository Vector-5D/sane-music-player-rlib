#pragma once

#include <raylib.h>
#include <functional>

class Slider {
    public:
        Slider(Vector2 pos, Vector2 size, Color col_bg, Color col_fg);
        ~Slider();

        void draw();
        void updateValue(float value);
        void updateSize(Vector2 size);
        void updatePos(Vector2 pos);
        void updateState(Vector2 mouse_pos, bool is_pressed);
        void setOnSeek(std::function<void(float)> on_seek);

    private:
        Rectangle rect_bg;
        Rectangle rect_fg;
        Color     col_fg;
        Color     col_bg;
        float     value;

        // callback
        std::function<void(float)> on_seek;
};
