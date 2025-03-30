#pragma once

#include "raylib-cpp/Color.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "widget_toolkit/interfaces.hpp"

namespace ds_viz::themes::dark_simple
{
    class ButtonStyle : public raywtk::IStyle
    {
        void RenderWithStyle (raywtk::IRenderable *self)
        {
            raywtk::Button *buttonSelf = dynamic_cast<raywtk::Button *>(self);

            if (buttonSelf->state == raywtk::ButtonClass::Hover)
                buttonSelf->buttonRect.DrawRounded(0.5, 8, raylib::Color::Yellow());
            else
                buttonSelf->buttonRect.DrawRounded(0.5, 8, raylib::Color::Pink());
            
            int fontSize = 20;
            raylib::Vector2 textSize = raylib::MeasureText(buttonSelf->buttonText.c_str(), fontSize);
            float textX = buttonSelf->buttonRect.x + (buttonSelf->buttonRect.width - textSize.x) / 2;
            float textY = buttonSelf->buttonRect.y + (buttonSelf->buttonRect.height - textSize.y) / 2;
            DrawText(buttonSelf->buttonText.c_str(), (int)textX, (int)textY, fontSize, RAYWHITE);
        }
    };
}
