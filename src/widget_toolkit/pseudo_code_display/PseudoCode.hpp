#pragma once

#include "widget_toolkit/interfaces.hpp"
#include "raylib-cpp/raylib-cpp.hpp"
#include <vector>
#include <string>
#include <memory>

namespace raywtk
{

    // Class for displaying pseudo-code
    class PseudoCodeDisplay : public IScreenElement {
    public:
        // Enum for line states
        enum class LineState {
            DEFAULT,
            HIGHLIGHTED,
            EXECUTED
        };
        // Constructor
        PseudoCodeDisplay(raylib::Vector2 position, int numLines, float lineWidth, float lineHeight, 
                        raylib::Color defaultColor, raylib::Color highlightColor, raylib::Color executedColor);

        void SetPseudoCodeLines(const std::vector<std::string>& lines);

        void SetLineState(int lineIndex, LineState state);

        // Destructor
        ~PseudoCodeDisplay() {
            font.Unload(); // Unload font to free resources
        }

        void Render() override;

        void Update(float dt) override;
    private:
        raylib::Vector2 position;
        int numLines;
        float lineWidth;
        float lineHeight;
        raylib::Color defaultColor;
        raylib::Color highlightColor;
        raylib::Color executedColor;

        std::vector<std::string> pseudoCodeLines;
        std::vector<LineState> lineStates;
        std::shared_ptr<raylib::Font> codeFont;
        inline static raylib::Font font;

        // Get the color for a specific line state
        raylib::Color GetColorForState(LineState state) const {
            switch (state) {
                case LineState::HIGHLIGHTED:
                    return highlightColor;
                case LineState::EXECUTED:
                    return executedColor;
                default:
                    return defaultColor;
            }
        }
    };
}