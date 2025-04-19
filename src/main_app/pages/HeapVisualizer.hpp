#pragma once
 
#include <memory>
#include <vector>
#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/controls/button2.hpp"
#include "widget_toolkit/notification/Notification.hpp"
#include "widget_toolkit/tree_widgets/node.hpp"
#include "widget_toolkit/input_box/InputBox.hpp"
#include "widget_toolkit/controls/text_button.hpp"
#include "widget_toolkit/animation/animation_step.hpp"
#include "widget_toolkit/display_frame/display_frame.hpp"
#include "widget_toolkit/pseudo_code/pseudo_code_display.hpp"
#include "./page.hpp"

const int OPERATOR_BUTTON_WIDTH = 150;
const int OPERATOR_BUTTON_HEIGHT = 25;

const int BUILD_HEAP_BUTTON_COORDX = 10;
const int BUILD_HEAP_BUTTON_COORDY = 565;

const int BUILD_HEAP_INITIALIZE_RANDOM_BUTTON_COORDX = BUILD_HEAP_BUTTON_COORDX + OPERATOR_BUTTON_WIDTH + 5;
const int BUILD_HEAP_INITIALIZE_RANDOM_BUTTON_COORDY = BUILD_HEAP_BUTTON_COORDY;
const int BUILD_HEAP_INPUT_VALUES_BUTTON_COORDX = BUILD_HEAP_INITIALIZE_RANDOM_BUTTON_COORDX;
const int BUILD_HEAP_INPUT_VALUES_BUTTON_COORDY = BUILD_HEAP_INITIALIZE_RANDOM_BUTTON_COORDY + OPERATOR_BUTTON_HEIGHT + 5;
const int BUILD_HEAP_LOAD_FROM_FILE_BUTTON_COORDX = BUILD_HEAP_INITIALIZE_RANDOM_BUTTON_COORDX;
const int BUILD_HEAP_LOAD_FROM_FILE_BUTTON_COORDY = BUILD_HEAP_INPUT_VALUES_BUTTON_COORDY + OPERATOR_BUTTON_HEIGHT + 5;

const int PUSH_VALUE_BUTTON_COORDX = BUILD_HEAP_BUTTON_COORDX;
const int PUSH_VALUE_BUTTON_COORDY = BUILD_HEAP_BUTTON_COORDY + OPERATOR_BUTTON_HEIGHT + 5;

const int POP_VALUE_BUTTON_COORDX = BUILD_HEAP_BUTTON_COORDX;
const int POP_VALUE_BUTTON_COORDY = PUSH_VALUE_BUTTON_COORDY + OPERATOR_BUTTON_HEIGHT + 5;

const int CLEAR_HEAP_BUTTON_COORDX = BUILD_HEAP_BUTTON_COORDX;
const int CLEAR_HEAP_BUTTON_COORDY = POP_VALUE_BUTTON_COORDY + OPERATOR_BUTTON_HEIGHT + 5;

const int SHOW_OPERATOR_BUTTON_COORDX = -7;
const int SHOW_OPERATOR_BUTTON_COORDY = BUILD_HEAP_BUTTON_COORDY;
const int SHOW_OPERATOR_BUTTON_WIDTH = 14;
const int SHOW_OPERATOR_BUTTON_HEIGHT = 30 * 3 + OPERATOR_BUTTON_HEIGHT;

const int SHOW_PSEUDO_CODE_DISPLAY_BUTTON_COORDX = 1273;
const int SHOW_PSEUDO_CODE_DISPLAY_BUTTON_COORDY = PSEUDO_CODE_FRAME_COORDY;
const int SHOW_PSEUDO_CODE_DISPLAY_BUTTON_WIDTH = 14;
const int SHOW_PSEUDO_CODE_DISPLAY_BUTTON_HEIGHT = PSEUDO_CODE_FRAME_HEIGHT;

const int INPUT_BOX_BUILD_HEAP_COORDX = BUILD_HEAP_INPUT_VALUES_BUTTON_COORDX + OPERATOR_BUTTON_WIDTH + 5;
const int INPUT_BOX_BUILD_HEAP_COORDY = BUILD_HEAP_INPUT_VALUES_BUTTON_COORDY;
const int INPUT_BOX_BUILD_HEAP_WIDTH = 250;
const int INPUT_BOX_BUILD_HEAP_HEIGHT = OPERATOR_BUTTON_HEIGHT;

const int INPUT_BOX_PUSH_VALUE_COORDX = PUSH_VALUE_BUTTON_COORDX + OPERATOR_BUTTON_WIDTH + 5;
const int INPUT_BOX_PUSH_VALUE_COORDY = PUSH_VALUE_BUTTON_COORDY;
const int INPUT_BOX_PUSH_VALUE_WIDTH = 75;
const int INPUT_BOX_PUSH_VALUE_HEIGHT = OPERATOR_BUTTON_HEIGHT;

const int NOTIFICATION_COORDX = NOTIFICATION_FRAME_COORDX + 20;
const int NOTIFICATION_COORDY = NOTIFICATION_FRAME_COORDY + 80;

const int PSEUDO_CODE_LINE_WIDTH = 240;
const int PSEUDO_CODE_LINE_HEIGHT = 16;

const int PAUSE_RESUME_BUTTON_COORDX = (1280 - 120) / 2;
const int PAUSE_RESUME_BUTTON_COORDY = 660;
const int PAUSE_RESUME_BUTTON_WIDTH = 120;
const int PAUSE_RESUME_BUTTON_HEIGHT = 25;

const int STEP_BACK_BUTTON_COORDX = PAUSE_RESUME_BUTTON_COORDX - PAUSE_RESUME_BUTTON_WIDTH - 5;
const int STEP_BACK_BUTTON_COORDY = PAUSE_RESUME_BUTTON_COORDY;
const int STEP_BACK_BUTTON_WIDTH = PAUSE_RESUME_BUTTON_WIDTH;
const int STEP_BACK_BUTTON_HEIGHT = PAUSE_RESUME_BUTTON_HEIGHT;

const int STEP_FORWARD_BUTTON_COORDX = PAUSE_RESUME_BUTTON_COORDX + PAUSE_RESUME_BUTTON_WIDTH + 5;
const int STEP_FORWARD_BUTTON_COORDY = PAUSE_RESUME_BUTTON_COORDY;
const int STEP_FORWARD_BUTTON_WIDTH = PAUSE_RESUME_BUTTON_WIDTH;
const int STEP_FORWARD_BUTTON_HEIGHT = PAUSE_RESUME_BUTTON_HEIGHT;

const int ANIMATION_TIMELINE_BAR_COORDX = 0;
const int ANIMATION_TIMELINE_BAR_COORDY = 715;
const int ANIMATION_TIMELINE_BAR_WIDTH = 1280;
const int ANIMATION_TIMELINE_BAR_HEIGHT = 10;

#define sz(x) int((x).size())

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
        std::unique_ptr<raywtk::Button> showOperatorButton;

        // Build heap button
        std::unique_ptr<raywtk::Button> buildHeapButton;

        // Children buttons of build heap button
        std::unique_ptr<raywtk::Button> buildHeap_inputValuesButton;
        std::unique_ptr<raywtk::Button> buildHeap_loadFromFileButton;
        std::unique_ptr<raywtk::Button> buildHeap_initializeRandomButton;
        
        bool waitingForLoadFromFile = false;

        // Push new value button
        std::unique_ptr<raywtk::Button> pushValueButton;
        
        // Pop max value button
        std::unique_ptr<raywtk::Button> popMaxValueButton;
        
        // Clear heap button
        std::unique_ptr<raywtk::Button> clearHeapButton;

        // Step back button
        std::unique_ptr<raywtk::Button> stepBackButton;

        // Pause/resume back button
        std::unique_ptr<raywtk::Button> pauseResumeButton;

        // Step forward button
        std::unique_ptr<raywtk::Button> stepForwardButton;
        
        // Input box for build heap
        std::unique_ptr<raywtk::InputBox> inputBoxBuildHeap;
        bool inputBuildHeapButtonFlag;
        
        // Input box for push new value
        std::unique_ptr<raywtk::InputBox> inputBoxPushNewValue;
        bool inputPushNewValueButtonFlag;
        
        // Notification
        //std::unique_ptr<raywtk::Notification> notification = nullptr;

        // vector store node to perform heap operator
        std::vector<int> values;

        // vector store nodes
        std::vector<std::unique_ptr<raywtk::NodeWidget>> nodes;

        // vector store animation steps
        std::vector<raywtk::Step> animation_steps;
        
        // Show pseudo code display button
        std::unique_ptr<raywtk::Button> showPseudoCodeDisplayButton;

        // pseudo code display
        std::unique_ptr<raywtk::PseudoCodeDisplay> pseudoCodeDisplay;

        // variables to manage animation speed
        int animationStep = 0;
        float animationTimer = 0.0;
        float animationSpeed = 1.0f;
        bool animationPaused = false;
        bool isLastStepForward = false;

        public:
            HeapVisualizer();
            int parent(int i); // parent of node with index i on heap
            int left_child(int i); // left child of node with index i on heap
            int right_child(int i); // right child of node with index i on heap
            void swapNodes(int i, int j, int idPseudoCode, string pseudoCodeProcessText); // swap two nodes i and j on heap
            void maxHeapify(int i); // max heapify for subtree of node with index i
            raylib::Vector2 GetPositionInDisplay(int index, int depth); // get position for node with index 'index' on working frame
            std::vector<int> GetValuesFromFiles(const std::string &filename); // get values from file
            void BuildHeap(const vector<int> &val); // build new heap
            void PushNewValue(int value); // push 'value' into heap
            void PopMaxValue(); // pop the max value out of heap (root of heap)
            void ClearHeap(); // clear heap
            void ResetStatus(); // reset status before a heap operator
            void doingStep(int idStep, bool callAgain); // perform an animation step
            void undoingStep(int idStep, bool callAgain); // undo an animation step
            void changeStateOperatorButton(bool state); // change all opreator buttons state to state (state = 0 -> turn off, state = 1 -> turn on)
            void DrawTimelineBar(raylib::Rectangle bar, raylib::Color backgroundColor, raylib::Color filledColor, int currentStep, int totalSteps); // draw animation timeline bar
            void Update(float dt) override;
            void Render() override;
    };
}