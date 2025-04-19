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
#include "widget_toolkit/graph_widgets/node.hpp"
#include "widget_toolkit/graph_widgets/edge.hpp"
#include "widget_toolkit/display_frame/display_frame.hpp"
#include "widget_toolkit/input_box/InputBox.hpp"
#include "main_app/themes/dark_simple/text_button.hpp"
#include "./page.hpp"
#include "widget_toolkit/notification/Notification.hpp"
#include "widget_toolkit/pseudo_code_display/PseudoCode.hpp"

const int BUTTON_WIDTH = 200;
const int BUTTON_HEIGHT = 50;

const int DISTANCE_BETWEEN_BUTTONS = 10;

const int INPUT_BOX_WIDTH = 200;
const int INPUT_BOX_HEIGHT = 50;

const int PSEUDOCODE_LINE_WIDTH = 700;
const int PSEUDOCODE_LINE_HEIGHT = 50;

const int TOGGLE_BUTTON_WIDTH = 10;
const int TOGGLE_BUTTON_HEIGHT = BUTTON_HEIGHT * 11 + DISTANCE_BETWEEN_BUTTONS * 10;

const int PSEUDO_CODE_TOGGLE_BUTTON_POSX = 2500;
const int PSEUDO_CODE_TOGGLE_BUTTON_POSY = 200;

const int PSEUDO_CODE_TOGGLE_BUTTON_WIDTH = 10;
const int PSEUDO_CODE_TOGGLE_BUTTON_HEIGHT = 5 * PSEUDOCODE_LINE_HEIGHT;

const int TOGGLE_BUTTON_POSX = 100;
const int TOGGLE_BUTTON_POSY = 200;

const int PSEUDO_CODE_POS_X = PSEUDO_CODE_TOGGLE_BUTTON_POSX - PSEUDOCODE_LINE_WIDTH - 5;
const int PSEUDO_CODE_POS_Y = PSEUDO_CODE_TOGGLE_BUTTON_POSY;

const int LOAD_FILE_BUTTON_POSX = TOGGLE_BUTTON_POSX + TOGGLE_BUTTON_WIDTH + 5;
const int LOAD_FILE_BUTTON_POSY = TOGGLE_BUTTON_POSY;

const int INIT_GRAPH_BUTTON_POSX = LOAD_FILE_BUTTON_POSX;
const int INIT_GRAPH_BUTTON_POSY = LOAD_FILE_BUTTON_POSY + BUTTON_HEIGHT + DISTANCE_BETWEEN_BUTTONS;

const int INSERT_NODE_BUTTON_POSX = INIT_GRAPH_BUTTON_POSX;
const int INSERT_NODE_BUTTON_POSY = INIT_GRAPH_BUTTON_POSY + BUTTON_HEIGHT + DISTANCE_BETWEEN_BUTTONS;

const int INSERT_EDGE_BUTTON_POSX = INSERT_NODE_BUTTON_POSX;
const int INSERT_EDGE_BUTTON_POSY = INSERT_NODE_BUTTON_POSY + BUTTON_HEIGHT + DISTANCE_BETWEEN_BUTTONS;

const int DELETE_NODE_BUTTON_POSX = INSERT_EDGE_BUTTON_POSX;
const int DELETE_NODE_BUTTON_POSY = INSERT_EDGE_BUTTON_POSY + BUTTON_HEIGHT + DISTANCE_BETWEEN_BUTTONS;

const int DELETE_EDGE_BUTTON_POSX = DELETE_NODE_BUTTON_POSX;
const int DELETE_EDGE_BUTTON_POSY = DELETE_NODE_BUTTON_POSY + BUTTON_HEIGHT + DISTANCE_BETWEEN_BUTTONS;

const int KRUSKAL_BUTTON_POSX = DELETE_EDGE_BUTTON_POSX;
const int KRUSKAL_BUTTON_POSY = DELETE_EDGE_BUTTON_POSY + BUTTON_HEIGHT + DISTANCE_BETWEEN_BUTTONS;

const int CLEAR_GRAPH_BUTTON_POSX = KRUSKAL_BUTTON_POSX;
const int CLEAR_GRAPH_BUTTON_POSY = KRUSKAL_BUTTON_POSY + BUTTON_HEIGHT + DISTANCE_BETWEEN_BUTTONS;

const int UNDO_BUTTON_POSX = CLEAR_GRAPH_BUTTON_POSX;
const int UNDO_BUTTON_POSY = CLEAR_GRAPH_BUTTON_POSY + BUTTON_HEIGHT + DISTANCE_BETWEEN_BUTTONS;

const int REDO_BUTTON_POSX = UNDO_BUTTON_POSX;
const int REDO_BUTTON_POSY = UNDO_BUTTON_POSY + BUTTON_HEIGHT + DISTANCE_BETWEEN_BUTTONS;

const int ADJUST_SPEED_BUTTON_POSX = REDO_BUTTON_POSX;
const int ADJUST_SPEED_BUTTON_POSY = REDO_BUTTON_POSY + BUTTON_HEIGHT + DISTANCE_BETWEEN_BUTTONS;

const int INPUT_BOX_INIT_GRAPH_POSX = INIT_GRAPH_BUTTON_POSX + BUTTON_WIDTH;
const int INPUT_BOX_INIT_GRAPH_POSY = INIT_GRAPH_BUTTON_POSY;

const int INPUT_BOX_INSERT_EDGE_POSX = INSERT_EDGE_BUTTON_POSX + BUTTON_WIDTH;
const int INPUT_BOX_INSERT_EDGE_POSY = INSERT_EDGE_BUTTON_POSY;

const int INPUT_BOX_DELETE_NODE_POSX = DELETE_NODE_BUTTON_POSX + BUTTON_WIDTH;
const int INPUT_BOX_DELETE_NODE_POSY = DELETE_NODE_BUTTON_POSY;

const int INPUT_BOX_DELETE_EDGE_POSX = DELETE_EDGE_BUTTON_POSX + BUTTON_WIDTH;
const int INPUT_BOX_DELETE_EDGE_POSY = DELETE_EDGE_BUTTON_POSY;

const int INPUT_BOX_ADJUST_SPEED_POSX = ADJUST_SPEED_BUTTON_POSX + BUTTON_WIDTH;
const int INPUT_BOX_ADJUST_SPEED_POSY = ADJUST_SPEED_BUTTON_POSY;

const int NOTIFICATION_COORDX = NOTIFICATION_FRAME_COORDX + 20;
const int NOTIFICATION_COORDY = NOTIFICATION_FRAME_COORDY + 20;

const int PROGRESS_BAR_POSX = WORKING_FRAME_COORDX;
const int PROGRESS_BAR_POSY = WORKING_FRAME_COORDY + WORKING_FRAME_HEIGHT;
const int PROGRESS_BAR_LENGTH = WORKING_FRAME_WIDTH;
const int PROGRESS_BAR_HEIGHT = 20;

const int PREV_STEP_BUTTON_POSX = WORKING_FRAME_COORDX;
const int PREV_STEP_BUTTON_POSY = WORKING_FRAME_COORDY + WORKING_FRAME_HEIGHT + PROGRESS_BAR_HEIGHT + 10;

const int CONTINUE_BUTTON_POSX = PREV_STEP_BUTTON_POSX + BUTTON_WIDTH + DISTANCE_BETWEEN_BUTTONS;
const int CONTINUE_BUTTON_POSY = WORKING_FRAME_COORDY + WORKING_FRAME_HEIGHT + PROGRESS_BAR_HEIGHT + 10;

const int NEXT_STEP_BUTTON_POSX = CONTINUE_BUTTON_POSX + BUTTON_WIDTH + DISTANCE_BETWEEN_BUTTONS;
const int NEXT_STEP_BUTTON_POSY = WORKING_FRAME_COORDY + WORKING_FRAME_HEIGHT + PROGRESS_BAR_HEIGHT + 10;


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
        // Init graph button
        std::unique_ptr<raywtk::Button> initializeGraphButton;
        // Load file button
        std::unique_ptr<raywtk::Button> loadFileButton;
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
        // Undo button
        std::unique_ptr<raywtk::Button> undoButton;
        // Redo button
        std::unique_ptr<raywtk::Button> redoButton;
        // Clear graph button
        std::unique_ptr<raywtk::Button> clearGraphButton;
        // Toggle button
        std::unique_ptr<raywtk::Button> toggleButton;
        // Adjust speed button
        std::unique_ptr<raywtk::Button> adjustSpeedButton;
        // Toggle button for pseudo code 
        std::unique_ptr<raywtk::Button> pseudoCodeToggleButton;
        bool showOperatorButtons = true; // Initially, operator buttons are visible
        // Notification
        std::unique_ptr<raywtk::Notification> currentNotification;
        // Input box for init graph
        std::unique_ptr<raywtk::InputBox> inputBoxInitializeGraph;
        bool inputInitializeGraphFlag;
        // Input box for insert new edge
        std::unique_ptr<raywtk::InputBox> inputBoxInsertEdge;
        bool inputInsertEdgeButtonFlag;
        // Input box for delete node
        std::unique_ptr<raywtk::InputBox> inputBoxDeleteNode;
        bool inputDeleteNodeButtonFlag;
        // Input box for delete edge
        std::unique_ptr<raywtk::InputBox> inputBoxDeleteEdge;
        bool inputDeleteEdgeButtonFlag;
        // Input box for adjust speed
        std::unique_ptr<raywtk::InputBox> inputBoxAdjustSpeed;
        bool inputAdjustSpeedFlag;

        // Notification
        std::unique_ptr<raywtk::Notification> notification = nullptr;

        // Kruskal index processing
        int indexProcessing;
        
        // Kruskal processing flag
        bool kruskalFlag;
        
        // par and sz for kruskal
        std::vector<int> par, sz;
        
        // in-mst edges list
        std::set<int> inMstList;

        // vector store nodes
        std::vector<std::unique_ptr<raywtk::NodeWidget>> nodes;

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
        std::stack<std::pair<std::vector<std::unique_ptr<raywtk::NodeWidget>>, std::vector<std::pair<std::pair<int, int>, int>>>> undoStack;
        // Redo stack for storing the state of the graph
        std::stack<std::pair<std::vector<std::unique_ptr<raywtk::NodeWidget>>, std::vector<std::pair<std::pair<int, int>, int>>>> redoStack;

        bool showPseudoCodeBox = true; 
        bool waitingForFile = false;

        // No processing operator flag
        bool freeFlag = true;

        std::unique_ptr<raywtk::Button> prevStepButton;
        std::unique_ptr<raywtk::Button> nextStepButton;
        std::unique_ptr<raywtk::Button> continueButton;

        float progressBarLength = 400.0f; // Fixed length of the progress bar
        float progressBarHeight = 20.0f;  // Height of the progress bar
        int totalSteps = 0;               // Total number of steps in the algorithm
        bool animationRunning = false;    // Flag to control animation

        public:
            GraphVisualizer();
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