#pragma once

#include "Playlist.hpp"
#include "AudioPlayer.hpp"
#include "MetadataManager.hpp"
#include "FontRenderer.hpp"

#include <optional>

class App {
public:
    App();
    ~App();

    // Primary application loop
    void run();

private:
    void handleInput();
    void update();
    void render();

    AudioPlayer            player;
    Playlist               playlist;
    MetadataManager        metadata;
    std::optional<FontRenderer> fontRenderer;

    // Window dimensions
    int w_width;
    int w_height;
};
