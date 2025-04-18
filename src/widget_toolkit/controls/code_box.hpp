#pragma once

#include <string>
#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/interfaces.hpp"
#include "widget_toolkit/theme_manager.hpp"

namespace raywtk 
{
    class CodeBox : public raywtk::IScreenElement
    {
        public:
        std::string code;
        int highlightedLine = 0;
        raywtk::IStyle *style = nullptr;

        raylib::Rectangle boxRect;

        void Update(float deltaTime) override {}

        void Render() override
        {
            if (style)
                style->RenderWithStyle(this);
            else
                ThemeManager::GetCurrentTheme().GetStyle(typeid(CodeBox).name()).RenderWithStyle(this); 
        }
    };
}