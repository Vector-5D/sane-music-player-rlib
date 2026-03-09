#include "ui_components/ImageButton.hpp"
#include <raylib.h>

ImageButton::ImageButton(Vector2 pos, Vector2 size,
    Color regular_color, Color hovered_color, Color clicked_color,
    std::string image_path, int margin)
    : Button(pos, size, regular_color, hovered_color, clicked_color),
      margin(margin)
{
    Image img = LoadImage(image_path.c_str());

    // Resize based on button size and margin
    int target_w = size.x - margin * 2;
    int target_h = size.y - margin * 2;
    ImageResize(&img, target_w, target_h);

    this->tex = LoadTextureFromImage(img);
    SetTextureFilter(this->tex, TEXTURE_FILTER_BILINEAR);

    UnloadImage(img);
}

ImageButton::~ImageButton() {
    UnloadTexture(this->tex);
}

void ImageButton::draw(FontRenderer* renderer) {
    Button::draw(renderer);
    DrawTexture(this->tex, this->pos.x + margin, this->pos.y + margin, RAYWHITE);
}

void ImageButton::changeImage(std::string image_path) {
    
    Image img = LoadImage(image_path.c_str());

    // Resize based on button size and margin
    int target_w = size.x - margin * 2;
    int target_h = size.y - margin * 2;
    ImageResize(&img, target_w, target_h);

    UnloadTexture(this->tex);
    this->tex = LoadTextureFromImage(img);
    SetTextureFilter(this->tex, TEXTURE_FILTER_BILINEAR);

    UnloadImage(img);
}
