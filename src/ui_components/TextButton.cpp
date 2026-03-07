#include "ui_components/TextButton.hpp"

TextButton::TextButton(
    Vector2 pos, Vector2 size,
    Color regular_color, Color hovered_color, Color clicked_color,
    std::string text, Color text_color, int font_size, int margin)
    : Button(pos, size, regular_color, hovered_color, clicked_color),
      text(text), margin(margin), font_size(font_size), text_color(text_color) {}

void TextButton::draw(FontRenderer* renderer) {
    Button::draw(renderer);
    renderer->setSize(font_size);
    if (renderer) {
        renderer->drawText(text, this->pos.x + margin, this->pos.y + margin, text_color);
    }
}
