#pragma once

#include "Playlist.hpp"
#include "AudioPlayer.hpp"
#include "MetadataManager.hpp"
#include "FontRenderer.hpp"

#include <raylib.h>
#include <optional>

class App {
    public:
    App();
    ~App();

    // Primary application loop
    void run();

    private:
        // Primary application loop components
        void handleInput();
        void update();
        void render();

        // Helper functions
        void getCurrentArtwork(std::string path, bool init = false);
        const char* detextImageExt(const std::vector<unsigned char>& data);

        // Application components
        AudioPlayer     player;
        Playlist        playlist;
        MetadataManager metadata;

        // Font renderer (for unicode text)
        std::optional<FontRenderer> fontRenderer;

        // Window dimensions
        int w_width;
        int w_height;

        // Current artwork data
        Texture2D current_artwork;
};
