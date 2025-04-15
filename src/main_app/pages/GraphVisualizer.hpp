#pragma once

#include <memory>
#include <vector>
#include <set>
#include <map>
#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "widget_toolkit/graph_widgets/node.hpp"
#include "widget_toolkit/graph_widgets/edge.hpp"
#include "widget_toolkit/display_frame/display_frame.hpp"
#include "widget_toolkit/input_box/InputBox.hpp"
#include "main_app/themes/dark_simple/text_button.hpp"
#include "./page.hpp"
#include "widget_toolkit/notification/Notification.hpp"
#include "widget_toolkit/pseudo_code_display/PseudoCode.hpp"

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;

const int INPUT_BOX_WIDTH = 100;
const int INPUT_BOX_HEIGHT = 200;

const int INSERT_NODE_BUTTON_POSX = 100;
const int INSERT_NODE_BUTTON_POSY = 200;

const int INSERT_EDGE_BUTTON_POSX = 100;
const int INSERT_EDGE_BUTTON_POSY = 450;

const int KRUSKAL_BUTTON_POSX = 100;
const int KRUSKAL_BUTTON_POSY = 700;

const int DELETE_NODE_BUTTON_POSX = 100;
const int DELETE_NODE_BUTTON_POSY = 950;

const int DELETE_EDGE_BUTTON_POSX = 100;
const int DELETE_EDGE_BUTTON_POSY = 1200;

const int INPUT_BOX_INSERT_EDGE_POSX = INSERT_EDGE_BUTTON_POSX + BUTTON_WIDTH;
const int INPUT_BOX_INSERT_EDGE_POSY = INSERT_EDGE_BUTTON_POSY;

const int INPUT_BOX_DELETE_NODE_POSX = DELETE_NODE_BUTTON_POSX + BUTTON_WIDTH;
const int INPUT_BOX_DELETE_NODE_POSY = DELETE_NODE_BUTTON_POSY;

const int INPUT_BOX_DELETE_EDGE_POSX = DELETE_EDGE_BUTTON_POSX + BUTTON_WIDTH;
const int INPUT_BOX_DELETE_EDGE_POSY = DELETE_EDGE_BUTTON_POSY;

const int NOTIFICATION_COORDX = NOTIFICATION_FRAME_COORDX + 20;
const int NOTIFICATION_COORDY = NOTIFICATION_FRAME_COORDY + 20;

const int PSEUDOCODE_LINE_WIDTH = 700;
const int PSEUDOCODE_LINE_HEIGHT = 50;

namespace ds_viz::pages 
{
    class GraphVisualizer : public Page
    {
        // font and title
        std::unique_ptr<raylib::Font> font;
        raylib::Text title;
        
        // Display frame
        std::unique_ptr<raywtk::DisplayFrame> workingFrame;
        // Notification frame
        std::unique_ptr<raywtk::DisplayFrame> notificationFrame;
        // Insert new node
        std::unique_ptr<raywtk::Button> insertNodeButton;
        // Insert new edge
        std::unique_ptr<raywtk::Button> insertEdgeButton;
        // Kruskal
        std::unique_ptr<raywtk::Button> KruskalButton;
        // Delete node button
        std::unique_ptr<raywtk::Button> deleteNodeButton;
        // Delete edge button
        std::unique_ptr<raywtk::Button> deleteEdgeButton;

        // Notification
        std::unique_ptr<raywtk::Notification> currentNotification;

        // Input box for insert new edge
        std::unique_ptr<raywtk::InputBox> inputBoxInsertEdge;
        bool inputInsertEdgeButtonFlag;
        // Input box for delete node
        std::unique_ptr<raywtk::InputBox> inputBoxDeleteNode;
        bool inputDeleteNodeButtonFlag;
        // Input box for delete edge
        std::unique_ptr<raywtk::InputBox> inputBoxDeleteEdge;
        bool inputDeleteEdgeButtonFlag;

        // Notification
        std::unique_ptr<raywtk::Notification> notification = nullptr;

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
        
        std::vector<std::string> kruskalPseudoCode;
        std::unique_ptr<raywtk::PseudoCodeDisplay> pseudoCodeDisplay;


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