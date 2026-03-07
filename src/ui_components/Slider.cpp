#include "ui_components/Slider.hpp"

Slider::Slider(Vector2 pos, Vector2 size, Color col_bg, Color col_fg) {
    this->rect_bg = (Rectangle){ pos.x, pos.y, size.x, size.y };
    this->rect_fg = (Rectangle){ pos.x, pos.y, 0.0f, size.y };
    this->col_bg = col_bg;
    this->col_fg = col_fg;
    this->value = 0.0f;
}

Slider::~Slider() {
    // default deconstructor
}

void Slider::draw() {
    DrawRectangleRec(rect_bg, col_bg);
    DrawRectangleRec(rect_fg, col_fg);
}

void Slider::updateValue(float value) {
    this->value = value;
    rect_fg.width = value * rect_bg.width;
}

void Slider::updateSize(Vector2 size) {
    rect_bg.height = size.y;
    rect_fg.height = size.y;
    rect_bg.width = size.x;
    rect_fg.width = value * rect_bg.width;
}

void Slider::updatePos(Vector2 pos) {
    rect_bg.x = pos.x;
    rect_fg.x = pos.x;
    rect_bg.y = pos.y;
    rect_fg.y = pos.y;
}
