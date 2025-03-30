#pragma once

#include "./page.hpp"
#include <memory>
#include "raylib-cpp/raylib-cpp.hpp"

namespace ds_viz::pages 
{
    class LinkedListPage : public Page 
    {
        std::unique_ptr<raylib::Font> font;
        raylib::Text title;

    public:
    
    LinkedListPage () 
    {
        font = std::unique_ptr<raylib::Font>(new raylib::Font("./ttf/InterDisplay-Black.ttf", 128, 0, 250));
        title = raylib::Text("Singly-Linked List", 128, raylib::Color::White(), *font, 0);
    }
        
    void Update(float dt) override
    {

    }

    void Render() override;
    
    };
}