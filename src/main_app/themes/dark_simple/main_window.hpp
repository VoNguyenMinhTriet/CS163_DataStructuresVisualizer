#pragma once
#include "main_app/main_window.hpp"
#include "widget_toolkit/interfaces.hpp"

namespace ds_viz::themes::dark_simple
{
    class MainWindowStyle : public raywtk::IStyle
    {
        void RenderWithStyle (raywtk::IRenderable *self)
        {
            ds_viz::MainWindow *mainWinSelf = dynamic_cast<ds_viz::MainWindow *>(self);

            mainWinSelf->ref_raylib_window->ClearBackground();
        }
    };
}
