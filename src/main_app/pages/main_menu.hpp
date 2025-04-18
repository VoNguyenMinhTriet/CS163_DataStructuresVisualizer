#pragma once

#include "raylib-cpp/raylib-cpp.hpp"
#include "./page.hpp"
#include <memory>
#include <raylib.h>
#include "widget_toolkit/controls/button2.hpp"
#include "widget_toolkit/controls/text_button.hpp"
#include "./HeapVisualizer.hpp"

namespace ds_viz::pages
{
    class MainMenuPage : public Page
    {
        std::unique_ptr<raylib::Font> font;
        raylib::Text title;
        std::vector<std::unique_ptr<raywtk::Button>> buttons;

        public:

        MainMenuPage () 
        {
            font = std::unique_ptr<raylib::Font>(new raylib::Font("./ttf/InterDisplay-Black.ttf", 128, 0, 250));
            title = raylib::Text("DATA LA VISTA", 128, raylib::Color::White(), *font, 0);
            CreateButton("Heap Visualization", 200, 300, [this]() { OnLLButtonClick(); });
        }

        void CreateButton(const std::string& text, float x, float y, std::function<void()> callback)
        {
            auto button = std::make_unique<raywtk::Button>();
            button->buttonRect = raylib::Rectangle(x, y, 200, 60);
            button->buttonText = text;
            button->Click.append(callback);
            button->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();
            buttons.push_back(std::move(button));
        }

        void OnLLButtonClick()
        {
            _context->ChangePage(std::make_shared<ds_viz::pages::HeapVisualizer>());
        }

        void Update (float dt) override 
        {
            for (auto& button: buttons) 
            {
                button->Update(dt);
            }
        }

        void Render () override;
    };
}
