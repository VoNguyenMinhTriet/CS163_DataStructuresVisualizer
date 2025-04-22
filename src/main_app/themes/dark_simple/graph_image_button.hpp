#pragma once

#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/controls/graph_button.hpp"
#include "widget_toolkit/interfaces.hpp"

namespace ds_viz::themes::dark_simple
{
    class GraphImageButtonStyle : public raywtk::IStyle
    {
        raylib::Texture* img = nullptr;
    
    public:
        GraphImageButtonStyle(raylib::Texture* texture) : img(texture) {}
    
        void RenderWithStyle (raywtk::IRenderable *self) override
        {
            if (!img)
                return;

            raywtk::GraphButton* buttonSelf = dynamic_cast<raywtk::GraphButton*>(self);
            if (buttonSelf)
                img->Draw(raylib::Rectangle(0,0,img->GetWidth(),img->GetHeight()), buttonSelf->buttonRect);
        }
    };
}