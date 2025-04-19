#pragma once

#include "eventpp/callbacklist.h"
#include "raylib-cpp/raylib-cpp.hpp"

#include "../interfaces.hpp"
#include "../theme_manager.hpp"
#include <memory>
#include <raylib.h>

using namespace std;

namespace raywtk
{
    enum class HeapButtonClass
    {
        Inactive = 0,
        Normal = 1,
        Hover = 2,
        Pressed = 3,
        Unenabled = 4
    };

    class HeapButton : public IScreenElement
    {
    public:
        raylib::Rectangle buttonRect;
        bool enabled = true; // can be click or not
        bool showing = true; // is showing in display or not (if not, it can't be click)

        // Text for button
        string buttonText = "Button";

        // Events
        eventpp::CallbackList<void()> Click;

        // Styling
        HeapButtonClass state = HeapButtonClass::Normal;
        std::unique_ptr<IStyle> style;

        void Update(float deltaTime) override
        {
            HandleMouseInput();
        }

        void HandleMouseInput()
        {
            if (!showing) 
            {
                state = HeapButtonClass::Inactive;
                return;
            }

            if(!enabled)
            {
                state = HeapButtonClass::Unenabled;
                return;
            }

            state = HeapButtonClass::Normal;

            if (buttonRect.CheckCollision(raylib::Mouse::GetPosition()))
            {
                state = HeapButtonClass::Hover;

                if (raylib::Mouse::IsButtonPressed(MouseButton::MOUSE_BUTTON_LEFT))
                    Click();

                if (raylib::Mouse::IsButtonDown(MouseButton::MOUSE_BUTTON_LEFT))
                    state = HeapButtonClass::Pressed;
            }
        }

        void Render() override
        {
            if(!showing)
            {
                return;
            }

            if (style)
                style->RenderWithStyle(this);
            else
                ThemeManager::GetCurrentTheme().GetStyle(typeid(HeapButton).name()).RenderWithStyle(this);
        }
    };
}
