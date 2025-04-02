#pragma once

#include <memory>
#include <vector>
#include <set>
#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "widget_toolkit/graph_widgets/node.hpp"
#include "widget_toolkit/graph_widgets/edge.hpp"
#include "widget_toolkit/display_frame/display_frame.hpp"
#include "main_app/themes/dark_simple/text_button.hpp"
#include "./page.hpp"

namespace ds_viz::pages 
{
    class GraphVisualizer : public Page
    {
        // font and title
        std::unique_ptr<raylib::Font> font;
        raylib::Text title;
        
        // Display frame
        std::unique_ptr<raywtk::DisplayFrame> displayFrame;

        // Insert new node
        std::unique_ptr<raywtk::Button> insertNodeButton;

        // Show input bar
        bool showInputBar;
        raylib::Rectangle inputBoxRect;
        char inputBoxText[128];

        // set store node value
        std::set<int> setValue;

        // vector store nodes
        std::vector<std::unique_ptr<raywtk::NodeWidget>> nodes;


        public:
            GraphVisualizer();
            void InsertNewNode(); // insert new node
            void Update(float dt) override;
            void Render() override;
    };
}