#pragma once
 
#include "raylib-cpp/raylib-cpp.hpp"
#include "main_app/main_window.hpp"
#include "../interfaces.hpp"

static const int WORKING_FRAME_HEIGHT = 600;
static const int WORKING_FRAME_WIDTH = 1050;
static const int WORKING_FRAME_COORDX = 200;
static const int WORKING_FRAME_COORDY = 100;

static const int NOTIFICATION_FRAME_HEIGHT = 200;
static const int NOTIFICATION_FRAME_WIDTH = 700;
static const int NOTIFICATION_FRAME_COORDX = 1180;
static const int NOTIFICATION_FRAME_COORDY = 0;

static const int PSEUDO_CODE_FRAME_HEIGHT = 130;
static const int PSEUDO_CODE_FRAME_WIDTH = 250;
static const int PSEUDO_CODE_FRAME_COORDX = DEFAULT_WIN_WIDTH - PSEUDO_CODE_FRAME_WIDTH - 10;
static const int PSEUDO_CODE_FRAME_COORDY = 680 - PSEUDO_CODE_FRAME_HEIGHT;

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