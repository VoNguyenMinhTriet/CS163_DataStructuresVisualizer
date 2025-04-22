#pragma once

#include "raylib-cpp/Color.hpp"
#include "raylib-cpp/Font.hpp"
#include "widget_toolkit/controls/graph_button.hpp"
#include "widget_toolkit/interfaces.hpp"
#include <memory>

namespace ds_viz::themes::dark_simple
{
    class GraphButtonStyle : public raywtk::IStyle
    {
        std::shared_ptr<raylib::Font> textFont;

        public:

        GraphButtonStyle(){ textFont = std::make_shared<raylib::Font>("./ttf/Inter-Regular.ttf", 18); }

        GraphButtonStyle(std::shared_ptr<raylib::Font> font) : textFont(font) {}

        ~GraphButtonStyle()
        {
            textFont->Unload(); // Unload font to free resources
        }   

        void RenderWithStyle (raywtk::IRenderable *self) override
        {
            raywtk::GraphButton *buttonSelf = dynamic_cast<raywtk::GraphButton *>(self);
            
            if (buttonSelf->state == raywtk::GraphButtonClass::Hover)
                buttonSelf->buttonRect.DrawRounded(0.5, 8, raylib::Color::Yellow());
            else
                buttonSelf->buttonRect.DrawRounded(0.5, 8, raylib::Color::Pink());
            
            int fontSize = 18;
            float spacing = 1.0f;
            raylib::Vector2 textSize = textFont->MeasureText(buttonSelf->buttonText, fontSize, spacing);
            float textX = buttonSelf->buttonRect.x + (buttonSelf->buttonRect.width - textSize.x) / 2;
            float textY = buttonSelf->buttonRect.y + (buttonSelf->buttonRect.height - textSize.y) / 2;

            textFont->DrawText(buttonSelf->buttonText, raylib::Vector2(textX, textY), fontSize, spacing, raylib::Color::Black());
        }
    };
}