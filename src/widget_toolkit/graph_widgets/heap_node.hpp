#pragma once

#include <functional>
#include "raylib-cpp/raylib-cpp.hpp"
#include "eventpp/callbacklist.h"
#include "../interfaces.hpp"

namespace raywtk 
{
    class HeapNodeWidget : public IScreenElement
    {
        bool held = false;

    public:
        raylib::Vector2 position;
        raylib::Camera2D *camera;
        float radius = 20.0f;
        raylib::Color color = raylib::Color::Blue();
        raylib::Color highlighedColor = raylib::Color::Yellow();
        bool visible = true;
        std::function<void()> content; // Optional content function
        eventpp::CallbackList<void()> Click; // Optional click handler
        int value; // Store node's value
        bool highlighted = false;

        HeapNodeWidget(int val): value(val), content([this]() 
        {
            std::string text = std::to_string(value);
            int fontSize = 20;
            int textWidth = MeasureText(text.c_str(), fontSize);
            int textHeight = fontSize;
            float centeredX = position.x - (textWidth / 2);
            float centeredY = position.y - (textHeight / 2);
            DrawText(text.c_str(), centeredX, centeredY, fontSize, BLACK);
        }) {}

        void Update (float deltaTime) override
        {
        }

        void Render () override
        {
            if (visible)
            {
                position.DrawCircle(radius, (highlighted) ? highlighedColor : color);
                if (content) content();
            }
        }
    };
}