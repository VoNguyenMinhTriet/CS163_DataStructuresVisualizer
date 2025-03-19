#pragma once

#include <memory>
#include "raylib-cpp/raylib-cpp.hpp"
#include "pages/page.hpp"
#include "widget_toolkit/interfaces.hpp"
#include "widget_toolkit/theme_manager.hpp"

#define DEFAULT_FPS 60
#define DEFAULT_WIN_WIDTH 1280
#define DEFAULT_WIN_HEIGHT 720

namespace ds_viz
{
    // This class also serves as the app's lifetime.
    class MainWindow : public raywtk::IRenderable
    {
    private:
        int fixedFPS = DEFAULT_FPS;
        int initScreenWidth = DEFAULT_WIN_WIDTH, initScreenHeight = DEFAULT_WIN_HEIGHT;
        std::string title;
        int raylibFlags = FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT;

    public:
        // Storing the value would invoke the destructor after the assignment in Run(),
        // ending the program prematurely. Ah, how I love programming with C++.
        std::unique_ptr<raylib::Window> ref_raylib_window;

        std::unique_ptr<raywtk::IStyle> style;
        std::shared_ptr<Page> content;

        void Render () override;
        void ChangePage (std::shared_ptr<Page> to);
        int Run ();
    };
}
