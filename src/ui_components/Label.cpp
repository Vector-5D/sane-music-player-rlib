#include "ui_components/Label.hpp"

Label::Label(Vector2 pos, float size, Color color, std::string text) {
    this->pos = pos;
    this->size = size;
    this->color = color;
    this->text = text;
}

void Label::draw(FontRenderer& renderer) {
    renderer.setSize(size);
    renderer.drawText(text, pos.x, pos.y, color);
}

void Label::updatePos(Vector2 pos) {
    this->pos = pos;
}

void Label::updateSize(float size) {
    this->size = size;
}

void Label::updateColor(Color color) {
    this->color = color;
}

void Label::updateText(std::string text) {
    this->text = text;
}
