#pragma once
 
#include <memory>
#include <vector>
#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "widget_toolkit/tree_widgets/node.hpp"
#include "widget_toolkit/tree_widgets/edge.hpp"
#include "widget_toolkit/display_frame/display_frame.hpp"
#include "main_app/themes/dark_simple/text_button.hpp"
#include "InputBox.hpp"
#include "./page.hpp"


namespace ds_viz::pages
{
    class HeapVisualizer : public Page
    {
        // font and title
        std::unique_ptr<raylib::Font> font;
        raylib::Text title;

        // Display frame
        std::unique_ptr<raywtk::DisplayFrame> displayFrame;
 
        // Push new value
        std::unique_ptr<raywtk::Button> pushValueButton;
 
        // Pop max value
        std::unique_ptr<raywtk::Button> popMaxValueButton;

        // Input box for push new value
        std::unique_ptr<raywtk::InputBox> inputBoxPushNewValue;

        // Input new value button flag
        bool inputPushNewValueButtonFlag;

        // Input new value string
        char textInputPushNewValue[128];

        // vector store node to perform heap operator
        std::vector<int> values;

        // vector store nodes
        std::vector<std::unique_ptr<raywtk::NodeWidget>> nodes;

        public:
            HeapVisualizer();
            int parent(int i); // parent of node with index i on heap
            int left_child(int i); // left child of node with index i on heap
            int right_child(int i); // right child of node with index i on heap
            void swapNodes(int i, int j); // swap two nodes i and j on heap
            void PopMaxValue(); // pop the max value out of heap (root of heap)
            void PushNewValue(int value); // push 'value' into heap
            void Update(float dt) override;
            void Render() override;
    };
}