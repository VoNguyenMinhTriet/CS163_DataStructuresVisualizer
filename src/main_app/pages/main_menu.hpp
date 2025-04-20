#pragma once

#include "raylib-cpp/raylib-cpp.hpp"
#include "./page.hpp"
#include <memory>
#include <raylib.h>
#include "widget_toolkit/controls/button.hpp"
#include "./heap/HeapVisualizer.hpp"
#include "./GraphVisualizer.hpp"
#include "./linked_list_page.hpp"
#include "main_app/themes/dark_simple/dark_simple.hpp"

namespace ds_viz::pages
{
    class MainMenuPage : public Page
    {
        std::unique_ptr<raylib::Font> font;
        raylib::Text title;

        raywtk::Button _trieButton;
        raywtk::Button _graphButton;
        raywtk::Button _heapButton;
        raywtk::Button _linkedListButton;

        std::function<void()> _deferredStateChange;

    public:
        MainMenuPage(MainWindow &context);

        void Update(float dt) override;

        void Render() override;

    };
}