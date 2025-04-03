#pragma once

#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/interfaces.hpp"

namespace raywtk {
    class InputBox : public IScreenElement
    {
    public:
        raylib::Rectangle rect; 
        char text[128];
        int spaceLimit;
        int fontSize;
        bool done;
        raylib::Color textColor;
        raylib::Color bgColor;
        raylib::Color borderColor;
        bool focused;

        InputBox(raylib::Rectangle rect, int fontSize, raylib::Color textColor, raylib::Color bgColor, raylib::Color borderColor, int spaceLimit, bool done);

        void Update(float dt) override;
        void Render() override;
        const char* GetText() const;
    };
}