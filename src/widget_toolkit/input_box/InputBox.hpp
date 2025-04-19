#pragma once

#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/interfaces.hpp"
#include <algorithm>

namespace raywtk {
    class InputBox : public IScreenElement
    {
    public:
        raylib::Rectangle rect;
        char text[128];
        int number; // number of numbers to extract
        bool processing;
        bool decimalFlag = false; // Flag to indicate if decimal numbers are allowed
        std::vector<float> values;
        raylib::Color textColor;
        raylib::Color bgColor;
        raylib::Color borderColor;

        InputBox(raylib::Rectangle rect, raylib::Color textColor, raylib::Color bgColor, raylib::Color borderColor, int number, bool processing, bool decimalFlag = false);
        
        void Reset();
        std::vector<float> GetValues();
        void Update(float dt) override;
        void Render() override;
        const char* GetText() const;
    };
}