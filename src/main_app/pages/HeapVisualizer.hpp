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

const int OPERATOR_BUTTON_WIDTH = 250;
const int OPERATOR_BUTTON_HEIGHT = 170;

const int BUILD_HEAP_BUTTON_COORDX = 75;
const int BUILD_HEAP_BUTTON_COORDY = 220;

const int PUSH_VALUE_BUTTON_COORDX = 75;
const int PUSH_VALUE_BUTTON_COORDY = 420;

const int POP_VALUE_BUTTON_COORDX = 75;
const int POP_VALUE_BUTTON_COORDY = 620;

const int CLEAR_HEAP_BUTTON_COORDX = 75;
const int CLEAR_HEAP_BUTTON_COORDY = 820;

const int INPUT_BOX_BUILD_HEAP_COORDX = 330;
const int INPUT_BOX_BUILD_HEAP_COORDY = 250;
const int INPUT_BOX_BUILD_HEAP_WIDTH = 150;
const int INPUT_BOX_BUILD_HEAP_HEIGHT = 110;

const int INPUT_BOX_PUSH_VALUE_COORDX = 330;
const int INPUT_BOX_PUSH_VALUE_COORDY = 450;
const int INPUT_BOX_PUSH_VALUE_WIDTH = 150;
const int INPUT_BOX_PUSH_VALUE_HEIGHT = 110;

#define sz(x) int((x).size())

namespace ds_viz::pages
{
    class HeapVisualizer : public Page
    {
        // font and title
        std::unique_ptr<raylib::Font> font;
        raylib::Text title;

        // Display frame
        std::unique_ptr<raywtk::DisplayFrame> displayFrame;

        // Build heap button
        std::unique_ptr<raywtk::Button> buildHeapButton;
 
        // Push new value button
        std::unique_ptr<raywtk::Button> pushValueButton;
 
        // Pop max value button
        std::unique_ptr<raywtk::Button> popMaxValueButton;

        // Clear heap button
        std::unique_ptr<raywtk::Button> clearHeapButton;

        // Input box for build heap
        std::unique_ptr<raywtk::InputBox> inputBoxBuildHeap;

        // Input box for push new value
        std::unique_ptr<raywtk::InputBox> inputBoxPushNewValue;

        // Input build heap button flag
        bool inputBuildHeapButtonFlag;

        // Input new value button flag
        bool inputPushNewValueButtonFlag;

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
            void maxHeapify(int i); // max heapify for subtree of node with index i
            raylib::Vector2 GetPositionInDisplay(int &index, int &depth); // get position for node with index 'index' on display
            void BuildHeap(const vector<int> &val); // build new heap
            void PushNewValue(int value); // push 'value' into heap
            void PopMaxValue(); // pop the max value out of heap (root of heap)
            void ClearHeap(); // clear heap
            void Update(float dt) override;
            void Render() override;
    };
}