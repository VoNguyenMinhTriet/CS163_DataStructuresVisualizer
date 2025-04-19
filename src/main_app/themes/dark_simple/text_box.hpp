#pragma once

#include "raylib-cpp/Font.hpp"
#include "raylib-cpp/Rectangle.hpp"
#include "raylib-cpp/Vector2.hpp"
#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/controls/text_box.hpp"
#include "widget_toolkit/interfaces.hpp"

#include <raylib.h>
#include <chrono>
#include <memory>

namespace ds_viz::themes::dark_simple
{
class TextBoxStyle : public raywtk::IStyle
{
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> caretBlinkCycle = std::chrono::milliseconds(500);
    std::unique_ptr<raylib::Font> font;
    int fontSize = 16;
    float borderSize = 1;
    float padding = 5;

  public:
    TextBoxStyle () {
        font = std::unique_ptr<raylib::Font>(new raylib::Font("./ttf/Inter-Regular.ttf", fontSize));
    }

    void RenderWithStyle(raywtk::IRenderable* self)
    {
        // TODO: Clip text
        
        raywtk::TextBox* textBoxSelf = dynamic_cast<raywtk::TextBox*>(self);

        // Background
        auto actualRect = raylib::Rectangle(
            textBoxSelf->textBoxRect.GetPosition() +
                raylib::Vector2 {borderSize, borderSize},
            textBoxSelf->textBoxRect.GetSize() -
                raylib::Vector2 {borderSize * 2, borderSize * 2});
        if (textBoxSelf->state == raywtk::TextBoxClass::Hover)
            actualRect.DrawRoundedLines(0.1, 32, borderSize,
                                        raylib::Color::Yellow());
        else
            actualRect.DrawRoundedLines(0.1, 32, borderSize,
                                        raylib::Color::Pink());

        // Text
        raylib::Text text(textBoxSelf->text, fontSize, raylib::Color::White(),
                          *font, 0);
        // For measuring purposes only. Probably should find another way instead
        // of creating and destroying it later.
        raylib::Text substringText(
            textBoxSelf->text.substr(0, textBoxSelf->GetCaretPosition()), fontSize,
            raylib::Color::White(), *font, 0);

        text.Draw(textBoxSelf->textBoxRect.x + padding,
                  textBoxSelf->textBoxRect.y + padding);

        // Caret
        int caretScreenXPosition =
            textBoxSelf->textBoxRect.x + padding + substringText.MeasureEx().x;
        if (textBoxSelf->state == raywtk::TextBoxClass::Active)
        {
            raylib::Rectangle caretRect(caretScreenXPosition,
                                        textBoxSelf->textBoxRect.y + padding, 2,
                                        textBoxSelf->textBoxRect.height - padding * 2);
            if (std::chrono::high_resolution_clock::now() - startTime >= caretBlinkCycle * 2)
                startTime = std::chrono::high_resolution_clock::now();

            if (std::chrono::high_resolution_clock::now() - startTime <= caretBlinkCycle)
                caretRect.Draw(raylib::Color::White());
        }
    }
};
} // namespace ds_viz::themes::dark_simple
