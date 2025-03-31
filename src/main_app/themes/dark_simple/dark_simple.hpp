#pragma once

#include "main_app/main_window.hpp"
#include "main_app/themes/dark_simple/main_window.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "widget_toolkit/interfaces.hpp"
#include "widget_toolkit/theme_manager.hpp"
#include "./text_button.hpp"
#include <memory>

namespace ds_viz::themes::dark_simple
{
    static raywtk::Theme *DarkSimpleTheme ()
    {
        static bool _inited = false;
        static raywtk::Theme _darkSimpleTheme;

        if (!_inited)
        {
            _darkSimpleTheme.styles
                .insert({
                    typeid(ds_viz::MainWindow).name(),
                    std::shared_ptr<raywtk::IStyle>(new MainWindowStyle())
                });
            _darkSimpleTheme.styles
                .insert({
                    typeid(raywtk::Button).name(),
                    std::shared_ptr<raywtk::IStyle>(new ButtonStyle())
                });
        }

        return &_darkSimpleTheme;
    }
}
