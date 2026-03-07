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
    UnloadTexture(this->current_artwork);
    CloseWindow();
    LOG_INFO("App deinitialized successfully.");
}

void App::initUI() {
    getCurrentArtwork("", true);

    this->progress_slider.emplace(
        Vector2{ PROGRESS_SLIDER_X, PROGRESS_SLIDER_Y },
        Vector2{ PROGRESS_SLIDER_WIDTH, PROGRESS_SLIDER_HEIGHT },
        DARKERGRAY,
        LIGHTGRAY
    );

    this->test_button_1.emplace(
        Vector2{this->w_width - 120.f, 20}, Vector2{100, 100},
        DARKERGRAY, DARKGRAY, LIGHTGRAY,
        "assets/icons/icon4.png", 5
    );

    this->test_button_1->setOnClick(
        [](){ LOG_INFO("You clicked a button."); }
    );

    this->test_button_2.emplace(
        Vector2{this->w_width - 120.f, 140}, Vector2{100, 100},
        DARKERGRAY, DARKGRAY, LIGHTGRAY,
        "Hi", WHITE, 24, 5
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
        this->playlist.playPrevious(this->player);
    if (IsKeyPressed(KEY_RIGHT))
        this->playlist.playNext(this->player);

    if (IsKeyPressed(KEY_SPACE)) {
        if (this->player.isPaused())
            this->player.resume();
        else
            this->player.pause();
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

    this->progress_slider->updateValue(this->song_progress);
    this->progress_slider->updateSize({ PROGRESS_SLIDER_WIDTH, PROGRESS_SLIDER_HEIGHT });
    this->progress_slider->updatePos({ PROGRESS_SLIDER_X, PROGRESS_SLIDER_Y });

    this->test_button_1->update(mouse, clicked);
    this->test_button_2->update(mouse, clicked);
}

void App::render() {
    BeginDrawing();
    ClearBackground(BLACK);

    this->fontRenderer->loadSize(22);
    this->fontRenderer->drawText("Genres", 20, 15, LIGHTGRAY);

    Track* track = this->playlist.getCurrentTrack();
    if (track) {
        DrawTextureEx(this->current_artwork, { 20.f, float(this->w_height - 276) }, 0.f, 0.5, WHITE);

        this->fontRenderer->setSize(24);
        this->fontRenderer->drawText(
            track->getTitle(), 317, w_height - 122, WHITE);

        this->fontRenderer->setSize(16);
        this->fontRenderer->drawText(
            track->getArtist(), 317, w_height - 93, GRAY);
        this->fontRenderer->drawText(
            track->getAlbum(), 317, w_height - 75, DARKGRAY);
    } else {
        DrawTextureEx(this->current_artwork, { 20.f, float(this->w_height - 276) }, 0.f, 0.5, WHITE);

        this->fontRenderer->setSize(24);
        this->fontRenderer->drawText(
            "No song currently playing.", 317, w_height - 104, WHITE);

        this->fontRenderer->setSize(16);
        this->fontRenderer->drawText(
            "Select a song using CTRL+O.", 317, w_height - 75, GRAY);
    }

    this->progress_slider->draw();
    this->test_button_1->draw();
    this->test_button_2->draw(&this->fontRenderer.value());

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
