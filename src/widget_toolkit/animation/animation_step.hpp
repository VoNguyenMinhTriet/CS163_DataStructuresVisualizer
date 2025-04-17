#pragma once

#include "raylib-cpp/raylib-cpp.hpp"
#include "../interfaces.hpp"

namespace raywtk 
{
    enum class StepType 
    {
        Swap2Nodes = 0,
        Highlight2Nodes = 1,
        InsertNode = 2,
        DeleteNode = 3,
        None = 4
    };

    struct Step {
        StepType type;
        int u, v;
        std::unique_ptr<raywtk::NodeWidget> node;

        Step() : type(StepType::None), u(-1), v(-1), node(nullptr) {};
        Step(StepType type, int u, int v) : type(type), u(u), v(v), node(nullptr) {};
        Step(StepType type, int u, int v, std::unique_ptr<raywtk::NodeWidget> node) : type(type), u(u), v(v), node(std::move(node)) {};
    };
}