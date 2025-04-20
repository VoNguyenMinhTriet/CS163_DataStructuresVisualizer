#pragma once

#include "widget_toolkit/interfaces.hpp"
#include "raylib-cpp/raylib-cpp.hpp"
#include <vector>
#include <string>
#include <memory>
#include <cassert>

const std::vector<std::string> BUILD_HEAP_PSEUDO_CODE = {
    "Clear heap.",
    "For each value in values:",
    "    Add value to the end of heap",
    "k <- Parent(index of last element)",
    "For i = k down to 0:",
    "    MaxHeapify(i)",
};

const std::vector<std::string> PUSH_NEW_VALUE_PSEUDO_CODE = {
    "Add value to the end of heap",
    "i <- index of last element",
    "While i > 0 and heap[i] > heap[Parent(i)]:",
    "    Swap heap[i] and heap[Parent(i)]",
    "    i <- Parent(i)",
};

const std::vector<std::string> POP_MAX_VALUE_PSEUDO_CODE = {
    "Swap root (max value) with last element",
    "Remove last element from heap",
    "MaxHeapify(0)",
};

const std::vector<std::string> MAX_HEAPIFY_PSEUDO_CODE = {
    "k <- LeftChild(i)",
    "R <- RightChild(i)",
    "If R < heap size and heap[k] > heap[R]:",
    "    k = R",
    "If k < heap size and heap[i] < heap[k]:",
    "    Swap heap[i] and heap[k]",
    "    MaxHeapify(k)",
};

namespace raywtk
{
    
    // Class for displaying pseudo-code
    class HeapPseudoCodeDisplay : public IScreenElement {
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
            HeapPseudoCodeDisplay(raylib::Vector2 position, int numLines, float lineWidth, float lineHeight,
                raylib::Color defaultColor, raylib::Color highlightColor, raylib::Color executedColor)
                : position(position), numLines(numLines), lineWidth(lineWidth), lineHeight(lineHeight),
                defaultColor(defaultColor), highlightColor(highlightColor), executedColor(executedColor),
                font("./ttf/Inter-Regular.ttf", 18) // Load font during initialization
            {
                pseudoCodeLines.resize(numLines, "");
                lineStates.resize(numLines, LineState::DEFAULT);
            }

            // Destructor
            ~HeapPseudoCodeDisplay() {
                font.Unload(); // Unload font to free resources
            }
            
            // set the state for all lines of pseudo code
            void SetPseudoCodeLines(const std::vector<std::string>& lines) 
            {
                numLines = lines.size();
                assert(pseudoCodeLines.size() >= lines.size());
                for (size_t i = 0; i < numLines; ++i) 
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

            // Swap the state of a specific line (highlighted -> default, default -> highlighted)
            void SwapLineState(int lineIndex) 
            {
                if(lineIndex >= 0 && lineIndex < lineStates.size()) 
                {
                    if (lineStates[lineIndex] == LineState::HIGHLIGHTED) 
                    {
                        lineStates[lineIndex] = LineState::DEFAULT;
                    } 
                    else 
                    {
                        lineStates[lineIndex] = LineState::HIGHLIGHTED;
                    }
                }
            }
            
            // Render the pseudo-code window
            void Render() override {
                if(!visible)
                {
                    return;
                }
                
                int fontSize = 18;
                for (int i = 0; i < numLines; ++i) 
                {
                    raylib::Color lineColor = GetColorForState(lineStates[i]);
                    raylib::Rectangle lineRect(position.x, position.y + i * lineHeight, lineWidth, lineHeight);
                    raylib::DrawTextEx(font, pseudoCodeLines[i].c_str(), raylib::Vector2(position.x + 5, position.y + i * lineHeight + 2), fontSize, 0, lineColor);
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
        raylib::Font font;
        
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