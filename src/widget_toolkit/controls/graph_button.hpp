#pragma once

#include <memory>
#include "eventpp/callbacklist.h"
#include "raylib-cpp/raylib-cpp.hpp"
#include "../interfaces.hpp"
#include "../theme_manager.hpp"

namespace raywtk
{
    enum class GraphButtonClass
    {
        Inactive = 0,
        Normal = 1,
        Hover = 2,
        Pressed = 3
    };

    class GraphButton : public IScreenElement
    {
    public:
        raylib::Rectangle buttonRect;
        bool enabled = true;

        // Text for button
        std::string buttonText = "";
        
        // Events
        eventpp::CallbackList<void()> Click;

        // Styling
        GraphButtonClass state = GraphButtonClass::Normal;
        std::unique_ptr<IStyle> style;


        void Update (float deltaTime) override
        {
            HandleMouseInput();
        }

        void HandleMouseInput ()
        {
            if (!enabled) {
                state = GraphButtonClass::Inactive;
                return;
            }

            state = GraphButtonClass::Normal;

            if (buttonRect.CheckCollision(raylib::Mouse::GetPosition()))
            {
                state = GraphButtonClass::Hover;

                if (raylib::Mouse::IsButtonDown(MouseButton::MOUSE_BUTTON_LEFT))
                    state = GraphButtonClass::Pressed;

                if (raylib::Mouse::IsButtonPressed(MouseButton::MOUSE_BUTTON_LEFT))
                    Click();
            }
        }

        void Render () override
        {
            if (style)
                style->RenderWithStyle(this);
            else
                ThemeManager::GetCurrentTheme().GetStyle(typeid(GraphButton).name()).RenderWithStyle(this);
        }
    };
}