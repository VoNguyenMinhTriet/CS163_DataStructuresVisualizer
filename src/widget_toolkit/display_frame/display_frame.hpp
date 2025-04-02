#pragma once

#include "raylib-cpp/raylib-cpp.hpp"
#include "../interfaces.hpp"

static const int WORKING_FRAME_HEIGHT = 1100;
static const int WORKING_FRAME_WIDTH = 2000;
static const int WORKING_FRAME_COORDX = 500;
static const int WORKING_FRAME_COORDY = 200;

namespace raywtk 
{
    class DisplayFrame : public IScreenElement
    {
        public:
        raylib::Rectangle frame;
        raylib::Vector2 pos;
        raylib::Color color = raylib::Color::Gray();
        float thickness = 2.0f;
        bool visible = true;

        DisplayFrame(raylib::Rectangle frame = raylib::Rectangle(WORKING_FRAME_COORDX, WORKING_FRAME_COORDY, WORKING_FRAME_WIDTH, WORKING_FRAME_HEIGHT)) : frame(frame) {}

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