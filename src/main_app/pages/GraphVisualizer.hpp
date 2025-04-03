#pragma once

#include <memory>
#include <vector>
#include <set>
#include <map>
#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "widget_toolkit/graph_widgets/node.hpp"
#include "widget_toolkit/graph_widgets/edge.hpp"
#include "InputBox.hpp"
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

        // Insert new edge
        std::unique_ptr<raywtk::Button> insertEdgeButton;

        // Input insert edge button flag
        bool inputInsertEdgeButtonFlag;

        // Input insert edge string
        char textInputInsertEdge[128];

        // Kruskal
        std::unique_ptr<raywtk::Button> KruskalButton;

        // set store node value
        std::set<int> setValue;

        // vector store nodes
        std::vector<std::unique_ptr<raywtk::NodeWidget>> nodes;

        // vector store edges
        std::vector<std::pair<std::pair<int, int>, int>> edges;


        public:
            GraphVisualizer();
            void InsertNewNode(); // insert new node
            void InsertNewEdge(int u, int v, int c); // insert new edge
            void Update(float dt) override;
            void Render() override;
    };
}