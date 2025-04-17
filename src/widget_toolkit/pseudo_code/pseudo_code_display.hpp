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
            
            // display is visible or not
            bool visible = true;

            // Constructor
            PseudoCodeDisplay(raylib::Vector2 position, int numLines, float lineWidth, float lineHeight, 
                raylib::Color defaultColor, raylib::Color highlightColor, raylib::Color executedColor) :
                position(position), numLines(numLines), lineWidth(lineWidth), lineHeight(lineHeight), defaultColor(defaultColor), highlightColor(highlightColor), executedColor(executedColor) 
            {
                pseudoCodeLines.resize(numLines, "");
                lineStates.resize(numLines, LineState::DEFAULT);
            }
            
            void SetPseudoCodeLines(const std::vector<std::string>& lines) 
            {
                for (size_t i = 0; i < lines.size() && i < pseudoCodeLines.size(); ++i) 
                {
                    pseudoCodeLines[i] = lines[i];
                }
            }
            
            // Set the state of a specific line
            void SetLineState(int lineIndex, LineState state)
            {
                if (lineIndex >= 0 && lineIndex < lineStates.size()) 
                {
                    lineStates[lineIndex] = state;
                }
            }
            
            // Render the pseudo-code window
            void Render() override {
                if(!visible)
                {
                    return;
                }

                for (int i = 0; i < numLines; ++i) 
                {
                    raylib::Color lineColor = GetColorForState(lineStates[i]);
                    raylib::Rectangle lineRect(position.x, position.y + i * lineHeight, lineWidth, lineHeight);
                    DrawRectangleRec(lineRect, lineColor);
                    raylib::DrawText(pseudoCodeLines[i].c_str(), position.x + 10, position.y + i * lineHeight + 10, 20, raylib::Color::Black());
                }
            }
            
            void Update(float dt) override 
            {  
            }

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