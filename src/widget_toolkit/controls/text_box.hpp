#pragma once

#include "eventpp/callbacklist.h"

#include "../interfaces.hpp"
#include "../theme_manager.hpp"
#include "raylib-cpp/raylib-cpp.hpp"
#include <raylib.h>
#include <algorithm>
#include <memory>
#include <string>

namespace raywtk
{

enum class TextBoxClass
{
    Inactive = 0,
    Normal = 1,
    Hover = 2,
    Active = 3
};

class TextBox : public IScreenElement
{
    size_t caretPosition = 0;

    inline size_t GetLastWordBegin() {
        size_t wordEnd = text.find_last_not_of(" ", caretPosition);
        size_t lastSpace = (caretPosition == std::string::npos) ? std::string::npos : text.find_last_of(" ", wordEnd);
        if (lastSpace == std::string::npos) return 0;
        return lastSpace+1;
    }

    inline size_t GetNextWordEnd() {
        size_t wordBegin = text.find_first_not_of(" ", caretPosition);
        size_t nextSpace = (caretPosition == std::string::npos) ? std::string::npos : text.find_first_of(" ", wordBegin);
        if (nextSpace == std::string::npos) return text.size();
        return nextSpace;
    }

    void HandleCaretMovement()
    {
        if (caretPosition > text.size())
            caretPosition = text.size();

        if (raylib::Keyboard::IsKeyPressed(KEY_BACKSPACE) || raylib::Keyboard::IsKeyPressedRepeat(KEY_BACKSPACE))
        {
            if (!readOnly && !text.empty() && caretPosition)
            {
                if (raylib::Keyboard::IsKeyDown(KEY_LEFT_CONTROL) || raylib::Keyboard::IsKeyDown(KEY_RIGHT_CONTROL)) 
                {
                    // Ctrl+Bksp to backspace word
                    size_t lastWordBegin = GetLastWordBegin();
                    text.erase(lastWordBegin, caretPosition - lastWordBegin);
                    caretPosition = lastWordBegin;
                }
                else
                {
                    text.erase(caretPosition - 1, 1);
                    caretPosition--;
                }
            }
        }
        else if (raylib::Keyboard::IsKeyPressed(KEY_DELETE) || raylib::Keyboard::IsKeyPressedRepeat(KEY_DELETE))
        {
            if (!readOnly && !text.empty() && caretPosition != text.size())
            {
                if (raylib::Keyboard::IsKeyDown(KEY_LEFT_CONTROL) || raylib::Keyboard::IsKeyDown(KEY_RIGHT_CONTROL)) //
                {
                    size_t nextWordEnd = GetNextWordEnd();
                    text.erase(caretPosition, nextWordEnd - caretPosition);
                }
                else
                {
                    if (caretPosition > 0) caretPosition--;
                    text.erase(caretPosition, 1);
                }
            }
        }
        else if (raylib::Keyboard::IsKeyPressed(KEY_LEFT))
        {
            if (caretPosition > 0)
            {
                if (raylib::Keyboard::IsKeyDown(KEY_LEFT_CONTROL) || raylib::Keyboard::IsKeyDown(KEY_RIGHT_CONTROL)) 
                {
                    // Ctrl+Left to move caret to the beginning of the previous word
                    caretPosition = GetLastWordBegin();
                }
                else
                {
                    caretPosition--;
                }
            }
        }
        else if (raylib::Keyboard::IsKeyPressed(KEY_RIGHT))
        {
            if (caretPosition < text.size())
            {
                if (raylib::Keyboard::IsKeyDown(KEY_LEFT_CONTROL) || raylib::Keyboard::IsKeyDown(KEY_RIGHT_CONTROL)) 
                {
                    // Ctrl+Left to move caret to the beginning of the previous word
                    caretPosition = GetNextWordEnd();
                }
                else
                {
                    caretPosition++;
                }
            }
        }
        else if (raylib::Keyboard::IsKeyPressed(KEY_HOME))
        {
            caretPosition = 0;
        }
        else if (raylib::Keyboard::IsKeyPressed(KEY_END))
        {
            caretPosition = text.size();
        }
    
    }

    void HandleTextInput()
    {
        if (!readOnly)
        {
            char key = raylib::Keyboard::GetCharPressed();
            if (key > 0 && key < 256)
            {
                text.insert(caretPosition, 1, key);
                caretPosition++;
                //TextChanged();
            }
        }
    }

    void HandleMouseInput()
    {
        state = TextBoxClass::Normal;

        if (!enabled)
        {
            state = TextBoxClass::Inactive;
            return;
        }

        if (textBoxRect.CheckCollision(raylib::Mouse::GetPosition()))
        {
            state = TextBoxClass::Hover;

            if (raylib::Mouse::IsButtonDown(MouseButton::MOUSE_BUTTON_LEFT)) {
                focused = true;
                caretPosition = text.size();
            }
        } else if (raylib::Mouse::IsButtonDown(MouseButton::MOUSE_BUTTON_LEFT)) {
            focused = false;
            caretPosition = text.size();
        }
    }

  public:
    TextBoxClass state = TextBoxClass::Normal;
    raylib::Rectangle textBoxRect;
    std::string text;
    bool enabled = true;
    bool readOnly = false;
    bool focused = false;

    // Events
    //eventpp::CallbackList<void()> TextSubmitted;

    // Styling
    std::unique_ptr<IStyle> style;

    size_t GetCaretPosition()
    {
        return caretPosition;
    }

    void Update(float deltaTime) override
    {
        // Update control
        //--------------------------------------------------------------------
        // WARNING: Text editing is only supported under certain conditions:
        
        HandleMouseInput();
        if (focused) 
        {
            HandleTextInput();
            HandleCaretMovement();
            state = TextBoxClass::Active;
        }
    }

    void Render() override
    {
        if (style)
            style->RenderWithStyle(this);
        else
            ThemeManager::GetCurrentTheme().GetStyle(typeid(TextBox).name()).RenderWithStyle(this);
    }
};

} // namespace raywtk
