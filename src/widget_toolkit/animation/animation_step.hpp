#pragma once

#include "raylib-cpp/raylib-cpp.hpp"
#include "../interfaces.hpp"

namespace raywtk 
{
    enum class StepType 
    {
        Swap2Nodes = 0,
        Highlight2Nodes = 1,
        Highlight1Node = 2,
        InsertNode = 3,
        DeleteNode = 4,
        SetNewPseudoCodeLines = 5,
        None = 6
    };

    struct Step {
        StepType type;
        int u, v, idPseudoCodeLine;
        string pseudoCodeProcessText;
        std::unique_ptr<raywtk::HeapNodeWidget> node;
        std::vector<std::string> pseudoCodeLines;
        std::vector<std::string> lastPseudoCodeLines;

        Step() : type(StepType::None), u(-1), v(-1), idPseudoCodeLine(-1), pseudoCodeProcessText(""), pseudoCodeLines(vector<string>()), lastPseudoCodeLines(vector<string>()),  node(nullptr) {};
        Step(StepType type, int u, int v, int idPseudoCodeLine, string pseudoCodeProcessText) : type(type), u(u), v(v), idPseudoCodeLine(idPseudoCodeLine), pseudoCodeProcessText(pseudoCodeProcessText), pseudoCodeLines(vector<string>()), lastPseudoCodeLines(vector<string>()), node(nullptr) {};
        Step(StepType type, int u, int v, int idPseudoCodeLine, string pseudoCodeProcessText, std::unique_ptr<raywtk::HeapNodeWidget> node) : type(type), u(u), v(v), idPseudoCodeLine(idPseudoCodeLine), pseudoCodeProcessText(pseudoCodeProcessText), pseudoCodeLines(vector<string>()), lastPseudoCodeLines(vector<string>()), node(std::move(node)) {};
        Step(StepType type, int u, int v, int idPseudoCodeLine, string pseudoCodeProcessText, std::vector<std::string> pseudoCodeLines, std::vector<std::string> lastPseudoCodeLines, std::unique_ptr<raywtk::HeapNodeWidget> node) : type(type), u(u), v(v), idPseudoCodeLine(idPseudoCodeLine), pseudoCodeProcessText(pseudoCodeProcessText), pseudoCodeLines(pseudoCodeLines), lastPseudoCodeLines(lastPseudoCodeLines), node(std::move(node)) {};
    };
}