#include "App.hpp"
#include "FileDialog.hpp"
#include "Logger.hpp"
#include "MetadataManager.hpp"
#include <raylib.h>

App::App() : w_width(800), w_height(450)
{
    InitWindow(this->w_width, this->w_height, "Sane Music Player");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(144);
    Image icon = LoadImage("assets/icons/icon4.png");
    SetWindowIcon(icon);
    UnloadImage(icon);

    this->fontRenderer.emplace(48, 48, 512);
    this->fontRenderer->loadFile("assets/fonts/NotoSansJP-Medium.ttf");
    this->fontRenderer->loadSize(16);
    this->fontRenderer->loadSize(24);

    this->song_progress = 0.0f;
    this->song_duration = 0;

    initUI();

    LOG_INFO("App initialized successfully.");
}

App::~App() {
    this->player.stop();
    UnloadTexture(this->current_artwork);
    CloseWindow();
    LOG_INFO("App deinitialized successfully.");
    std::exit(0); // dirty fix for player hanging after window closes
}

void App::initUI() {
    getCurrentArtwork("", true);

    this->progress_slider.emplace(
        Vector2{ PROGRESS_SLIDER_X, PROGRESS_SLIDER_Y },
        Vector2{ PROGRESS_SLIDER_WIDTH, PROGRESS_SLIDER_HEIGHT },
        DARKERGRAY, LIGHTGRAY
    );

    this->progress_slider->setOnSeek([this](float ratio) {
        this->player.setProgress(ratio);
    });

    this->prev_button.emplace(
        Vector2{ PREV_BUTTON_X, PREV_BUTTON_Y },
        Vector2{ PREV_BUTTON_WIDTH, PREV_BUTTON_HEIGHT },
        DARKERGRAY, DARKGRAY, LIGHTGRAY,
        "assets/icons/prev.png", 5
    );

    this->pause_resume_button.emplace(
        Vector2{ PAUSE_RESUME_BUTTON_X, PAUSE_RESUME_BUTTON_Y },
        Vector2{ PAUSE_RESUME_BUTTON_WIDTH, PAUSE_RESUME_BUTTON_HEIGHT },
        DARKERGRAY, DARKGRAY, LIGHTGRAY,
        "assets/icons/play.png", 5
    );

    this->next_button.emplace(
        Vector2{ NEXT_BUTTON_X, NEXT_BUTTON_Y },
        Vector2{ NEXT_BUTTON_WIDTH, NEXT_BUTTON_HEIGHT },
        DARKERGRAY, DARKGRAY, LIGHTGRAY,
        "assets/icons/next.png", 5
    );

    this->prev_button->setOnClick([this](){
            playPrevious();
    });

    this->pause_resume_button->setOnClick([this](){
            pauseResume();
    });

    this->next_button->setOnClick([this](){
            playNext();
    });

    this->now_playing_label.emplace(
        Vector2{ NOW_PLAYING_LABEL_X, NOW_PLAYING_LABEL_Y },
        NOW_PLAYING_LABEL_SIZE,
        DARKGRAY,
        "No song currently playing. Select a song using CTRL+O."
    );
}

void App::run() {
    while (!WindowShouldClose()) {
        this->handleInput();
        this->update();
        this->render();
    }
}

void App::handleInput() {
    // Playback controls
    if (IsKeyPressed(KEY_LEFT))
        playPrevious();
    if (IsKeyPressed(KEY_RIGHT))
        playNext();
    if (IsKeyPressed(KEY_SPACE)) {
        pauseResume();
    }

    // Volume controls
    if (IsKeyPressed(KEY_DOWN)) {
        float v = this->player.getVolume();
        this->player.setVolume(v - 0.05f <= 0.0f ? 0.0f : v - 0.05f);
    }
    if (IsKeyPressed(KEY_UP)) {
        float v = this->player.getVolume();
        this->player.setVolume(v + 0.05f >= 1.0f ? 1.0f : v + 0.05f);
    }

    // File IO
    if (IsKeyDown(KEY_LEFT_CONTROL) &&
        IsKeyDown(KEY_LEFT_SHIFT) &&
        IsKeyPressed(KEY_O)) {
        // TODO: Implement folder scan part after Library exists
    } else if (IsKeyDown(KEY_LEFT_CONTROL) &&
               IsKeyPressed(KEY_O)) {
        auto result = FileDialog::openFile("mp3,flac,wav,ogg,m4a");
        if (result) {
            // TODO: once Library exists, check track from there instead
            Track* track = new Track(*result);
            getCurrentArtwork(track->getPath());
            this->metadata.populateTrack(*track);
            this->playlist.clear();
            this->playlist.append(track);
            this->playlist.playCurrent(this->player);
            this->song_progress = 0.0f;
            this->song_duration = track->getDuration();
            updatePauseResumeButton();
        }
    }
}

void App::update() {
    this->w_width  = GetScreenWidth();
    this->w_height = GetScreenHeight();

    this->song_progress = this->player.getProgress();
    this->song_duration = this->player.getDurationSeconds();

    updateUI();

    if (this->player.isFinished())
        this->playlist.playNext(this->player);
}

void App::updateUI() {
    Vector2 mouse = GetMousePosition();
    bool clicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    this->prev_button->updateState(mouse, clicked);
    this->prev_button->updatePos({ PREV_BUTTON_X, PREV_BUTTON_Y });
    this->prev_button->updateSize({ PREV_BUTTON_WIDTH, PREV_BUTTON_HEIGHT });

    this->pause_resume_button->updateState(mouse, clicked);
    this->pause_resume_button->updatePos({ PAUSE_RESUME_BUTTON_X, PAUSE_RESUME_BUTTON_Y });
    this->pause_resume_button->updateSize({ PAUSE_RESUME_BUTTON_WIDTH, PAUSE_RESUME_BUTTON_HEIGHT });

    this->next_button->updateState(mouse, clicked);
    this->next_button->updatePos({ NEXT_BUTTON_X, NEXT_BUTTON_Y });
    this->next_button->updateSize({ NEXT_BUTTON_WIDTH, NEXT_BUTTON_HEIGHT });

    this->progress_slider->updateValue(this->song_progress);
    this->progress_slider->updatePos({ PROGRESS_SLIDER_X, PROGRESS_SLIDER_Y });
    this->progress_slider->updateSize({ PROGRESS_SLIDER_WIDTH, PROGRESS_SLIDER_HEIGHT });
    this->progress_slider->updateState(mouse, clicked);

    this->now_playing_label->updatePos({ NOW_PLAYING_LABEL_X, NOW_PLAYING_LABEL_Y });

    if (this->player.hasSoundLoaded()) {
        Track* track = this->playlist.getCurrentTrack();
        this->now_playing_label->updateText(
            track->getTitle() + " • " +
            track->getArtist() + " • " +
            track->getAlbum()
        );
        this->now_playing_label->updateColor(LIGHTGRAY);
    } else {
        this->now_playing_label->updateText("No song currently playing. Select a song using CTRL+O.");
        this->now_playing_label->updateColor(DARKGRAY);
    }
}

void App::render() {
    BeginDrawing();
    ClearBackground(BLACK);

    this->fontRenderer->loadSize(16);

    DrawTextureEx(this->current_artwork, { 20.f, float(this->w_height - 276) }, 0.f, 0.5, WHITE);
    this->progress_slider->draw();
    this->prev_button->draw();
    this->pause_resume_button->draw();
    this->next_button->draw();
    this->now_playing_label->draw(this->fontRenderer.value());

    DrawLine(0, this->w_height - 296, 296, this->w_height - 296, DARKGRAY);
    DrawLine(296, 0, 296, this->w_height, DARKGRAY);

    EndDrawing();
}

void App::getCurrentArtwork(std::string path, bool init) {
    if (init) { // use the fallback cover on startup
        Image img = LoadImage("assets/icons/fallback.png");
        ImageResize(&img, 512, 512);
        Texture2D tex = LoadTextureFromImage(img);
        SetTextureFilter(tex, TEXTURE_FILTER_BILINEAR);
        this->current_artwork = tex;
        UnloadImage(img);
        return;
    }

    std::vector<unsigned char> data = metadata.getArtwork(path);
    std::string extension = detextImageExt(data);

    Image img;
    if (extension == ".jpg" || extension == ".png") {
        img = LoadImageFromMemory(extension.c_str(), data.data(), data.size());
        if (img.data == nullptr) {
            img = LoadImage("assets/icons/fallback.png");
        }
    } else {
        img = LoadImage("assets/icons/fallback.png"); // Fallback icon
    };

    ImageResize(&img, 512, 512);
    Texture2D tex = LoadTextureFromImage(img);
    SetTextureFilter(tex, TEXTURE_FILTER_BILINEAR);
    this->current_artwork = tex;
    UnloadImage(img);
}

const char* App::detextImageExt(const std::vector<unsigned char>& data) {
    if (data.size() < 4) return nullptr;
    if (data[0] == 0xFF && data[1] == 0xD8 && data[2] == 0xFF) return ".jpg";
    if (data[0] == 0x89 && data[1] == 0x50 && data[2] == 0x4E && data[3] == 0x47) return ".png";
    return ".jpg"; // Fallback extension
}

void App::playNext() {
    this->playlist.playNext(this->player);
    this->pause_resume_button->changeImage("assets/icons/pause.png");
}

void App::playPrevious() {
    this->playlist.playPrevious(this->player);
    this->pause_resume_button->changeImage("assets/icons/pause.png");
}

void App::pauseResume() {
    if (this->player.isPlaying()) {
        this->player.pause();
    } else {
        this->player.resume();
    }
    updatePauseResumeButton();
}

void App::updatePauseResumeButton() {
    if (this->player.isPlaying()) {
        this->pause_resume_button->changeImage("assets/icons/pause.png");
    } else {
        this->pause_resume_button->changeImage("assets/icons/play.png");
    }
}
