#pragma once

#include <functional>
#include <memory>
#include <vector>
#include "raylib-cpp/Rectangle.hpp"
#include "raylib-cpp/raylib-cpp.hpp"
#include "./page.hpp"
#include "./heap/HeapVisualizer.hpp"
#include "main_app/themes/dark_simple/dark_simple.hpp"
#include "./GraphVisualizer.hpp"
#include "./trie/trie_page.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "./linked_list_page.hpp"
#include "main_app/themes/dark_simple/text_button.hpp"

namespace ds_viz::pages
{
    class MainMenuPage : public Page
    {
        std::unique_ptr<raylib::Font> font;
        raylib::Text title;

        // Controls
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
