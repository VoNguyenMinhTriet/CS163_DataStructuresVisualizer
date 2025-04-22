#include "PseudoCode.hpp"


namespace raywtk
{
    PseudoCodeDisplay::PseudoCodeDisplay(raylib::Vector2 position, int numLines, float lineWidth, float lineHeight, 
        raylib::Color defaultColor, raylib::Color highlightColor, raylib::Color executedColor) :
        position(position), numLines(numLines), lineWidth(lineWidth), lineHeight(lineHeight), defaultColor(defaultColor), highlightColor(highlightColor), executedColor(executedColor) {
            pseudoCodeLines.resize(numLines, "");
            lineStates.resize(numLines, LineState::DEFAULT);
            font = raylib::Font("./ttf/Inter-Regular.ttf", 18);
        }

    void PseudoCodeDisplay::SetPseudoCodeLines(const std::vector<std::string>& lines) {
        for (size_t i = 0; i < lines.size() && i < pseudoCodeLines.size(); ++i) {
            pseudoCodeLines[i] = lines[i];
        }
    }

    // Set the state of a specific line
    void PseudoCodeDisplay::SetLineState(int lineIndex, LineState state) {
        if (lineIndex >= 0 && lineIndex < lineStates.size()) {
            lineStates[lineIndex] = state;
        }
    }

    // Render the pseudo-code window
    void PseudoCodeDisplay::Render() {
        raylib::Rectangle lineRect(position.x, position.y, lineWidth, lineHeight * numLines);
        lineRect.DrawRoundedLines(0.1f, 13, PINK);
        // std::shared_ptr<raylib::Font> codeFont = std::make_shared<raylib::Font>("./ttf/Inter-Regular.ttf", 18);
        float fontSize = 18;
        float spacing = 1.0f;
        for (int i = 0; i < numLines; ++i) {
            raylib::Color lineColor = GetColorForState(lineStates[i]);
            font.DrawText(pseudoCodeLines[i].c_str(), raylib::Vector2(position.x + 3, position.y + i * lineHeight + 5), fontSize, spacing, lineColor);
        }
    }    

    void PseudoCodeDisplay::Update(float dt){

    }
};