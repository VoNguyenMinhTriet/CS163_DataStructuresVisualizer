#pragma once

#include "raylib-cpp/raylib-cpp.hpp"
#include "../interfaces.hpp"

static const int WORKING_FRAME_HEIGHT = 700;
static const int WORKING_FRAME_WIDTH = 1000;
static const int WORKING_FRAME_COORDX = 500;
static const int WORKING_FRAME_COORDY = 200;

static const int NOTIFICATION_FRAME_HEIGHT = 200;
static const int NOTIFICATION_FRAME_WIDTH = 700;
static const int NOTIFICATION_FRAME_COORDX = 500;
static const int NOTIFICATION_FRAME_COORDY = 1000;

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