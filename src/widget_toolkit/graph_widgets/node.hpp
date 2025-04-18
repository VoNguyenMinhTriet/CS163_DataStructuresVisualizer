#pragma once

#include <functional>
#include "raylib-cpp/raylib-cpp.hpp"
#include "eventpp/callbacklist.h"
#include "../interfaces.hpp"

namespace raywtk 
{
    class NodeWidget : public IScreenElement
    {
        bool held = false;

    public:
        raylib::Vector2 position;
        raylib::Camera2D *camera;
        float radius = 10.0f;
        raylib::Color color = raylib::Color::White();
        bool visible = true;
        std::function<void()> content; // Optional content function
        eventpp::CallbackList<void()> Click; // Optional click handler

        void Update (float deltaTime) override
        {
            if (visible)
            {
                // Check if the mouse is over the node
                if (position.CheckCollision(raylib::Mouse::GetPosition(), radius))
                {
                    if (raylib::Mouse::IsButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        if (!Click.empty()) Click();
                        held = true;
                    }
                }
                
                // Drag handler, to be refactored
                if (held)                
                    position += raylib::Mouse::GetDelta();
            }

            if (raylib::Mouse::IsButtonReleased(MOUSE_LEFT_BUTTON))
                held = false;
        }

        void Render () override
        {
            if (visible)
                position.DrawCircle(radius, color);
            if (content) content(); // Call the content function if provided
        }
    };
}