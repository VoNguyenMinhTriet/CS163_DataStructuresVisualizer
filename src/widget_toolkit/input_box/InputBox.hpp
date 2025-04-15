#pragma once
 
#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/interfaces.hpp"
 
 namespace raywtk {
    class InputBox : public IScreenElement
    {
    public:
        raylib::Rectangle rect; 
        char text[128];
        int number; // number of numbers to extract
        bool processing;
        std::vector<int> values;
        raylib::Color textColor;
        raylib::Color bgColor;
        raylib::Color borderColor;

        InputBox(raylib::Rectangle rect, raylib::Color textColor, raylib::Color bgColor, raylib::Color borderColor, int number, bool processing);
        
        void Reset();
        std::vector<int> GetValues();
        void Update(float dt) override;
        void Render() override;
        const char* GetText() const;
    };
 }