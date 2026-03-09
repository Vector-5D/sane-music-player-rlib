#pragma once

#include "Playlist.hpp"
#include "AudioPlayer.hpp"
#include "MetadataManager.hpp"
#include "FontRenderer.hpp"

#include "ui_components/Slider.hpp"
#include "ui_components/ImageButton.hpp"
#include "ui_components/Label.hpp"

#include <raylib.h>
#include <optional>

// Constants for UI components
// Don't try to use these outside the App class
#define PREV_BUTTON_X (316.f)
#define PREV_BUTTON_Y (this->w_height - 50.f)
#define PREV_BUTTON_WIDTH (30.f)
#define PREV_BUTTON_HEIGHT (30.f)

#define PAUSE_RESUME_BUTTON_X (356.f)
#define PAUSE_RESUME_BUTTON_Y (this->w_height - 50.f)
#define PAUSE_RESUME_BUTTON_WIDTH (30.f)
#define PAUSE_RESUME_BUTTON_HEIGHT (30.f)

#define NEXT_BUTTON_X (396.f)
#define NEXT_BUTTON_Y (this->w_height - 50.f)
#define NEXT_BUTTON_WIDTH (30.f)
#define NEXT_BUTTON_HEIGHT (30.f)

#define PROGRESS_SLIDER_X (436.f)
#define PROGRESS_SLIDER_Y (this->w_height - 50.f)
#define PROGRESS_SLIDER_WIDTH (this->w_width - (20.f * 4) - 256.f - (40.f * 3) - 160.f)
#define PROGRESS_SLIDER_HEIGHT (30.f)

#define VOLUME_SLIDER_X (this->w_width - 170.f)
#define VOLUME_SLIDER_Y (this->w_height - 50.f)
#define VOLUME_SLIDER_WIDTH (150.f)
#define VOLUME_SLIDER_HEIGHT (30.f)

#define NOW_PLAYING_LABEL_X (317.f)
#define NOW_PLAYING_LABEL_Y (this->w_height - 77.f)
#define NOW_PLAYING_LABEL_SIZE (16.f)

#define VOLUME_LABEL_X (this->w_width - 170.f)
#define VOLUME_LABEL_Y (this->w_height - 77.f)
#define VOLUME_LABEL_SIZE (16.f)

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
        void        updateUI();
        void        getCurrentArtwork(std::string path, bool init = false);
        const char* detextImageExt(const std::vector<unsigned char>& data);
        void        playNext();
        void        playPrevious();
        void        pauseResume();
        void        updatePauseResumeButton();
        std::string formatTime(float seconds);

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
        std::optional<Slider>      progress_slider;
        std::optional<Slider>      volume_slider;
        std::optional<ImageButton> prev_button;
        std::optional<ImageButton> pause_resume_button;
        std::optional<ImageButton> next_button;
        std::optional<Label>       now_playing_label;
        std::optional<Label>       volume_label;
};
