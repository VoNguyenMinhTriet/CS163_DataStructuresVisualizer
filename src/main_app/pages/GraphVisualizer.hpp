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

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;

const int INPUT_BOX_WIDTH = 100;
const int INPUT_BOX_HEIGHT = 200;

const int INSERT_NODE_BUTTON_POSX = 100;
const int INSERT_NODE_BUTTON_POSY = 200;

const int INSERT_EDGE_BUTTON_POSX = 100;
const int INSERT_EDGE_BUTTON_POSY = 450;

const int INPUT_BOX_INSERT_EDGE_POSX = INSERT_EDGE_BUTTON_POSX + BUTTON_WIDTH;
const int INPUT_BOX_INSERT_EDGE_POSY = INSERT_EDGE_BUTTON_POSY;


const int KRUSKAL_BUTTON_POSX = 100;
const int KRUSKAL_BUTTON_POSY = 700;

const int DELETE_NODE_BUTTON_POSX = 100;
const int DELETE_NODE_BUTTON_POSY = 950;

const int DELETE_EDGE_BUTTON_POSX = 100;
const int DELETE_EDGE_BUTTON_POSY = 1200;

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

        // Input box for insert new edge
        std::unique_ptr<raywtk::InputBox> inputBoxInsertEdge;

        // Input insert edge button flag
        bool inputInsertEdgeButtonFlag;

        // Input insert edge string
        char textInputInsertEdge[128];

        // Kruskal
        std::unique_ptr<raywtk::Button> KruskalButton;

        // Delete node button
        std::unique_ptr<raywtk::Button> deleteNodeButton;

        // Input delete node string
        char textInputDeleteNode[128];

        // Input delete node button flag
        bool inputDeleteNodeButtonFlag;

        // Delete edge button
        std::unique_ptr<raywtk::Button> deleteEdgeButton;

        // Input delete edge string
        char textInputDeleteEdge[128];

        // Input delete edge button flag
        bool inputDeleteEdgeButtonFlag;

        // Kruskal index processing
        int indexProcessing;
        
        // Kruskal processing flag
        bool kruskalFlag;

        // Kruskal animation control
        int animationStep;
        float animationTimer;
        const float animationDelay = 1.0f;
        
        // par and sz for kruskal
        std::vector<int> par, sz;
        
        // in-mst edges list
        std::set<int> inMstList;

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
            void Kruskal();
            void DeleteNode(int node); // delete node
            void DeleteEdge(int u, int v); // delete edge
            void Update(float dt) override;
            void Render() override;
    };
}