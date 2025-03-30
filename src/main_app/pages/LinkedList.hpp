#pragma once

#include <memory>
#include <vector>
#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "main_app/themes/dark_simple/button.hpp"
#include "./page.hpp"


namespace ds_viz::pages 
{
    class LinkedListPage : public Page 
    {
        std::unique_ptr<raylib::Font> font;
        raylib::Text title;
        std::vector<std::unique_ptr<raywtk::Button>> Buttons;

    public:
    
    LinkedListPage(); 
    void OnReturnButtonClick();
    void Update(float dt) override;
    void Render() override;
    
    };
}

