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
        float radius = 30.0f;
        raylib::Color color = raylib::Color::Blue();
        std::shared_ptr<raylib::Font> nodefont; 
        bool visible = true;
        std::function<void()> content; // Optional content function
        eventpp::CallbackList<void()> Click; // Optional click handler
        int value; // Store node's value
        std::unique_ptr<NodeWidget> next = nullptr; // Pointer to next node

        NodeWidget(int val, std::shared_ptr<raylib::Font> font) : value(val), nodefont(font)  
        {
            content = [this]()
            {
                std::string text = std::to_string(value);
                int fontSize = 25;
                raylib::Vector2 textSize = MeasureTextEx(*nodefont, text.c_str(), fontSize, 1);
                
                float textWidth = textSize.x;
                float textHeight = textSize.y;
                
                float centeredX = position.x - (textWidth / 2);
                float centeredY = position.y - (textHeight / 2);
                raylib::Vector2 textPos = {centeredX, centeredY};
                DrawTextEx(*nodefont, text, textPos, fontSize, 1, RAYWHITE);
            };
        }

        ~NodeWidget() {}

        void Update (float deltaTime) override
        {
            if (visible)
            {
                if (position.CheckCollision(raylib::Mouse::GetPosition(), radius))
                {
                    if (raylib::Mouse::IsButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        if (!Click.empty()) Click();
                        held = true;
                    }
                }

                if (held)                
                    position += raylib::Mouse::GetDelta();
            }

            if (raylib::Mouse::IsButtonReleased(MOUSE_LEFT_BUTTON))
                held = false;

            if (next)
                next->Update(deltaTime); // Recursively update the next node
        }

        void Render () override
        {
            if (visible)
            {
                position.DrawCircle(radius, color);
                if (content) content();
            }

            if (next)
            {
                raylib::Vector2 start = position;
                raylib::Vector2 end = next->position;

                // Compute direction vector from start to end
                raylib::Vector2 direction = (end - start).Normalize();

                // Adjust positions to be at the edges of the circles
                raylib::Vector2 startEdge = start + (direction * radius);
                raylib::Vector2 endEdge = end - (direction * radius);

                // Draw the line (edge)
                DrawLineEx(startEdge, endEdge, 5, raylib::Color::Yellow());

                // Draw arrowhead
                float arrowSize = 12.0f;
                float arrowAngle = 60.0f * DEG2RAD; // Angle of arrowhead

                raylib::Vector2 leftWing = endEdge - direction * arrowSize +
                raylib::Vector2(cos(arrowAngle) * -direction.x - sin(arrowAngle) * -direction.y,
                            sin(arrowAngle) * -direction.x + cos(arrowAngle) * -direction.y) * arrowSize;

                raylib::Vector2 rightWing = endEdge - direction * arrowSize +
                raylib::Vector2(cos(-arrowAngle) * -direction.x - sin(-arrowAngle) * -direction.y,
                            sin(-arrowAngle) * -direction.x + cos(-arrowAngle) * -direction.y) * arrowSize;

                // Draw arrowhead as a triangle
                DrawTriangle(endEdge, leftWing, rightWing, raylib::Color::Yellow());

                // Recursively render the next node
                next->Render();
            }
        }
    };
}