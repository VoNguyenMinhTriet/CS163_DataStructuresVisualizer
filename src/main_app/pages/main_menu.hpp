#pragma once

#include <memory>
#include "raylib-cpp/Rectangle.hpp"
#include "raylib-cpp/raylib-cpp.hpp"
#include "./page.hpp"
#include "./trie/trie_page.hpp"
#include "widget_toolkit/controls/button.hpp"

namespace ds_viz::pages
{
    class MainMenuPage : public Page
    {
        std::unique_ptr<raylib::Font> font;
        raylib::Text title;

        // Controls
        raywtk::Button _trieButton;

      public:
        MainMenuPage(MainWindow &context);

        void Update(float dt) override;

        void Render() override;
    };
}
