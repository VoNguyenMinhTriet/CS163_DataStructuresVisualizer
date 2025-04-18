#pragma once

#include "widget_toolkit/interfaces.hpp"
#include "widget_toolkit/theme_manager.hpp"
#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/controls/code_box.hpp"
#include <memory>
#include <sstream>
#include <string>

namespace ds_viz::themes::dark_simple
{
class CodeBoxStyle : public raywtk::IStyle
{
    std::unique_ptr<raylib::Font> font;
    int fontSize = 16;
    float borderSize = 1;
    float padding = 5;
    float lineSpacing = 0;

  public:
    CodeBoxStyle () {
        font = std::unique_ptr<raylib::Font>(new raylib::Font("./ttf/Inter-Regular.ttf", fontSize));
    }

    void RenderWithStyle(raywtk::IRenderable* self)
    {
        // TODO: Clip text
        
        raywtk::CodeBox* codeBoxSelf = dynamic_cast<raywtk::CodeBox *>(self);

        // Background
        auto actualRect = raylib::Rectangle(
            codeBoxSelf->boxRect.GetPosition() +
                raylib::Vector2 {borderSize, borderSize},
            codeBoxSelf->boxRect.GetSize() -
                raylib::Vector2 {borderSize * 2, borderSize * 2});
        
        actualRect.DrawRoundedLines(0.1, 32, borderSize,
                                    raylib::Color::Pink());

        // Text
        std::stringstream ss; ss << codeBoxSelf->code;
        std::string line; int lineCount = 1;
        while (std::getline(ss, line))
        {
            float yPos = codeBoxSelf->boxRect.y + padding + (lineCount - 1) * (fontSize + lineSpacing);
            auto color = (lineCount == codeBoxSelf->highlightedLine) ? raylib::Color::Yellow() : raylib::Color::White();
            auto lineText = raylib::Text(line, fontSize, color,
                                            *font, 0);
            lineText.Draw(codeBoxSelf->boxRect.x + padding,
                            yPos);
            
            ++lineCount;
        }
    }
};
} // namespace ds_viz::themes::dark_simple