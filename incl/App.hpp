#pragma once

#include "Playlist.hpp"
#include "AudioPlayer.hpp"
#include "MetadataManager.hpp"
#include "FontRenderer.hpp"

#include "ui_components/Slider.hpp"
#include "ui_components/ImageButton.hpp"
#include "ui_components/TextButton.hpp"

#include <raylib.h>
#include <optional>

// Constants for UI components
// Don't try to use these outside App
#define PROGRESS_SLIDER_X (20.f + 256.f + 20.f + 20.f)
#define PROGRESS_SLIDER_Y (this->w_height - 50.f)
#define PROGRESS_SLIDER_WIDTH (this->w_width - (20.f * 4) - 256.f)
#define PROGRESS_SLIDER_HEIGHT 30

#define DARKERGRAY Color{ 23, 23, 23, 255 }

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

        // Constructor helpers
        void initUI();

        // Application helpers
        void updateUI();
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

        // Current song state
        float song_progress;
        int   song_duration;

        // UI components
        std::optional<Slider> progress_slider;
        std::optional<ImageButton> test_button_1;
        std::optional<TextButton> test_button_2;
};
