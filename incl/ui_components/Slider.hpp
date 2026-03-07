#pragma once

#include <raylib.h>
class Slider {
    public:
        Slider(Vector2 pos, Vector2 size, Color col_bg, Color col_fg);
        ~Slider();

        void draw();
        void updateValue(float value);
        void updateSize(Vector2 size);
        void updatePos(Vector2 pos);

    private:
        Rectangle rect_bg;
        Rectangle rect_fg;
        Color     col_fg;
        Color     col_bg;
        float     value;
};
