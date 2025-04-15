#pragma once

#include "raylib-cpp/Color.hpp"
#include "raylib-cpp/Font.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "widget_toolkit/interfaces.hpp"
#include <memory>

namespace ds_viz::themes::dark_simple
{
    class ButtonStyle : public raywtk::IStyle
    {
        std::shared_ptr<raylib::Font> textFont;

        public:

        ButtonStyle() : textFont(std::make_shared<raylib::Font>(GetFontDefault())) {}

        ButtonStyle(std::shared_ptr<raylib::Font> font) : textFont(font) {}

        void RenderWithStyle (raywtk::IRenderable *self) override
        {
            raywtk::Button *buttonSelf = dynamic_cast<raywtk::Button *>(self);
            
            if (buttonSelf->state == raywtk::ButtonClass::Hover)
                buttonSelf->buttonRect.DrawRounded(0.5, 8, raylib::Color::Yellow());
            else
                buttonSelf->buttonRect.DrawRounded(0.5, 8, raylib::Color::Pink());
            
            int fontSize = 30;
            float spacing = 1.0f;
            raylib::Vector2 textSize = textFont->MeasureText(buttonSelf->buttonText, fontSize, spacing);
            float textX = buttonSelf->buttonRect.x + (buttonSelf->buttonRect.width - textSize.x) / 2;
            float textY = buttonSelf->buttonRect.y + (buttonSelf->buttonRect.height - textSize.y) / 2;

            textFont->DrawText(buttonSelf->buttonText, raylib::Vector2(textX, textY), fontSize, spacing, raylib::Color::Black());
        }
    };
}
