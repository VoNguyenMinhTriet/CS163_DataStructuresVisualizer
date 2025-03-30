#pragma once

#include <memory>
#include "raylib-cpp/raylib-cpp.hpp"
#include "./page.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "./LinkedList.hpp"

namespace ds_viz::pages
{
    class MainMenuPage : public Page
    {
        std::unique_ptr<raylib::Font> font;
        raylib::Text title;
        std::unique_ptr<raywtk::Button> LLButton;

    public:
        
    MainMenuPage () {
        font = std::unique_ptr<raylib::Font>(new raylib::Font("./ttf/InterDisplay-Black.ttf", 128, 0, 250));
        title = raylib::Text("DATA LA VISTA", 128, raylib::Color::White(), *font, 0);

        LLButton = std::make_unique<raywtk::Button>();
        LLButton->buttonRect = raylib::Rectangle(300, 400, 200, 60); 
        LLButton->Click.append([this]() { OnLLButtonClick(); });
    }

    void OnLLButtonClick()
    {
        _context->ChangePage(std::make_shared<ds_viz::pages::LinkedListPage>());
    }

    void Update(float dt) override
    {
        LLButton->Update(dt);
    }

    void Render() override;
        
    };
}
