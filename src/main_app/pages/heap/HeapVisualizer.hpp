#pragma once

#include <memory>
#include <vector>
#include "main_app/main_window.hpp"
#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "widget_toolkit/controls/heap_button.hpp"
#include "widget_toolkit/notification/Notification.hpp"
#include "widget_toolkit/graph_widgets/heap_node.hpp"
#include "widget_toolkit/input_box/heap_input_box.hpp"
#include "main_app/themes/dark_simple/image_button.hpp"
#include "main_app/themes/dark_simple/heap/heap_text_button.hpp"
#include "widget_toolkit/animation/animation_step.hpp"
#include "widget_toolkit/display_frame/display_frame.hpp"
#include "widget_toolkit/pseudo_code_display/heap_pseudo_code_display.hpp"
#include "../page.hpp"
#include "../../main_window.hpp"

const int HEAP_PSEUDO_CODE_FRAME_HEIGHT = 130;
const int HEAP_PSEUDO_CODE_FRAME_WIDTH = 300;
const int HEAP_PSEUDO_CODE_FRAME_COORDX = 1280 - HEAP_PSEUDO_CODE_FRAME_WIDTH - 10;
const int HEAP_PSEUDO_CODE_FRAME_COORDY = 680 - HEAP_PSEUDO_CODE_FRAME_HEIGHT;

const int HEAP_OPERATOR_BUTTON_WIDTH = 150;
const int HEAP_OPERATOR_BUTTON_HEIGHT = 25;

const int BUILD_HEAP_BUTTON_COORDX = 10;
const int BUILD_HEAP_BUTTON_COORDY = 565;

const int BUILD_HEAP_INITIALIZE_RANDOM_BUTTON_COORDX = BUILD_HEAP_BUTTON_COORDX + HEAP_OPERATOR_BUTTON_WIDTH + 5;
const int BUILD_HEAP_INITIALIZE_RANDOM_BUTTON_COORDY = BUILD_HEAP_BUTTON_COORDY;
const int BUILD_HEAP_INPUT_VALUES_BUTTON_COORDX = BUILD_HEAP_INITIALIZE_RANDOM_BUTTON_COORDX;
const int BUILD_HEAP_INPUT_VALUES_BUTTON_COORDY = BUILD_HEAP_INITIALIZE_RANDOM_BUTTON_COORDY + HEAP_OPERATOR_BUTTON_HEIGHT + 5;
const int BUILD_HEAP_LOAD_FROM_FILE_BUTTON_COORDX = BUILD_HEAP_INITIALIZE_RANDOM_BUTTON_COORDX;
const int BUILD_HEAP_LOAD_FROM_FILE_BUTTON_COORDY = BUILD_HEAP_INPUT_VALUES_BUTTON_COORDY + HEAP_OPERATOR_BUTTON_HEIGHT + 5;

const int HEAP_PUSH_VALUE_BUTTON_COORDX = BUILD_HEAP_BUTTON_COORDX;
const int HEAP_PUSH_VALUE_BUTTON_COORDY = BUILD_HEAP_BUTTON_COORDY + HEAP_OPERATOR_BUTTON_HEIGHT + 5;

const int HEAP_POP_VALUE_BUTTON_COORDX = BUILD_HEAP_BUTTON_COORDX;
const int HEAP_POP_VALUE_BUTTON_COORDY = HEAP_PUSH_VALUE_BUTTON_COORDY + HEAP_OPERATOR_BUTTON_HEIGHT + 5;

const int CLEAR_HEAP_BUTTON_COORDX = BUILD_HEAP_BUTTON_COORDX;
const int CLEAR_HEAP_BUTTON_COORDY = HEAP_POP_VALUE_BUTTON_COORDY + HEAP_OPERATOR_BUTTON_HEIGHT + 5;

const int HEAP_SHOW_OPERATOR_BUTTON_COORDX = -7;
const int HEAP_SHOW_OPERATOR_BUTTON_COORDY = BUILD_HEAP_BUTTON_COORDY;
const int HEAP_SHOW_OPERATOR_BUTTON_WIDTH = 14;
const int HEAP_SHOW_OPERATOR_BUTTON_HEIGHT = 30 * 3 + HEAP_OPERATOR_BUTTON_HEIGHT;

const int HEAP_SHOW_PSEUDO_CODE_DISPLAY_BUTTON_COORDX = 1273;
const int HEAP_SHOW_PSEUDO_CODE_DISPLAY_BUTTON_COORDY = HEAP_PSEUDO_CODE_FRAME_COORDY;
const int HEAP_SHOW_PSEUDO_CODE_DISPLAY_BUTTON_WIDTH = 14;
const int HEAP_SHOW_PSEUDO_CODE_DISPLAY_BUTTON_HEIGHT = HEAP_PSEUDO_CODE_FRAME_HEIGHT;

const int INPUT_BOX_BUILD_HEAP_COORDX = BUILD_HEAP_INPUT_VALUES_BUTTON_COORDX + HEAP_OPERATOR_BUTTON_WIDTH + 5;
const int INPUT_BOX_BUILD_HEAP_COORDY = BUILD_HEAP_INPUT_VALUES_BUTTON_COORDY;
const int INPUT_BOX_BUILD_HEAP_WIDTH = 250;
const int INPUT_BOX_BUILD_HEAP_HEIGHT = HEAP_OPERATOR_BUTTON_HEIGHT;

const int HEAP_INPUT_BOX_PUSH_VALUE_COORDX = HEAP_PUSH_VALUE_BUTTON_COORDX + HEAP_OPERATOR_BUTTON_WIDTH + 5;
const int HEAP_INPUT_BOX_PUSH_VALUE_COORDY = HEAP_PUSH_VALUE_BUTTON_COORDY;
const int HEAP_INPUT_BOX_PUSH_VALUE_WIDTH = 75;
const int HEAP_INPUT_BOX_PUSH_VALUE_HEIGHT = HEAP_OPERATOR_BUTTON_HEIGHT;

//const int NOTIFICATION_COORDX = NOTIFICATION_FRAME_COORDX + 20;
//const int NOTIFICATION_COORDY = NOTIFICATION_FRAME_COORDY + 80;

const int HEAP_PSEUDO_CODE_LINE_WIDTH = 240;
const int HEAP_PSEUDO_CODE_LINE_HEIGHT = 16;

const int HEAP_PAUSE_RESUME_BUTTON_COORDX = (1280 - 120) / 2;
const int HEAP_PAUSE_RESUME_BUTTON_COORDY = 660;
const int HEAP_PAUSE_RESUME_BUTTON_WIDTH = 120;
const int HEAP_PAUSE_RESUME_BUTTON_HEIGHT = 25;

const int HEAP_STEP_BACK_BUTTON_COORDX = HEAP_PAUSE_RESUME_BUTTON_COORDX - HEAP_PAUSE_RESUME_BUTTON_WIDTH - 5;
const int HEAP_STEP_BACK_BUTTON_COORDY = HEAP_PAUSE_RESUME_BUTTON_COORDY;
const int HEAP_STEP_BACK_BUTTON_WIDTH = HEAP_PAUSE_RESUME_BUTTON_WIDTH;
const int HEAP_STEP_BACK_BUTTON_HEIGHT = HEAP_PAUSE_RESUME_BUTTON_HEIGHT;

const int HEAP_STEP_FORWARD_BUTTON_COORDX = HEAP_PAUSE_RESUME_BUTTON_COORDX + HEAP_PAUSE_RESUME_BUTTON_WIDTH + 5;
const int HEAP_STEP_FORWARD_BUTTON_COORDY = HEAP_PAUSE_RESUME_BUTTON_COORDY;
const int HEAP_STEP_FORWARD_BUTTON_WIDTH = HEAP_PAUSE_RESUME_BUTTON_WIDTH;
const int HEAP_STEP_FORWARD_BUTTON_HEIGHT = HEAP_PAUSE_RESUME_BUTTON_HEIGHT;

const int HEAP_ANIMATION_TIMELINE_BAR_COORDX = 0;
const int HEAP_ANIMATION_TIMELINE_BAR_COORDY = 715;
const int HEAP_ANIMATION_TIMELINE_BAR_WIDTH = 1280;
const int HEAP_ANIMATION_TIMELINE_BAR_HEIGHT = 10;

const int HEAP_WORKING_FRAME_HEIGHT = 600;
const int HEAP_WORKING_FRAME_WIDTH = 1180;
const int HEAP_WORKING_FRAME_COORDX = 50;
const int HEAP_WORKING_FRAME_COORDY = 0;

const int HEAP_HOME_BUTTON_COORDX = 1210;
const int HEAP_HOME_BUTTON_COORDY = 10;
const int HEAP_HOME_BUTTON_WIDTH = 50;
const int HEAP_HOME_BUTTON_HEIGHT = 50;

//const int NOTIFICATION_FRAME_HEIGHT = 200;const int NOTIFICATION_FRAME_WIDTH = 700;const int NOTIFICATION_FRAME_COORDX = 1180;const int NOTIFICATION_FRAME_COORDY = 0;

namespace ds_viz::pages
{
    class HeapVisualizer : public Page
    {

        // font and title
        std::unique_ptr<raylib::Font> font;
        raylib::Text title;

        // notification Title
        //raylib::Text notificationTitle;

        // animation text
        raylib::Text animationText;

        // pseudo code process text
        raylib::Text pseudoCodeProcessText;

        // Working frame
        raylib::Rectangle workingFrame;

        // Animation timeline bar
        raylib::Rectangle animationTimelineBar;
        raylib::Color animationTimelineBarFilledColor;
        raylib::Color animationTimelineBarBackgroundColor;
        bool animationTimelineBarVisible = false;

        // Pseudo code frame
        raylib::Rectangle pseudoCodeFrame;
        bool pseudoCodeFrameVisible = false;

        // Notification frame
        //std::unique_ptr<raywtk::DisplayFrame> notificationFrame;
        
        // Show operator button
        std::unique_ptr<raywtk::HeapButton> showOperatorButton;

        // Build heap button
        std::unique_ptr<raywtk::HeapButton> buildHeapButton;

        // Children buttons of build heap button
        std::unique_ptr<raywtk::HeapButton> buildHeap_inputValuesButton;
        std::unique_ptr<raywtk::HeapButton> buildHeap_loadFromFileButton;
        std::unique_ptr<raywtk::HeapButton> buildHeap_initializeRandomButton;
        
        bool waitingForLoadFromFile = false;

        // Push new value button
        std::unique_ptr<raywtk::HeapButton> pushValueButton;
        
        // Pop max value button
        std::unique_ptr<raywtk::HeapButton> popMaxValueButton;
        
        // Clear heap button
        std::unique_ptr<raywtk::HeapButton> clearHeapButton;

        // Step back button
        std::unique_ptr<raywtk::HeapButton> stepBackButton;

        // Pause/resume back button
        std::unique_ptr<raywtk::HeapButton> pauseResumeButton;

        // Step forward button
        std::unique_ptr<raywtk::HeapButton> stepForwardButton;
        
        // Input box for build heap
        std::unique_ptr<raywtk::HeapInputBox> inputBoxBuildHeap;
        bool inputBuildHeapButtonFlag;
        
        // Input box for push new value
        std::unique_ptr<raywtk::HeapInputBox> inputBoxPushNewValue;
        bool inputPushNewValueButtonFlag;
        
        // Notification
        //std::unique_ptr<raywtk::Notification> notification = nullptr;

        // vector store node to perform heap operator
        std::vector<int> values;

        // vector store nodes
        std::vector<std::unique_ptr<raywtk::HeapNodeWidget>> nodes;

        // vector store animation steps
        std::vector<raywtk::Step> animation_steps;
        
        // Show pseudo code display button
        std::unique_ptr<raywtk::HeapButton> showPseudoCodeDisplayButton;

        // pseudo code display
        std::unique_ptr<raywtk::HeapPseudoCodeDisplay> pseudoCodeDisplay;

        // home button
        raylib::Texture homeButtonTex;
        std::unique_ptr<raywtk::Button> homeButton;

        // variables to manage animation speed
        int animationStep = 0;
        float animationTimer = 0.0;
        bool animationPaused = false;
        bool isLastStepForward = false;
        
        // animation speed bar
        float speedBarX = HEAP_STEP_BACK_BUTTON_COORDX + 40;
        float speedBarY = HEAP_STEP_BACK_BUTTON_COORDY - 20;
        float speedBarWidth = 300;
        float speedBarHeight = 10;
        float speedKnobRadius = 8;
        
        float speedMin = 0.5f;  // Minimum speed factor
        float speedMax = 10.0f;  // Maximum speed factor
        float animationSpeed = speedMin;
        float speedKnobX = speedBarX;  // Start at the lowest speed
        bool isDraggingSpeedKnob = false;
        std::vector<float> speedSteps = {0.5f, 1.0f, 2.0f, 4.0f, 10.0f};

        public:
            HeapVisualizer(ds_viz::MainWindow &context);
            int parent(int i); // parent of node with index i on heap
            int left_child(int i); // left child of node with index i on heap
            int right_child(int i); // right child of node with index i on heap
            void swapNodes(int i, int j, int idPseudoCode, string pseudoCodeProcessText); // swap two nodes i and j on heap
            void maxHeapify(int i); // max heapify for subtree of node with index i
            raylib::Vector2 GetPositionInDisplay(int index, int depth); // get position for node with index 'index' on working frame
            std::vector<int> GetValuesFromFiles(const std::string &filename); // get values from file
            void BuildHeap(const vector<int> &val); // build new heap
            void BuildHeap(const vector<float>& val);
            void PushNewValue(int value); // push 'value' into heap
            void PopMaxValue(); // pop the max value out of heap (root of heap)
            void ClearHeap(); // clear heap
            void ResetStatus(); // reset status before a heap operator
            void doingStep(int idStep, bool callAgain); // perform an animation step
            void undoingStep(int idStep, bool callAgain); // undo an animation step
            void changeStateOperatorButton(bool state); // change all opreator buttons state to state (state = 0 -> turn off, state = 1 -> turn on)
            void DrawTimelineBar(raylib::Rectangle bar, raylib::Color backgroundColor, raylib::Color filledColor, int currentStep, int totalSteps); // draw animation timeline bar
            void DrawSpeedBar(); // draw speed bar
            void Update(float dt) override;
            void Render() override;
    };
}