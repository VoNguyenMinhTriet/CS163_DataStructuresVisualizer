#pragma once
 
#include <memory>
#include <vector>
#include "main_app/main_window.hpp"
#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/controls/heap_button.hpp"
#include "widget_toolkit/notification/Notification.hpp"
#include "widget_toolkit/tree_widgets/node.hpp"
#include "widget_toolkit/input_box/InputBox.hpp"
#include "main_app/themes/dark_simple/heap/heap_text_button.hpp"
#include "widget_toolkit/animation/animation_step.hpp"
#include "widget_toolkit/display_frame/display_frame.hpp"
#include "widget_toolkit/pseudo_code/pseudo_code_display.hpp"
#include "../page.hpp"

#define sz(x) int((x).size())

namespace ds_viz::pages
{
    class HeapVisualizer : public Page
    {
        static constexpr float PSEUDO_CODE_FRAME_HEIGHT = 130;
        static constexpr float PSEUDO_CODE_FRAME_WIDTH = 300;
        static constexpr float PSEUDO_CODE_FRAME_COORDX = DEFAULT_WIN_WIDTH - PSEUDO_CODE_FRAME_WIDTH - 10;
        static constexpr float PSEUDO_CODE_FRAME_COORDY = 680 - PSEUDO_CODE_FRAME_HEIGHT;

        static constexpr float OPERATOR_BUTTON_WIDTH = 150;
        static constexpr float OPERATOR_BUTTON_HEIGHT = 25;

        static constexpr float BUILD_HEAP_BUTTON_COORDX = 10;
        static constexpr float BUILD_HEAP_BUTTON_COORDY = 565;

        static constexpr float BUILD_HEAP_INITIALIZE_RANDOM_BUTTON_COORDX = BUILD_HEAP_BUTTON_COORDX + OPERATOR_BUTTON_WIDTH + 5;
        static constexpr float BUILD_HEAP_INITIALIZE_RANDOM_BUTTON_COORDY = BUILD_HEAP_BUTTON_COORDY;
        static constexpr float BUILD_HEAP_INPUT_VALUES_BUTTON_COORDX = BUILD_HEAP_INITIALIZE_RANDOM_BUTTON_COORDX;
        static constexpr float BUILD_HEAP_INPUT_VALUES_BUTTON_COORDY = BUILD_HEAP_INITIALIZE_RANDOM_BUTTON_COORDY + OPERATOR_BUTTON_HEIGHT + 5;
        static constexpr float BUILD_HEAP_LOAD_FROM_FILE_BUTTON_COORDX = BUILD_HEAP_INITIALIZE_RANDOM_BUTTON_COORDX;
        static constexpr float BUILD_HEAP_LOAD_FROM_FILE_BUTTON_COORDY = BUILD_HEAP_INPUT_VALUES_BUTTON_COORDY + OPERATOR_BUTTON_HEIGHT + 5;

        static constexpr float PUSH_VALUE_BUTTON_COORDX = BUILD_HEAP_BUTTON_COORDX;
        static constexpr float PUSH_VALUE_BUTTON_COORDY = BUILD_HEAP_BUTTON_COORDY + OPERATOR_BUTTON_HEIGHT + 5;

        static constexpr float POP_VALUE_BUTTON_COORDX = BUILD_HEAP_BUTTON_COORDX;
        static constexpr float POP_VALUE_BUTTON_COORDY = PUSH_VALUE_BUTTON_COORDY + OPERATOR_BUTTON_HEIGHT + 5;

        static constexpr float CLEAR_HEAP_BUTTON_COORDX = BUILD_HEAP_BUTTON_COORDX;
        static constexpr float CLEAR_HEAP_BUTTON_COORDY = POP_VALUE_BUTTON_COORDY + OPERATOR_BUTTON_HEIGHT + 5;

        static constexpr float SHOW_OPERATOR_BUTTON_COORDX = -7;
        static constexpr float SHOW_OPERATOR_BUTTON_COORDY = BUILD_HEAP_BUTTON_COORDY;
        static constexpr float SHOW_OPERATOR_BUTTON_WIDTH = 14;
        static constexpr float SHOW_OPERATOR_BUTTON_HEIGHT = 30 * 3 + OPERATOR_BUTTON_HEIGHT;

        static constexpr float SHOW_PSEUDO_CODE_DISPLAY_BUTTON_COORDX = 1273;
        static constexpr float SHOW_PSEUDO_CODE_DISPLAY_BUTTON_COORDY = PSEUDO_CODE_FRAME_COORDY;
        static constexpr float SHOW_PSEUDO_CODE_DISPLAY_BUTTON_WIDTH = 14;
        static constexpr float SHOW_PSEUDO_CODE_DISPLAY_BUTTON_HEIGHT = PSEUDO_CODE_FRAME_HEIGHT;

        static constexpr float INPUT_BOX_BUILD_HEAP_COORDX = BUILD_HEAP_INPUT_VALUES_BUTTON_COORDX + OPERATOR_BUTTON_WIDTH + 5;
        static constexpr float INPUT_BOX_BUILD_HEAP_COORDY = BUILD_HEAP_INPUT_VALUES_BUTTON_COORDY;
        static constexpr float INPUT_BOX_BUILD_HEAP_WIDTH = 250;
        static constexpr float INPUT_BOX_BUILD_HEAP_HEIGHT = OPERATOR_BUTTON_HEIGHT;

        static constexpr float INPUT_BOX_PUSH_VALUE_COORDX = PUSH_VALUE_BUTTON_COORDX + OPERATOR_BUTTON_WIDTH + 5;
        static constexpr float INPUT_BOX_PUSH_VALUE_COORDY = PUSH_VALUE_BUTTON_COORDY;
        static constexpr float INPUT_BOX_PUSH_VALUE_WIDTH = 75;
        static constexpr float INPUT_BOX_PUSH_VALUE_HEIGHT = OPERATOR_BUTTON_HEIGHT;

        static constexpr float NOTIFICATION_COORDX = NOTIFICATION_FRAME_COORDX + 20;
        static constexpr float NOTIFICATION_COORDY = NOTIFICATION_FRAME_COORDY + 80;

        static constexpr float PSEUDO_CODE_LINE_WIDTH = 240;
        static constexpr float PSEUDO_CODE_LINE_HEIGHT = 16;

        static constexpr float PAUSE_RESUME_BUTTON_COORDX = (1280 - 120) / 2;
        static constexpr float PAUSE_RESUME_BUTTON_COORDY = 660;
        static constexpr float PAUSE_RESUME_BUTTON_WIDTH = 120;
        static constexpr float PAUSE_RESUME_BUTTON_HEIGHT = 25;

        static constexpr float STEP_BACK_BUTTON_COORDX = PAUSE_RESUME_BUTTON_COORDX - PAUSE_RESUME_BUTTON_WIDTH - 5;
        static constexpr float STEP_BACK_BUTTON_COORDY = PAUSE_RESUME_BUTTON_COORDY;
        static constexpr float STEP_BACK_BUTTON_WIDTH = PAUSE_RESUME_BUTTON_WIDTH;
        static constexpr float STEP_BACK_BUTTON_HEIGHT = PAUSE_RESUME_BUTTON_HEIGHT;

        static constexpr float STEP_FORWARD_BUTTON_COORDX = PAUSE_RESUME_BUTTON_COORDX + PAUSE_RESUME_BUTTON_WIDTH + 5;
        static constexpr float STEP_FORWARD_BUTTON_COORDY = PAUSE_RESUME_BUTTON_COORDY;
        static constexpr float STEP_FORWARD_BUTTON_WIDTH = PAUSE_RESUME_BUTTON_WIDTH;
        static constexpr float STEP_FORWARD_BUTTON_HEIGHT = PAUSE_RESUME_BUTTON_HEIGHT;

        static constexpr float ANIMATION_TIMELINE_BAR_COORDX = 0;
        static constexpr float ANIMATION_TIMELINE_BAR_COORDY = 715;
        static constexpr float ANIMATION_TIMELINE_BAR_WIDTH = 1280;
        static constexpr float ANIMATION_TIMELINE_BAR_HEIGHT = 10;


        /*static constexpr float NOTIFICATION_FRAME_HEIGHT = 200;
        static constexpr float NOTIFICATION_FRAME_WIDTH = 700;
        static constexpr float NOTIFICATION_FRAME_COORDX = 1180;
        static constexpr float NOTIFICATION_FRAME_COORDY = 0;*/

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
        std::vector<std::unique_ptr<raywtk::TreeNodeWidget>> nodes;

        // vector store animation steps
        std::vector<raywtk::Step> animation_steps;
        
        // Show pseudo code display button
        std::unique_ptr<raywtk::HeapButton> showPseudoCodeDisplayButton;

        // pseudo code display
        std::unique_ptr<raywtk::HeapPseudoCodeDisplay> pseudoCodeDisplay;

        // variables to manage animation speed
        int animationStep = 0;
        float animationTimer = 0.0;
        float animationSpeed = 1.0f;
        bool animationPaused = false;
        bool isLastStepForward = false;

        public:
            HeapVisualizer(MainWindow &context);
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
            void Update(float dt) override;
            void Render() override;
    };
}