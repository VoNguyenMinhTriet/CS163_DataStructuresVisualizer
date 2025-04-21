#pragma once

#include "raylib-cpp/raylib-cpp.hpp"
#include "../interfaces.hpp"

namespace raywtk 
{
    class DisplayFrame : public IScreenElement
    {
        public:
        raylib::Rectangle frame;
        raylib::Color color = raylib::Color::Gray();
        float thickness = 5.0f;
        bool visible = true;

        DisplayFrame(raylib::Rectangle frame, raylib::Color color, float thickness) : frame(frame), color(color), thickness(thickness) {}

        void Update(float deltaTime) override
        {
            
        }

        void Render() override
        {
            if (visible)
            {
                DrawRectangleLinesEx(frame, thickness, color);
            }
        }

        
        bool IsPositionInside(raylib::Vector2 position)
        {
            return CheckCollisionPointRec(position, frame);
        }
    };
}