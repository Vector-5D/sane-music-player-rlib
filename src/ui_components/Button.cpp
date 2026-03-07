#include "ui_components/Button.hpp"
#include "FontRenderer.hpp"

Button::Button(Vector2 pos, Vector2 size, Color regular_color, Color hovered_color, Color clicked_color) {
    this->pos = pos;
    this->size = size;
    this->regular_color = regular_color;
    this->hovered_color = hovered_color;
    this->clicked_color = clicked_color;
}

void Button::draw(FontRenderer* renderer) {
    if (this->clicked) {
        DrawRectangle(this->pos.x, this->pos.y, this->size.x, this->size.y, this->clicked_color);
    } else if (this->hovered) {
        DrawRectangle(this->pos.x, this->pos.y, this->size.x, this->size.y, this->hovered_color);
    } else {
        DrawRectangle(this->pos.x, this->pos.y, this->size.x, this->size.y, this->regular_color);
    }

    // Base class does not draw text, derived classes may override this behavior
}

void Button::update(Vector2 mouse_pos, bool is_pressed) {
    this->hovered = mouse_pos.x >= this->pos.x && mouse_pos.x <= this->pos.x + this->size.x &&
                     mouse_pos.y >= this->pos.y && mouse_pos.y <= this->pos.y + this->size.y;
    this->clicked = this->hovered && is_pressed;

    if (this->clicked) {
        this->onClick();
    }
}

void Button::onClick() {
    if (this->on_click) {
        this->on_click();
    }
}

void Button::setOnClick(std::function<void()> on_click) {
    this->on_click = on_click;
}
