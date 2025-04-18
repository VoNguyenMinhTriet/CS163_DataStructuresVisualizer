#pragma once

#include <memory>
#include "eventpp/callbacklist.h"
#include "../interfaces.hpp"
#include "../theme_manager.hpp"

namespace raywtk
{
    enum class ButtonClass
    {
        Inactive = 0,
        Normal = 1,
        Hover = 2,
        Pressed = 3
    };

    class Button : public IScreenElement
    {
    public:
        raylib::Rectangle buttonRect;
        bool enabled = true;

        // Text for button
        std::string buttonText = "Button";
        // Events
        eventpp::CallbackList<void()> Click;

        // Styling
        ButtonClass state = ButtonClass::Normal;
        std::unique_ptr<IStyle> style;


        void Update (float deltaTime) override
        {
            HandleMouseInput();
        }

        void HandleMouseInput ()
        {
            if (!enabled) {
                state = ButtonClass::Inactive;
                return;
            }

            state = ButtonClass::Normal;

            if (buttonRect.CheckCollision(raylib::Mouse::GetPosition()))
            {
                state = ButtonClass::Hover;

                if (raylib::Mouse::IsButtonDown(MouseButton::MOUSE_BUTTON_LEFT))
                    state = ButtonClass::Pressed;

                if (raylib::Mouse::IsButtonPressed(MouseButton::MOUSE_BUTTON_LEFT))
                    Click();
            }
        }

        void Render () override
        {
            if (style)
                style->RenderWithStyle(this);
            else
                ThemeManager::GetCurrentTheme().GetStyle(typeid(Button).name()).RenderWithStyle(this);
        }
    };
}
