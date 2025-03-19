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
        }
    };
}
