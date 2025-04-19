#pragma once

#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "widget_toolkit/interfaces.hpp"

namespace ds_viz::themes::dark_simple
{
    class ImageButtonStyle : public raywtk::IStyle
    {
        raylib::Texture* img = nullptr;
    
    public:
        ImageButtonStyle(raylib::Texture* texture) : img(texture) {}
    
        void RenderWithStyle (raywtk::IRenderable *self) override
        {
            if (!img)
                return;

            raywtk::Button* buttonSelf = dynamic_cast<raywtk::Button*>(self);
            if (buttonSelf)
                img->Draw(raylib::Rectangle(0,0,img->GetWidth(),img->GetHeight()), buttonSelf->buttonRect);
        }
    };
}