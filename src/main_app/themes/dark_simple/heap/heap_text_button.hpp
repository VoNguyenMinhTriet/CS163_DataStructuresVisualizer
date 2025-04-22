#pragma once

#include "raylib-cpp/Color.hpp"
#include "widget_toolkit/controls/heap_button.hpp"
#include "widget_toolkit/interfaces.hpp"
#include "raylib-cpp/raylib-cpp.hpp"
#include <cassert>

namespace ds_viz::themes::dark_simple
{
    class HeapButtonStyle : public raywtk::IStyle
    {
    public:
        inline static raylib::Font font;

        // Constructor
        HeapButtonStyle() 
        {
            font = raylib::Font("./ttf/Inter-Regular.ttf", 18);
        }

        // Destructor
        ~HeapButtonStyle()
        {
            font.Unload();
        }    

        void RenderWithStyle (raywtk::IRenderable *self) override 
        {
            raywtk::HeapButton *buttonSelf = dynamic_cast<raywtk::HeapButton *>(self);
            raylib::Color color = raylib::Color::Gray();

            if (buttonSelf->state != raywtk::HeapButtonClass::Unenabled)
                color = (buttonSelf->state == raywtk::HeapButtonClass::Hover) ? raylib::Color::Yellow() : raylib::Color::Pink();
            
            buttonSelf->buttonRect.DrawRounded(0.5, 8, color);
            
            int fontSize = 18;
            raylib::Text text = raylib::Text(buttonSelf->buttonText.c_str(), fontSize, raylib::Color::Black(), font, 0);
            
            int textSizeX = MeasureTextEx(text.GetFont(), buttonSelf->buttonText.c_str(), fontSize, 0).x;
            float textX = buttonSelf->buttonRect.x + (buttonSelf->buttonRect.width - textSizeX) / 2.0;
            float textY = buttonSelf->buttonRect.y + (buttonSelf->buttonRect.height - fontSize) / 2.0;

            text.Draw(textX, textY);
        }
    };
}
