#pragma once

#include <functional>
#include "raylib-cpp/raylib-cpp.hpp"
#include "eventpp/callbacklist.h"
#include "../interfaces.hpp"
#include "widget_toolkit/display_frame/display_frame.hpp"

namespace raywtk {
    class NodeWidget : public IScreenElement {
        bool held = false;

    public:
        raylib::Vector2 position;
        raylib::Vector2 velocity = {0.0f, 0.0f}; // Velocity for physics-based movement
        raylib::Camera2D* camera;
        float radius = 30.0f;
        raylib::Color color = raylib::Color::Blue();
        bool visible = true;
        std::function<void()> content; // Optional content function
        eventpp::CallbackList<void()> Click; // Optional click handler
        int value; // Store node's value
        bool hightlighted = false; // In Kruskal's processing

        NodeWidget(int val) : value(val), content([this]() {
            std::string text = std::to_string(value);
            int fontSize = 30;
            int textWidth = MeasureText(text.c_str(), fontSize);
            int textHeight = fontSize;
            float centeredX = position.x - (textWidth / 2);
            float centeredY = position.y - (textHeight / 2);
            if (!hightlighted)
                DrawText(text.c_str(), centeredX, centeredY, fontSize, RAYWHITE);
            else
                DrawText(text.c_str(), centeredX, centeredY, fontSize + 10, RED);
        }) {}

        // Copy constructor
        NodeWidget(const NodeWidget& other)
            : value(other.value), position(other.position), velocity(other.velocity), radius(other.radius),
              color(other.color), visible(other.visible), Click(other.Click), hightlighted(other.hightlighted) {
            // Reinitialize the content lambda to ensure it references the correct state
            content = [this]() {
                std::string text = std::to_string(value);
                int fontSize = 30;
                int textWidth = MeasureText(text.c_str(), fontSize);
                int textHeight = fontSize;
                float centeredX = position.x - (textWidth / 2);
                float centeredY = position.y - (textHeight / 2);
                if (!hightlighted)
                    DrawText(text.c_str(), centeredX, centeredY, fontSize, RAYWHITE);
                else
                    DrawText(text.c_str(), centeredX, centeredY, fontSize + 10, RED);
            };
        }

        // Repulsive force between nodes
        void InteractWith(NodeWidget& other) {

            auto getDistance = [](const raylib::Vector2& a, const raylib::Vector2& b) {
                return (a - b).Length();
            };

            // Calculate the vector between the two nodes
            raylib::Vector2 delta = position - other.position;
            float distance = delta.Length();
        
            // Minimum distance to avoid division by zero and ensure nodes don't overlap
            float minDistance = radius + other.radius;
            if (distance < minDistance) {
                distance = minDistance; // Clamp the distance to avoid extreme forces
            }
        
            // Calculate the repulsive force using an inverse-square law
            float forceMagnitude = 8000.0f / (distance * distance); // Adjust the constant (1000.0f) for desired strength
            raylib::Vector2 force = delta.Normalize() * forceMagnitude;
        
            // Apply the repulsive force to both nodes
            if(getDistance(position, other.position) > 6 * radius) return;
            velocity += force;
            other.velocity -= force;
        }

        // Attractive force between connected nodes (edges)
        void ApplyAttractiveForce(NodeWidget& other, float springConstant, float desiredDistance) {
            raylib::Vector2 delta = position - other.position;
            float distance = delta.Length();
            float forceMagnitude = springConstant * (distance - desiredDistance); // Hooke's law
            raylib::Vector2 force = delta.Normalize() * forceMagnitude;

            position -= force;
            other.position += force;
        }

        // Apply gravity toward a center point
        void ApplyGravity(raylib::Vector2 center, float gravityStrength) {
            raylib::Vector2 delta = center - position;
            position += delta.Normalize() * gravityStrength;
        }

        // Apply friction to reduce velocity over time
        void ApplyFriction(float frictionCoefficient) {
            velocity *= (1.0f - frictionCoefficient);
        }

        // Constrain the node to stay within bounds
        void ConstrainToBounds(raylib::Rectangle bounds) {
            if (position.x - radius < bounds.x) position.x = bounds.x + radius;
            if (position.x + radius > bounds.x + bounds.width) position.x = bounds.x + bounds.width - radius;
            if (position.y - radius < bounds.y) position.y = bounds.y + radius;
            if (position.y + radius > bounds.y + bounds.height) position.y = bounds.y + bounds.height - radius;
        }

        void Update(float deltaTime) override {}

        // Update the node's position and interactions
        void Update(float deltaTime, std::vector<std::unique_ptr<NodeWidget>>& allNodes, raylib::Rectangle bounds) {
            if (visible) {
                // Handle mouse interactions
                if (position.CheckCollision(raylib::Mouse::GetPosition(), radius)) {
                    if (raylib::Mouse::IsButtonPressed(MOUSE_LEFT_BUTTON)) {
                        if (!Click.empty()) Click();
                        held = true;
                    }
                }
        
                if (held) {
                    position += raylib::Mouse::GetDelta();
                    ConstrainToBounds(bounds);
                }
        
                if (raylib::Mouse::IsButtonReleased(MOUSE_LEFT_BUTTON))
                    held = false;
                
                //velocity = raylib::Vector2(0.0f, 0.0f); // Reset velocity on mouse release
        
                // Interact with other nodes
                for (auto& other : allNodes) {
                    if (other.get() != this) { // Avoid self-interaction
                        InteractWith(*other);
                    }
                }
        
                // Apply friction to velocity
                ApplyFriction(0.05f);
        
                // Update position based on velocity
                position += velocity * deltaTime;
        
                // Constrain to bounds
                ConstrainToBounds(bounds);
            }
        }

        // Render the node
        void Render() override {
            if (visible) {
                position.DrawCircle(radius, color);
                if (content) content();
            }
        }
    };
}