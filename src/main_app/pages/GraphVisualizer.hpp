#pragma once

#include <memory>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <chrono>
#include <random>
#include <string>
#include <fstream>
#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "widget_toolkit/controls/graph_button.hpp"
#include "widget_toolkit/graph_widgets/node.hpp"
#include "widget_toolkit/graph_widgets/graph_node.hpp"
#include "widget_toolkit/graph_widgets/edge.hpp"
#include "widget_toolkit/display_frame/display_frame.hpp"
#include "widget_toolkit/input_box/InputBox.hpp"
#include "main_app/themes/dark_simple/text_button.hpp"
#include "main_app/themes/dark_simple/graph_text_button.hpp"
#include "./page.hpp"
#include "main_app/main_window.hpp"
#include "widget_toolkit/notification/Notification.hpp"
#include "widget_toolkit/pseudo_code_display/PseudoCode.hpp"

namespace ds_viz::pages 
{
    class GraphVisualizer : public Page
    {
        static const int BUTTON_WIDTH = 150;
        static const int BUTTON_HEIGHT = 25;

        static const int WORKING_FRAME_HEIGHT = 440;
        static const int WORKING_FRAME_WIDTH = 700;
        
        static const int WORKING_FRAME_COORDX = DEFAULT_WIN_WIDTH - WORKING_FRAME_WIDTH;
        static const int WORKING_FRAME_COORDY = 0;

        static const int MAIN_MENU_BUTTON_POSX = 5;
        static const int MAIN_MENU_BUTTON_POSY = 5;

        static const int MAIN_MENU_BUTTON_WIDTH = 25;
        static const int MAIN_MENU_BUTTON_HEIGHT = 25;

    
        static const int NOTIFICATION_FRAME_HEIGHT = 340;
        static const int NOTIFICATION_FRAME_WIDTH = DEFAULT_WIN_WIDTH - WORKING_FRAME_WIDTH;
        static const int NOTIFICATION_FRAME_COORDX = 0;
        static const int NOTIFICATION_FRAME_COORDY = 50;

        static const int DISTANCE_BETWEEN_BUTTONS = 5;

        static const int INPUT_BOX_WIDTH = 150;
        static const int INPUT_BOX_HEIGHT = BUTTON_HEIGHT;

        static const int PSEUDOCODE_LINE_WIDTH = DEFAULT_WIN_WIDTH - WORKING_FRAME_WIDTH;
        static const int PSEUDOCODE_LINE_HEIGHT = 25;

        static const int TOGGLE_BUTTON_WIDTH = 10;
        static const int TOGGLE_BUTTON_HEIGHT = BUTTON_HEIGHT * 11 + DISTANCE_BETWEEN_BUTTONS * 10;

        static const int PSEUDO_CODE_TOGGLE_BUTTON_WIDTH = 10;
        static const int PSEUDO_CODE_TOGGLE_BUTTON_HEIGHT = 5 * PSEUDOCODE_LINE_HEIGHT;

        static const int PSEUDO_CODE_TOGGLE_BUTTON_POSX = DEFAULT_WIN_WIDTH - PSEUDO_CODE_TOGGLE_BUTTON_WIDTH;
        static const int PSEUDO_CODE_TOGGLE_BUTTON_POSY = WORKING_FRAME_COORDY + WORKING_FRAME_HEIGHT + 60;

        static const int TOGGLE_BUTTON_POSX = 0;
        static const int TOGGLE_BUTTON_POSY = DEFAULT_WIN_HEIGHT - TOGGLE_BUTTON_HEIGHT;

        static const int PSEUDO_CODE_POS_X = PSEUDO_CODE_TOGGLE_BUTTON_POSX - PSEUDOCODE_LINE_WIDTH - 5;
        static const int PSEUDO_CODE_POS_Y = PSEUDO_CODE_TOGGLE_BUTTON_POSY;

        static const int LOAD_FILE_BUTTON_POSX = TOGGLE_BUTTON_POSX + TOGGLE_BUTTON_WIDTH + 5;
        static const int LOAD_FILE_BUTTON_POSY = TOGGLE_BUTTON_POSY;

        static const int INIT_GRAPH_BUTTON_POSX = LOAD_FILE_BUTTON_POSX;
        static const int INIT_GRAPH_BUTTON_POSY = LOAD_FILE_BUTTON_POSY + BUTTON_HEIGHT + DISTANCE_BETWEEN_BUTTONS;

        static const int INSERT_NODE_BUTTON_POSX = INIT_GRAPH_BUTTON_POSX;
        static const int INSERT_NODE_BUTTON_POSY = INIT_GRAPH_BUTTON_POSY + BUTTON_HEIGHT + DISTANCE_BETWEEN_BUTTONS;

        static const int INSERT_EDGE_BUTTON_POSX = INSERT_NODE_BUTTON_POSX;
        static const int INSERT_EDGE_BUTTON_POSY = INSERT_NODE_BUTTON_POSY + BUTTON_HEIGHT + DISTANCE_BETWEEN_BUTTONS;

        static const int DELETE_NODE_BUTTON_POSX = INSERT_EDGE_BUTTON_POSX;
        static const int DELETE_NODE_BUTTON_POSY = INSERT_EDGE_BUTTON_POSY + BUTTON_HEIGHT + DISTANCE_BETWEEN_BUTTONS;

        static const int DELETE_EDGE_BUTTON_POSX = DELETE_NODE_BUTTON_POSX;
        static const int DELETE_EDGE_BUTTON_POSY = DELETE_NODE_BUTTON_POSY + BUTTON_HEIGHT + DISTANCE_BETWEEN_BUTTONS;

        static const int KRUSKAL_BUTTON_POSX = DELETE_EDGE_BUTTON_POSX;
        static const int KRUSKAL_BUTTON_POSY = DELETE_EDGE_BUTTON_POSY + BUTTON_HEIGHT + DISTANCE_BETWEEN_BUTTONS;

        static const int CLEAR_GRAPH_BUTTON_POSX = KRUSKAL_BUTTON_POSX;
        static const int CLEAR_GRAPH_BUTTON_POSY = KRUSKAL_BUTTON_POSY + BUTTON_HEIGHT + DISTANCE_BETWEEN_BUTTONS;

        static const int UNDO_BUTTON_POSX = CLEAR_GRAPH_BUTTON_POSX;
        static const int UNDO_BUTTON_POSY = CLEAR_GRAPH_BUTTON_POSY + BUTTON_HEIGHT + DISTANCE_BETWEEN_BUTTONS;

        static const int REDO_BUTTON_POSX = UNDO_BUTTON_POSX;
        static const int REDO_BUTTON_POSY = UNDO_BUTTON_POSY + BUTTON_HEIGHT + DISTANCE_BETWEEN_BUTTONS;

        static const int ADJUST_SPEED_BUTTON_POSX = REDO_BUTTON_POSX;
        static const int ADJUST_SPEED_BUTTON_POSY = REDO_BUTTON_POSY + BUTTON_HEIGHT + DISTANCE_BETWEEN_BUTTONS;

        static const int INPUT_BOX_INIT_GRAPH_POSX = INIT_GRAPH_BUTTON_POSX + BUTTON_WIDTH;
        static const int INPUT_BOX_INIT_GRAPH_POSY = INIT_GRAPH_BUTTON_POSY;

        static const int INPUT_BOX_INSERT_EDGE_POSX = INSERT_EDGE_BUTTON_POSX + BUTTON_WIDTH;
        static const int INPUT_BOX_INSERT_EDGE_POSY = INSERT_EDGE_BUTTON_POSY;

        static const int INPUT_BOX_DELETE_NODE_POSX = DELETE_NODE_BUTTON_POSX + BUTTON_WIDTH;
        static const int INPUT_BOX_DELETE_NODE_POSY = DELETE_NODE_BUTTON_POSY;

        static const int INPUT_BOX_DELETE_EDGE_POSX = DELETE_EDGE_BUTTON_POSX + BUTTON_WIDTH;
        static const int INPUT_BOX_DELETE_EDGE_POSY = DELETE_EDGE_BUTTON_POSY;

        static const int INPUT_BOX_ADJUST_SPEED_POSX = ADJUST_SPEED_BUTTON_POSX + BUTTON_WIDTH;
        static const int INPUT_BOX_ADJUST_SPEED_POSY = ADJUST_SPEED_BUTTON_POSY;
        static const int NOTIFICATION_COORDX = 0;
        static const int NOTIFICATION_COORDY = 50;

        static const int PROGRESS_BAR_POSX = WORKING_FRAME_COORDX;
        static const int PROGRESS_BAR_POSY = WORKING_FRAME_COORDY + WORKING_FRAME_HEIGHT;
        static const int PROGRESS_BAR_LENGTH = WORKING_FRAME_WIDTH;
        static const int PROGRESS_BAR_HEIGHT = 20;

        static const int PREV_STEP_BUTTON_POSX = WORKING_FRAME_COORDX;
        static const int PREV_STEP_BUTTON_POSY = WORKING_FRAME_COORDY + WORKING_FRAME_HEIGHT + PROGRESS_BAR_HEIGHT + 10;

        static const int CONTINUE_BUTTON_POSX = PREV_STEP_BUTTON_POSX + BUTTON_WIDTH + DISTANCE_BETWEEN_BUTTONS;
        static const int CONTINUE_BUTTON_POSY = WORKING_FRAME_COORDY + WORKING_FRAME_HEIGHT + PROGRESS_BAR_HEIGHT + 10;

        static const int NEXT_STEP_BUTTON_POSX = CONTINUE_BUTTON_POSX + BUTTON_WIDTH + DISTANCE_BETWEEN_BUTTONS;
        static const int NEXT_STEP_BUTTON_POSY = WORKING_FRAME_COORDY + WORKING_FRAME_HEIGHT + PROGRESS_BAR_HEIGHT + 10;


        // font and title
        std::unique_ptr<raylib::Font> font;
        raylib::Text title;
        
        // Display frame
        std::unique_ptr<raywtk::DisplayFrame> workingFrame;
        // Notification frame
        std::unique_ptr<raywtk::DisplayFrame> notificationFrame;
        // Init graph button
        std::unique_ptr<raywtk::GraphButton> initializeGraphButton;
        // Load file button
        std::unique_ptr<raywtk::GraphButton> loadFileButton;
        // Insert new node
        std::unique_ptr<raywtk::GraphButton> insertNodeButton;
        // Insert new edge
        std::unique_ptr<raywtk::GraphButton> insertEdgeButton;
        // Kruskal
        std::unique_ptr<raywtk::GraphButton> KruskalButton;
        // Delete node button
        std::unique_ptr<raywtk::GraphButton> deleteNodeButton;
        // Delete edge button
        std::unique_ptr<raywtk::GraphButton> deleteEdgeButton;
        // Undo button
        std::unique_ptr<raywtk::GraphButton> undoButton;
        // Redo button
        std::unique_ptr<raywtk::GraphButton> redoButton;
        // Clear graph button
        std::unique_ptr<raywtk::GraphButton> clearGraphButton;
        // Toggle button
        std::unique_ptr<raywtk::GraphButton> toggleButton;
        // Adjust speed button
        std::unique_ptr<raywtk::GraphButton> adjustSpeedButton;
        // Toggle button for pseudo code 
        std::unique_ptr<raywtk::GraphButton> pseudoCodeToggleButton;
        // Main menu button initialize
        std::unique_ptr<raywtk::GraphButton> mainMenuButton;
        bool showOperatorButtons = true; // Initially, operator buttons are visible
        // Notification
        std::unique_ptr<raywtk::Notification> currentNotification;
        // Input box for init graph
        std::unique_ptr<raywtk::InputBox> inputBoxInitializeGraph;
        bool inputInitializeGraphFlag = false;
        // Input box for insert new edge
        std::unique_ptr<raywtk::InputBox> inputBoxInsertEdge;
        bool inputInsertEdgeButtonFlag = false;
        // Input box for delete node
        std::unique_ptr<raywtk::InputBox> inputBoxDeleteNode;
        bool inputDeleteNodeButtonFlag = false;
        // Input box for delete edge
        std::unique_ptr<raywtk::InputBox> inputBoxDeleteEdge;
        bool inputDeleteEdgeButtonFlag = false;
        // Input box for adjust speed
        std::unique_ptr<raywtk::InputBox> inputBoxAdjustSpeed;
        bool inputAdjustSpeedFlag = false;

        // Notification
        std::unique_ptr<raywtk::Notification> notification = nullptr;

        // Kruskal index processing
        int indexProcessing = 0;
        
        // Kruskal processing flag
        bool kruskalFlag = false;
        
        // par and sz for kruskal
        std::vector<int> par, sz;
        
        // in-mst edges list
        std::set<int> inMstList;

        // vector store nodes
        std::vector<std::unique_ptr<raywtk::GraphNode>> nodes;

        // vector store edges
        std::vector<std::pair<std::pair<int, int>, int>> edges;
        
        std::vector<std::string> kruskalPseudoCode;
        std::unique_ptr<raywtk::PseudoCodeDisplay> pseudoCodeDisplay;

        struct AnimationStep {
            int current_edge_index;               // Index of the edge being processed
            std::set<int> edges_in_mst;           // Indices of edges in the MST
            std::set<int> nodes_in_mst;           // Indices of nodes in the MST
            int pseudo_code_line;                 // Index of the pseudo-code line to highlight
        };
        std::vector<AnimationStep> animationSteps;
        float animationTimer = 0.0f;
        float animationStepDuration = 1.0f; // Duration of each step in seconds
        size_t currentAnimationStep = 0;

        // Undo stack for storing the state of the graph
        std::stack<std::pair<std::vector<std::unique_ptr<raywtk::GraphNode>>, std::vector<std::pair<std::pair<int, int>, int>>>> undoStack;
        // Redo stack for storing the state of the graph
        std::stack<std::pair<std::vector<std::unique_ptr<raywtk::GraphNode>>, std::vector<std::pair<std::pair<int, int>, int>>>> redoStack;

        bool showPseudoCodeBox = true; 
        bool waitingForFile = false;

        // No processing operator flag
        bool freeFlag = true;

        std::unique_ptr<raywtk::GraphButton> prevStepButton;
        std::unique_ptr<raywtk::GraphButton> nextStepButton;
        std::unique_ptr<raywtk::GraphButton> continueButton;

        float progressBarLength = 400.0f; // Fixed length of the progress bar
        float progressBarHeight = 20.0f;  // Height of the progress bar
        int totalSteps = 0;               // Total number of steps in the algorithm
        bool animationRunning = false;    // Flag to control animation

        public:
            GraphVisualizer(ds_viz::MainWindow &context);
            void LoadGraphFromFile(const std::string& filePath); // load graph from file
            void InitializeGraph(int n); // initialize graph with n nodes
            void InsertNewNode(); // insert new node
            void InsertNewEdge(int u, int v, int c); // insert new edge
            void Kruskal();
            void RenderAnimationStep(const AnimationStep& step, const std::vector<std::pair<std::pair<int, int>, int>>& sortedEdges);
            void DeleteNode(int node); // delete node
            void DeleteEdge(int u, int v); // delete edge
            void ClearGraph(); // clear graph
            void PushToUndoStack(); // push current state to undo stack
            void Undo();
            void Redo();
            void RenderProgressBar(); // render progress bar
            void Update(float dt) override;
            void Render() override;
    };
}