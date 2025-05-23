#pragma once

#include "main_app/main_window.hpp"
#include "main_app/themes/dark_simple/main_window.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "widget_toolkit/controls/code_box.hpp"
#include "widget_toolkit/interfaces.hpp"
#include "widget_toolkit/theme_manager.hpp"
#include "./text_button.hpp"
#include "./text_box.hpp"
#include "./code_box.hpp"
#include "./graph_text_button.hpp"

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
                    std::unique_ptr<raywtk::IStyle>(new MainWindowStyle())
                });
            _darkSimpleTheme.styles
                .insert({
                    typeid(raywtk::Button).name(),
                    std::unique_ptr<raywtk::IStyle>(new ButtonStyle())
                });
            _darkSimpleTheme.styles
                .insert({
                    typeid(raywtk::Button).name(),
                    std::unique_ptr<raywtk::IStyle>(new GraphButtonStyle())
                });
          
            _darkSimpleTheme.styles
                .insert({
                    typeid(raywtk::TextBox).name(),
                    std::unique_ptr<raywtk::IStyle>(new TextBoxStyle())
                });
            _darkSimpleTheme.styles
                .insert({
                    typeid(raywtk::CodeBox).name(),
                    std::unique_ptr<raywtk::IStyle>(new CodeBoxStyle())
                });
        }

        return &_darkSimpleTheme;
    }
}
