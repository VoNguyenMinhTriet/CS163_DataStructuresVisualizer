#pragma once

#include "main_app/main_window.hpp"
#include "widget_toolkit/controls/heap_button.hpp"
#include "widget_toolkit/controls/heap_text_button.hpp"
#include "widget_toolkit/interfaces.hpp"
#include "widget_toolkit/theme_manager.hpp"
#include <memory>

namespace ds_viz::themes::dark_simple
{
    static raywtk::Theme *HeapDarkSimpleTheme()
    {
        static bool _inited = false;
        static raywtk::Theme _darkSimpleTheme;

        if (!_inited)
        {
            _darkSimpleTheme.styles
                .insert({
                    typeid(raywtk::HeapButton).name(),
                    std::shared_ptr<raywtk::IStyle>(new HeapButtonStyle())
                });

        }

        return &_darkSimpleTheme;
    }
}
