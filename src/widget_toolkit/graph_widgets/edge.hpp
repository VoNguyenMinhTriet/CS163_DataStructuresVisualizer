#pragma once

#include "raylib-cpp/raylib-cpp.hpp"
#include "../interfaces.hpp"

namespace raywtk 
{
    class EdgeWidget : public IScreenElement
    {
    public:
        raylib::Vector2 start;
        raylib::Vector2 end;
        float thickness = 1.0f;
        raylib::Color color = raylib::Color::White();
        bool visible = true;

        

        void Update (float deltaTime) override
        {
            // No update logic for edges
        }

        void Render () override
        {
            if (visible)
                start.DrawLine(end, thickness, color);
        }
    };
}