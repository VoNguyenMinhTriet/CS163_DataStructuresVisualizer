#pragma once

#include "raylib-cpp/Color.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "widget_toolkit/interfaces.hpp"


namespace ds_viz::themes::dark_simple
{
    const std::unique_ptr<raylib::Font> font = std::make_unique<raylib::Font>("./ttf/InterDisplay-Black.ttf", 128, nullptr, 250);
    class ButtonStyle : public raywtk::IStyle
    {
        void RenderWithStyle (raywtk::IRenderable *self) override 
        {
            raywtk::Button *buttonSelf = dynamic_cast<raywtk::Button *>(self);
            
            if (buttonSelf->state == raywtk::ButtonClass::Hover)
                buttonSelf->buttonRect.DrawRounded(0.5, 8, raylib::Color::Yellow());
            else
                buttonSelf->buttonRect.DrawRounded(0.5, 8, raylib::Color::Pink());
            
            int fontSize = 18;
            raylib::Vector2 textSize = raylib::MeasureText(buttonSelf->buttonText.c_str(), fontSize);
            float textX = buttonSelf->buttonRect.x + (buttonSelf->buttonRect.width - textSize.x) / 2.0;
            float textY = buttonSelf->buttonRect.y + (buttonSelf->buttonRect.height - fontSize) / 2.0;

            raylib::Text text = raylib::Text(buttonSelf->buttonText.c_str(), fontSize, raylib::Color::Black(), *font, 1);
            text.Draw(textX, textY);
        }
    };
}
