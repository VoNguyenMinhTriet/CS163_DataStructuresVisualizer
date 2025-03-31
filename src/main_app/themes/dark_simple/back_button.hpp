#pragma once

#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "widget_toolkit/interfaces.hpp"

namespace ds_viz::themes::dark_simple
{
    class BackButtonStyle : public raywtk::IStyle
    {
        raylib::Texture img = raylib::Texture(raylib::Image("./images/return_button.png"));

        void RenderWithStyle (raywtk::IRenderable *self) override
        {
            raywtk::Button *buttonSelf = dynamic_cast<raywtk::Button *>(self);
            
            img.Draw(raylib::Rectangle(0,0,img.GetWidth(),img.GetHeight()), buttonSelf->buttonRect);
        }
    };
}