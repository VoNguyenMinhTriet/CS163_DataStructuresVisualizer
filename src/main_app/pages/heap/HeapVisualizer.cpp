#include "./HeapVisualizer.hpp"
#include "../main_menu.hpp"
#include "../../main_window.hpp"
#include "main_app/pages/page.hpp"
#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>
#include <cstring>
#include <cassert>
#include <fstream>

using namespace ds_viz::pages;

#define sz(x) int((x).size())

HeapVisualizer::HeapVisualizer(ds_viz::MainWindow &context) : ds_viz::Page(context)
{
    // window width and height initalize
    //windowWidth = _context->ref_raylib_window->GetWidth();
    //windowHeight = _context->ref_raylib_window->GetHeight();

    // font and title initialize
    font = std::make_unique<raylib::Font>("./ttf/InterDisplay-Black.ttf", 128, nullptr, 250);
    title = raylib::Text("Heap Visualizer", 100, raylib::Color(255, 255, 255, 128), *font, 0);
    
    // notification title
    //notificationTitle = raylib::Text("Notification: ", 40, raylib::Color::Yellow(), *font, 0);

    // Working Frame initialize
    //workingFrame = raylib::Rectangle(WF_COORDX, WF_COORDY, WF_WIDTH, WF_HEIGHT);

    // Pseudo Code Frame initialize
    pseudoCodeFrame = raylib::Rectangle(HEAP_PSEUDO_CODE_FRAME_COORDX, HEAP_PSEUDO_CODE_FRAME_COORDY, HEAP_PSEUDO_CODE_FRAME_WIDTH, HEAP_PSEUDO_CODE_FRAME_HEIGHT);
    pseudoCodeFrameVisible = false;

    animationTimelineBar = raylib::Rectangle(HEAP_ANIMATION_TIMELINE_BAR_COORDX, HEAP_ANIMATION_TIMELINE_BAR_COORDY, HEAP_ANIMATION_TIMELINE_BAR_WIDTH, HEAP_ANIMATION_TIMELINE_BAR_HEIGHT);
    animationTimelineBarFilledColor = raylib::Color::Green();
    animationTimelineBarBackgroundColor = raylib::Color::Gray();
    animationTimelineBarVisible = false;

    // Notification Frame initialize
    //notificationFrame = std::make_unique<raywtk::DisplayFrame>(raylib::Rectangle(NOTIFICATION_FRAME_COORDX, NOTIFICATION_FRAME_COORDY, NOTIFICATION_FRAME_WIDTH, NOTIFICATION_FRAME_HEIGHT), raylib::Color::Gray(), 5.0f);

    // Build heap button initialize
    buildHeapButton = std::make_unique<raywtk::HeapButton>();
    buildHeapButton->showing = false;
    buildHeapButton->buttonText = "Build heap";
    buildHeapButton->buttonRect = raylib::Rectangle(BUILD_HEAP_BUTTON_COORDX, BUILD_HEAP_BUTTON_COORDY, HEAP_OPERATOR_BUTTON_WIDTH, HEAP_OPERATOR_BUTTON_HEIGHT);
    buildHeapButton->style = std::make_unique<ds_viz::themes::dark_simple::HeapButtonStyle>();
    buildHeapButton->Click.append([this]() 
    {
        buildHeap_initializeRandomButton->showing = buildHeap_loadFromFileButton->showing = buildHeap_inputValuesButton->showing = 1 - buildHeap_inputValuesButton->showing;
        //notification = std::make_unique<raywtk::Notification>("Insert numbers (<= 31 numbers) to build a new heap.", raywtk::NotificationType::INFO, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
    });
    
    // Initialize random button initialize
    buildHeap_initializeRandomButton = std::make_unique<raywtk::HeapButton>();
    buildHeap_initializeRandomButton->showing = false;
    buildHeap_initializeRandomButton->buttonText = "Initialize random";
    buildHeap_initializeRandomButton->buttonRect = raylib::Rectangle(BUILD_HEAP_INITIALIZE_RANDOM_BUTTON_COORDX, BUILD_HEAP_INITIALIZE_RANDOM_BUTTON_COORDY, HEAP_OPERATOR_BUTTON_WIDTH, HEAP_OPERATOR_BUTTON_HEIGHT);
    buildHeap_initializeRandomButton->style = std::make_unique<ds_viz::themes::dark_simple::HeapButtonStyle>();
    buildHeap_initializeRandomButton->Click.append([this]() 
    {
        srand(time(0)); // Seed for random number generation
        vector<int> randomValues;
        
        int numValues = rand() %  31 + 1; // Random number of values between 1 and 31
        for(int i = 0; i < numValues; ++i) 
        {
            randomValues.push_back(rand() % 199 - 99); // Random values between -99 and 99
        }
        
        BuildHeap(randomValues);
        //notification = std::make_unique<raywtk::Notification>("Initialize heap with random values successfully.", raywtk::NotificationType::SUCCESS, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
    });
    
    // Build heap input values button initialize
    buildHeap_inputValuesButton = std::make_unique<raywtk::HeapButton>();
    buildHeap_inputValuesButton->showing = false;
    buildHeap_inputValuesButton->buttonText = "Input values";
    buildHeap_inputValuesButton->buttonRect = raylib::Rectangle(BUILD_HEAP_INPUT_VALUES_BUTTON_COORDX, BUILD_HEAP_INPUT_VALUES_BUTTON_COORDY, HEAP_OPERATOR_BUTTON_WIDTH, HEAP_OPERATOR_BUTTON_HEIGHT);
    buildHeap_inputValuesButton->style = std::make_unique<ds_viz::themes::dark_simple::HeapButtonStyle>();
    buildHeap_inputValuesButton->Click.append([this]() 
    {
        inputBuildHeapButtonFlag = inputBoxBuildHeap->processing = true; 
        //notification = std::make_unique<raywtk::Notification>("Insert numbers (<= 31 numbers) to build a new heap.", raywtk::NotificationType::INFO, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
    });

    // Load from file button initialize
    buildHeap_loadFromFileButton = std::make_unique<raywtk::HeapButton>();
    buildHeap_loadFromFileButton->showing = false;
    buildHeap_loadFromFileButton->buttonText = "Load from file";
    buildHeap_loadFromFileButton->buttonRect = raylib::Rectangle(BUILD_HEAP_LOAD_FROM_FILE_BUTTON_COORDX, BUILD_HEAP_LOAD_FROM_FILE_BUTTON_COORDY, HEAP_OPERATOR_BUTTON_WIDTH, HEAP_OPERATOR_BUTTON_HEIGHT);
    buildHeap_loadFromFileButton->style = std::make_unique<ds_viz::themes::dark_simple::HeapButtonStyle>();
    buildHeap_loadFromFileButton->Click.append([this]() 
    {
        waitingForLoadFromFile = true;
        //notification = std::make_unique<raywtk::Notification>("Load heap from file successfully.", raywtk::NotificationType::SUCCESS, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
    });
    
    // Push new value button initialize
    pushValueButton = std::make_unique<raywtk::HeapButton>();
    pushValueButton->showing = false;
    pushValueButton->buttonText = "Push new value";
    pushValueButton->buttonRect = raylib::Rectangle(HEAP_PUSH_VALUE_BUTTON_COORDX, HEAP_PUSH_VALUE_BUTTON_COORDY, HEAP_OPERATOR_BUTTON_WIDTH, HEAP_OPERATOR_BUTTON_HEIGHT);
    pushValueButton->style = std::make_unique<ds_viz::themes::dark_simple::HeapButtonStyle>();
    pushValueButton->Click.append([this]() 
    {
        inputPushNewValueButtonFlag = true; inputBoxPushNewValue->processing = true; 
        //notification = std::make_unique<raywtk::Notification>("Insert a number to push to heap.", raywtk::NotificationType::INFO, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
    });
    
    // Pop max value button initialize
    popMaxValueButton = std::make_unique<raywtk::HeapButton>();
    popMaxValueButton->showing = false;
    popMaxValueButton->buttonText = "Pop max value";
    popMaxValueButton->buttonRect = raylib::Rectangle(HEAP_POP_VALUE_BUTTON_COORDX, HEAP_POP_VALUE_BUTTON_COORDY, HEAP_OPERATOR_BUTTON_WIDTH, HEAP_OPERATOR_BUTTON_HEIGHT);
    popMaxValueButton->style = std::make_unique<ds_viz::themes::dark_simple::HeapButtonStyle>();
    popMaxValueButton->Click.append([this]() 
    {
        PopMaxValue();
    });

    // Clear heap button initialize
    clearHeapButton = std::make_unique<raywtk::HeapButton>();
    clearHeapButton->showing = false;
    clearHeapButton->buttonText = "Clear heap";
    clearHeapButton->buttonRect = raylib::Rectangle(CLEAR_HEAP_BUTTON_COORDX, CLEAR_HEAP_BUTTON_COORDY, HEAP_OPERATOR_BUTTON_WIDTH, HEAP_OPERATOR_BUTTON_HEIGHT);
    clearHeapButton->style = std::make_unique<ds_viz::themes::dark_simple::HeapButtonStyle>();
    clearHeapButton->Click.append([this]() 
    {
        ClearHeap();
    });
    
    // Show operator button
    showOperatorButton = std::make_unique<raywtk::HeapButton>();
    showOperatorButton->buttonText = "";
    showOperatorButton->buttonRect = raylib::Rectangle(HEAP_SHOW_OPERATOR_BUTTON_COORDX, HEAP_SHOW_OPERATOR_BUTTON_COORDY, HEAP_SHOW_OPERATOR_BUTTON_WIDTH, HEAP_SHOW_OPERATOR_BUTTON_HEIGHT);
    showOperatorButton->style = std::make_unique<ds_viz::themes::dark_simple::HeapButtonStyle>();
    showOperatorButton->Click.append([this]()
    {
        buildHeapButton->showing = pushValueButton->showing = popMaxValueButton->showing = clearHeapButton->showing = 1 - clearHeapButton->showing;
    });

    // Input box build heap initialize
    inputBuildHeapButtonFlag = false;
    inputBoxBuildHeap = std::make_unique<raywtk::HeapInputBox>(raylib::Rectangle(INPUT_BOX_BUILD_HEAP_COORDX, INPUT_BOX_BUILD_HEAP_COORDY, INPUT_BOX_BUILD_HEAP_WIDTH, INPUT_BOX_BUILD_HEAP_HEIGHT), raylib::Color::White(), raylib::Color::Black(), raylib::Color::Pink(), 31, false);
    
    // Input box push new value initialize
    inputPushNewValueButtonFlag = false;
    inputBoxPushNewValue = std::make_unique<raywtk::HeapInputBox>(raylib::Rectangle(HEAP_INPUT_BOX_PUSH_VALUE_COORDX, HEAP_INPUT_BOX_PUSH_VALUE_COORDY, HEAP_INPUT_BOX_PUSH_VALUE_WIDTH, HEAP_INPUT_BOX_PUSH_VALUE_HEIGHT), raylib::Color::White(), raylib::Color::Black(), raylib::Color::Pink(), 1, false);

    // Animation text initialize
    animationText = raylib::Text("No animation is currently on display.", 18, raylib::Color::White(), *font, 0);

    // Animation initialize
    animation_steps.clear();
    animationStep = 0, animationTimer = 1.0f, animationSpeed = speedMin;
    animation_steps.push_back(raywtk::Step(raywtk::StepType::None, -1, -1, -1, ""));

    // Pseudo code display initialize
    pseudoCodeDisplay = std::make_unique<raywtk::HeapPseudoCodeDisplay>(raylib::Vector2(HEAP_PSEUDO_CODE_FRAME_COORDX + 5, HEAP_PSEUDO_CODE_FRAME_COORDY + 5), 8, HEAP_PSEUDO_CODE_LINE_WIDTH, HEAP_PSEUDO_CODE_LINE_HEIGHT, raylib::Color::White(), raylib::Color::Yellow(), raylib::Color::Green());
    pseudoCodeDisplay->visible = false;

    // Pseudo code process text initialize
    pseudoCodeProcessText = raylib::Text("", 18, raylib::Color::White(), *font, 0);

    // Show pseudo code display button initialize
    showPseudoCodeDisplayButton = std::make_unique<raywtk::HeapButton>();
    showPseudoCodeDisplayButton->buttonText = "";
    showPseudoCodeDisplayButton->buttonRect = raylib::Rectangle(HEAP_SHOW_PSEUDO_CODE_DISPLAY_BUTTON_COORDX, HEAP_SHOW_PSEUDO_CODE_DISPLAY_BUTTON_COORDY, HEAP_SHOW_PSEUDO_CODE_DISPLAY_BUTTON_WIDTH, HEAP_SHOW_PSEUDO_CODE_DISPLAY_BUTTON_HEIGHT);
    showPseudoCodeDisplayButton->style = std::make_unique<ds_viz::themes::dark_simple::HeapButtonStyle>();
    showPseudoCodeDisplayButton->Click.append([this]()
    {
        pseudoCodeFrameVisible = pseudoCodeDisplay->visible = 1 - pseudoCodeFrameVisible;
    });
    
    // Step forward button initialize
    stepForwardButton = std::make_unique<raywtk::HeapButton>();
    stepForwardButton->enabled = false;
    stepForwardButton->buttonText = "Step forward";
    stepForwardButton->buttonRect = raylib::Rectangle(HEAP_STEP_FORWARD_BUTTON_COORDX, HEAP_STEP_FORWARD_BUTTON_COORDY, HEAP_STEP_FORWARD_BUTTON_WIDTH, HEAP_STEP_FORWARD_BUTTON_HEIGHT);
    stepForwardButton->style = std::make_unique<ds_viz::themes::dark_simple::HeapButtonStyle>();
    stepForwardButton->Click.append([this]()
    {
        animationTimer = 1.0f;
    });

    // Step back button initialize
    stepBackButton = std::make_unique<raywtk::HeapButton>();
    stepBackButton->enabled = false;
    stepBackButton->buttonText = "Step back";
    stepBackButton->buttonRect = raylib::Rectangle(HEAP_STEP_BACK_BUTTON_COORDX, HEAP_STEP_BACK_BUTTON_COORDY, HEAP_STEP_BACK_BUTTON_WIDTH, HEAP_STEP_BACK_BUTTON_HEIGHT);
    stepBackButton->style = std::make_unique<ds_viz::themes::dark_simple::HeapButtonStyle>();
    stepBackButton->Click.append([this]()
    {
        animationTimer = -1.0f;
    });

    // Pause/resume button initialize
    pauseResumeButton = std::make_unique<raywtk::HeapButton>();
    pauseResumeButton->buttonText = "Pause";
    pauseResumeButton->buttonRect = raylib::Rectangle(HEAP_PAUSE_RESUME_BUTTON_COORDX, HEAP_PAUSE_RESUME_BUTTON_COORDY, HEAP_PAUSE_RESUME_BUTTON_WIDTH, HEAP_PAUSE_RESUME_BUTTON_HEIGHT);
    pauseResumeButton->style = std::make_unique<ds_viz::themes::dark_simple::HeapButtonStyle>();
    pauseResumeButton->Click.append([this]()
    {
        animationTimer = 0.0f;
        animationPaused = 1 - animationPaused;
        pauseResumeButton->buttonText = (animationPaused) ? "Play" : "Pause";
        stepBackButton->enabled = stepForwardButton->enabled = 1 - stepBackButton->enabled;
    });

    // Home button initialize
    homeButtonTex = raylib::Texture(raylib::Image("./images/return_button.png"));
    homeButton = std::make_unique<raywtk::Button>();
    homeButton->buttonText = "";
    homeButton->buttonRect = raylib::Rectangle(HEAP_HOME_BUTTON_COORDX, HEAP_HOME_BUTTON_COORDY, HEAP_HOME_BUTTON_WIDTH, HEAP_HOME_BUTTON_HEIGHT);
    homeButton->style = std::make_unique<ds_viz::themes::dark_simple::ImageButtonStyle>(&homeButtonTex);
    homeButton->Click.append([this]()
    {
        ClearHeap();
        _context->ChangePage(std::make_shared<ds_viz::pages::MainMenuPage>(*_context));
    });
}

inline int HeapVisualizer::parent(int i)
{
    return (i - 1) / 2;
}

inline int HeapVisualizer::left_child(int i) 
{
    return i * 2 + 1;
}

inline int HeapVisualizer::right_child(int i)
{
    return i * 2 + 2;
}

void HeapVisualizer::ResetStatus()
{
    animation_steps.clear();
    waitingForLoadFromFile = false;
    animationTimelineBarVisible = false;
    animationStep = 0, isLastStepForward = true, animationTimer = 1.0f;
    animation_steps.push_back(raywtk::Step(raywtk::StepType::None, -1, -1, -1, ""));
}

void HeapVisualizer::doingStep(int idStep, bool callAgain) 
{
    raywtk::Step step;
    step.type = animation_steps[idStep].type;
    step.u = animation_steps[idStep].u;
    step.v = animation_steps[idStep].v;
    step.node = std::move(animation_steps[idStep].node);
    step.idPseudoCodeLine = animation_steps[idStep].idPseudoCodeLine;
    step.pseudoCodeProcessText = animation_steps[idStep].pseudoCodeProcessText;
    step.pseudoCodeLines = animation_steps[idStep].pseudoCodeLines;
           
    pseudoCodeDisplay->SwapLineState(step.idPseudoCodeLine);
    pseudoCodeProcessText.SetText(step.pseudoCodeProcessText);
    if(step.type == raywtk::StepType::InsertNode) 
    {
        if(!callAgain) 
        {
            nodes.push_back(std::move(step.node));
        }
        
        nodes.back()->highlighted ^= 1;
    }

    if(step.type == raywtk::StepType::Highlight1Node) 
    {

        nodes[step.u]->highlighted ^= 1;
    }
    
    if(step.type == raywtk::StepType::DeleteNode && !callAgain)
    {
        step.node = std::move(nodes.back());
        nodes.pop_back();
    }
    
    if(step.type == raywtk::StepType::Swap2Nodes && !callAgain) 
    {
        swap(nodes[step.u]->position, nodes[step.v]->position);
        swap(nodes[step.u], nodes[step.v]);
    }

    if(step.type == raywtk::StepType::Highlight2Nodes)
    {
        nodes[step.u]->highlighted ^= 1;
        nodes[step.v]->highlighted ^= 1;
    }

    if(step.type == raywtk::StepType::SetNewPseudoCodeLines && !callAgain) 
    {
        pseudoCodeDisplay->SetPseudoCodeLines(step.pseudoCodeLines);
    }

    if(step.node != nullptr)
    {
        animation_steps[idStep].node = std::move(step.node);
    }
}

void HeapVisualizer::undoingStep(int idStep, bool callAgain) 
{
    raywtk::Step step;
    step.type = animation_steps[idStep].type;
    step.u = animation_steps[idStep].u;
    step.v = animation_steps[idStep].v;
    step.node = std::move(animation_steps[idStep].node);
    step.idPseudoCodeLine = animation_steps[idStep].idPseudoCodeLine;
    step.pseudoCodeProcessText = animation_steps[idStep].pseudoCodeProcessText;
    step.pseudoCodeLines = animation_steps[idStep].pseudoCodeLines;
    step.lastPseudoCodeLines = animation_steps[idStep].lastPseudoCodeLines;

    pseudoCodeDisplay->SwapLineState(step.idPseudoCodeLine);
    pseudoCodeProcessText.SetText(step.pseudoCodeProcessText);
    if(step.type == raywtk::StepType::InsertNode) 
    {
        nodes.back()->highlighted ^= 1;
        if(!callAgain) 
        {
            step.node = std::move(nodes.back());
            nodes.pop_back();
        }
    }

    if(step.type == raywtk::StepType::Highlight1Node) 
    {

        nodes[step.u]->highlighted ^= 1;
    }
    
    if(step.type == raywtk::StepType::DeleteNode && !callAgain)
    {
        nodes.push_back(std::move(step.node));
    }
    
    if(step.type == raywtk::StepType::Swap2Nodes && !callAgain) 
    {
        swap(nodes[step.u]->position, nodes[step.v]->position);
        swap(nodes[step.u], nodes[step.v]);
    }

    if(step.type == raywtk::StepType::Highlight2Nodes)
    {
        nodes[step.u]->highlighted ^= 1;
        nodes[step.v]->highlighted ^= 1;
    }

    if(step.type == raywtk::StepType::SetNewPseudoCodeLines && !callAgain) 
    {
        pseudoCodeDisplay->SetPseudoCodeLines(step.lastPseudoCodeLines);
    }

    if(step.node != nullptr)
    {
        animation_steps[idStep].node = std::move(step.node);
    }
}

void HeapVisualizer::swapNodes(int i, int j, int idpseudoCodeLine, string pseudoCodeProcessText) 
{
    swap(values[i], values[j]);
    animation_steps.push_back(raywtk::Step(raywtk::StepType::Swap2Nodes, i, j, idpseudoCodeLine, pseudoCodeProcessText));
}

void HeapVisualizer::maxHeapify(int i) 
{
    if(i >= sz(values))
        return;

    int L = left_child(i), R = right_child(i);
    
    raywtk::StepType stepType = (L < sz(values)) ? raywtk::StepType::Highlight1Node : raywtk::StepType::None;
    animation_steps.push_back(raywtk::Step(stepType, L, -1, 0, "Setting variable k to LeftChild(i)."));
    
    stepType = (R < sz(values)) ? raywtk::StepType::Highlight1Node : raywtk::StepType::None;
    animation_steps.push_back(raywtk::Step(stepType, R, -1, 1, "Setting variable R to RightChild(i)."));
    
    if(R < sz(values))
    {
        string text = (values[L] < values[R]) ? "heap[k] is smaller than heap[R]." : "heap[k] is not smaller than heap[R].";
        animation_steps.push_back(raywtk::Step(raywtk::StepType::Highlight2Nodes, L, R, 2, text));
        if(values[L] < values[R]) 
        {
            L = R;
            animation_steps.push_back(raywtk::Step(raywtk::StepType::Highlight1Node, L, -1, 3, "Setting variable k to R."));
        }
    } 
    else 
    {
        animation_steps.push_back(raywtk::Step(raywtk::StepType::None, -1, -1, 2, "R is bigger than or equal to heap size."));
    }

    if(L < sz(values))
    {
        string text = (values[L] > values[i]) ? "heap[k] is bigger than heap[i]." : "heap[k] is not bigger than heap[i].";
        animation_steps.push_back(raywtk::Step(raywtk::StepType::Highlight2Nodes, i, L, 4, text));
        if(values[L] > values[i]) 
        {
            swapNodes(i, L, -1, "Swap heap[i] and heap[k].");

            animation_steps.push_back(raywtk::Step(raywtk::StepType::Highlight1Node, L, -1, 5, "Call recursive to function MaxHeapify(k)."));
            maxHeapify(L);
        }
    }
    else
    {
        animation_steps.push_back(raywtk::Step(raywtk::StepType::None, -1, -1, 4, "k is bigger than or equal to heap size."));
    }
}

raylib::Vector2 HeapVisualizer::GetPositionInDisplay(int index, int depth) 
{
    while(index >= (1 << depth)) 
    {
        index -= (1 << depth);
        ++depth;
    }
    
    raylib::Vector2 position = raylib::Vector2(HEAP_WORKING_FRAME_COORDX, HEAP_WORKING_FRAME_COORDY);
    position.x += 60 + double(HEAP_WORKING_FRAME_WIDTH - 120) / ((1 << depth)) / 2 + double(HEAP_WORKING_FRAME_WIDTH - 120) / ((1 << depth)) * (index);
    position.y += 50 + (HEAP_WORKING_FRAME_HEIGHT - 50) / 5 * (depth);
    return position;
}

std::vector<int> HeapVisualizer::GetValuesFromFiles(const std::string &filename) 
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        //notification = std::make_unique<raywtk::Notification>("Failed to open file.", raywtk::NotificationType::ERROR, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
        return vector<int>();
    }

    std::vector<int> values;
    std::string line;
    int x;

    while(file >> x)
    {
        if(sz(values) >= 31)
        {
            //notification = std::make_unique<raywtk::Notification>("Failed to get more values in files since number of values reached 31 (maximum of heap size is 31).", raywtk::NotificationType::ERROR, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
            break;
        }

        values.push_back(x);
    }
    
    file.close();
    return values;
}

void HeapVisualizer::BuildHeap(const vector<int> &val) 
{
    ClearHeap();
    animationTimelineBarVisible = true;
    animation_steps.push_back(raywtk::Step(raywtk::StepType::SetNewPseudoCodeLines, -1, -1, -1, "Calling to function BuildHeap(input_values).", BUILD_HEAP_PSEUDO_CODE, vector<string>(), nullptr));
    animationText.SetText("Building heap with these input values.");
    
    animation_steps.push_back(raywtk::Step(raywtk::StepType::None, -1, -1, 0, "Clear heap."));

    for (int i = 0; i < min(31, sz(val)); ++i) {
        std::unique_ptr<raywtk::HeapNodeWidget> newNode = std::make_unique<raywtk::HeapNodeWidget>(val[i]);
        newNode->position = GetPositionInDisplay(sz(values), 0);
        values.push_back(val[i]);

        animation_steps.push_back(raywtk::Step(raywtk::StepType::InsertNode, i, -1, 2, "Add " + to_string(val[i]) + " to the end of heap.", std::move(newNode)));
    }

    animation_steps.push_back(raywtk::Step(raywtk::StepType::None, -1, -1, 3, "Setting variable k to Parent(last element)."));

    int k = parent(sz(values) - 1); // parent of last leaf node on heap
    for (int i = k; i >= 0; --i)
    {
        animation_steps.push_back(raywtk::Step(raywtk::StepType::None, -1, -1, 5, "Calling to function MaxHeapify(i)."));
        animation_steps.push_back(raywtk::Step(raywtk::StepType::SetNewPseudoCodeLines, -1, -1, -1, "Calling to function MaxHeapify(i).", MAX_HEAPIFY_PSEUDO_CODE, BUILD_HEAP_PSEUDO_CODE, nullptr));
        
        maxHeapify(i);
        animation_steps.push_back(raywtk::Step(raywtk::StepType::SetNewPseudoCodeLines, -1, -1, -1, "Backing to function BuildHeap(input_values).", BUILD_HEAP_PSEUDO_CODE, MAX_HEAPIFY_PSEUDO_CODE, nullptr));
    }

    //notification = std::make_unique<raywtk::Notification>("Build a new heap with these numbers successfully.", raywtk::NotificationType::SUCCESS, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
}

void HeapVisualizer::PushNewValue(int value) 
{
    ResetStatus();
    if(sz(values) >= 31)
    {
        //notification = std::make_unique<raywtk::Notification>("Failed to push a value into heap since heap's size reached 31.", raywtk::NotificationType::ERROR, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
        return;
    }
    
    changeStateOperatorButton(false);
    animationTimelineBarVisible = true;
    animationText.SetText("Pushing value " + to_string(value) + " into heap.");
    animation_steps.push_back(raywtk::Step(raywtk::StepType::SetNewPseudoCodeLines, -1, -1, -1, "Calling to function PushNewValue(value).", PUSH_NEW_VALUE_PSEUDO_CODE, vector<string>(), nullptr));
    
    std::unique_ptr<raywtk::HeapNodeWidget> newNode = std::make_unique<raywtk::HeapNodeWidget>(value);
    newNode->position = GetPositionInDisplay(sz(values), 0);
    values.push_back(value);

    animation_steps.push_back(raywtk::Step(raywtk::StepType::InsertNode, sz(values) - 1, -1, 0, "Add " + to_string(value) + " to the end of heap.", std::move(newNode)));
    
    animation_steps.push_back(raywtk::Step(raywtk::StepType::Highlight1Node, sz(values) - 1, -1, 1, "Setting variable i to index of the last element of heap.")); 

    int i = sz(values) - 1;
    while(i > 0) 
    {
        string text = (values[i] > values[parent(i)]) ? "heap[i] is bigger than heap[Parent(i)]." : "heap[i] is not bigger than heap[Parent(i)].";
        animation_steps.push_back(raywtk::Step(raywtk::StepType::Highlight2Nodes, i, parent(i), 2, text));
        if(values[i] > values[parent(i)]) 
        {
            swapNodes(i, parent(i), 3, "Swap heap[i] and heap[Parent(i)].");
            i = parent(i);
            animation_steps.push_back(raywtk::Step(raywtk::StepType::Highlight1Node, i, -1, 4, "Moving variable i to Parent(i).")); 
        } else {
            break;
        }
    }

    if(i == 0)
    {
        animation_steps.push_back(raywtk::Step(raywtk::StepType::None, -1, -1, 2, "i is smaller than or equal to 0."));
    }

    //notification = std::make_unique<raywtk::Notification>("Push a number with value " + std::to_string(value) + " into heap successfully.", raywtk::NotificationType::SUCCESS, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
}

void HeapVisualizer::PopMaxValue()
{
    ResetStatus();
    if(sz(values) == 0) 
    {
        //notification = std::make_unique<raywtk::Notification>("Failed to pop max value out of an empty heap.", raywtk::NotificationType::ERROR, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
        return;
    }
    
    changeStateOperatorButton(false);
    animationTimelineBarVisible = true;
    animationText.SetText("Popping the element with maximum value (value = " + to_string(values[0]) + ") out of heap.");
    animation_steps.push_back(raywtk::Step(raywtk::StepType::SetNewPseudoCodeLines, -1, -1, -1, "Calling to function PopMaxValue().", POP_MAX_VALUE_PSEUDO_CODE, vector<string>(), nullptr));
    
    int i = sz(values) - 1;
    animation_steps.push_back(raywtk::Step(raywtk::StepType::Highlight2Nodes, i, 0, -1, "Swap root with last element on heap."));
    swapNodes(i, 0, 0, "Swap root with last element on heap.");
    
    //notification = std::make_unique<raywtk::Notification>("Pop max value (value = " + std::to_string(values[i]) + ") out of heap successfully.", raywtk::NotificationType::SUCCESS, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
    values.pop_back();
    animation_steps.push_back(raywtk::Step(raywtk::StepType::Highlight1Node, i, -1, 1, "Remove last element from heap."));
    animation_steps.push_back(raywtk::Step(raywtk::StepType::DeleteNode, i, -1, 1, "Remove last element from heap."));
    
    animation_steps.push_back(raywtk::Step(raywtk::StepType::None, -1, -1, 2, "Call to function MaxHeapify(0).")); 
    animation_steps.push_back(raywtk::Step(raywtk::StepType::SetNewPseudoCodeLines, -1, -1, -1, "Call to function MaxHeapify(0).", MAX_HEAPIFY_PSEUDO_CODE, POP_MAX_VALUE_PSEUDO_CODE, nullptr)); 
    maxHeapify(0);

    animation_steps.push_back(raywtk::Step(raywtk::StepType::SetNewPseudoCodeLines, -1, -1, -1, "Back to function PopMaxValue().", POP_MAX_VALUE_PSEUDO_CODE, MAX_HEAPIFY_PSEUDO_CODE, nullptr));
}

void HeapVisualizer::ClearHeap()
{
    ResetStatus();
    changeStateOperatorButton(false);
    values.clear();
    nodes.clear();
    //notification = std::make_unique<raywtk::Notification>("Clear the heap successfully.", raywtk::NotificationType::SUCCESS, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
}

void HeapVisualizer::changeStateOperatorButton(bool state)
{
    pushValueButton->enabled = popMaxValueButton->enabled = buildHeapButton->enabled = clearHeapButton->enabled = buildHeap_initializeRandomButton->enabled = buildHeap_inputValuesButton->enabled = buildHeap_loadFromFileButton->enabled = state;
}

void HeapVisualizer::DrawTimelineBar(raylib::Rectangle bar, raylib::Color backgroundColor, raylib::Color filledColor, int currentStep, int totalSteps) 
{
    if (totalSteps <= 0) 
        return;

    // Calculate the width of the filled portion of the bar
    float progressWidth = (float(currentStep - 1) / (totalSteps - 1)) * bar.GetWidth();

    // Draw the background of the timeline bar
    DrawRectangle(bar.GetX(), bar.GetY(), bar.GetWidth(), bar.GetHeight(), backgroundColor);

    // Draw the filled portion of the timeline bar
    DrawRectangle(bar.GetX(), bar.GetY(), progressWidth, bar.GetHeight(), filledColor);
}

void HeapVisualizer::DrawSpeedBar()
{
    DrawRectangle(speedBarX, speedBarY, speedBarWidth, speedBarHeight, RAYWHITE);
    DrawCircle(speedKnobX, (speedBarY + speedBarHeight / 2), speedKnobRadius, RED);
    DrawText("Speed:", (speedBarX - 80), (speedBarY - 5), 20, WHITE);
    
    char speedText[10];
    sprintf(speedText, "%.2fx", animationSpeed);
    DrawText(speedText, (speedBarX + speedBarWidth + 10), (speedBarY - 5), 20, YELLOW);
}

void HeapVisualizer::Update(float dt) 
{
    // window width and height update
    //windowWidth = _context->ref_raylib_window->GetWidth();
    //windowHeight = _context->ref_raylib_window->GetHeight();

    // Notification Frame update
    //notificationFrame->Update(dt);

    // Show operator button update
    showOperatorButton->Update(dt);

    // Build heap button update
    buildHeapButton->Update(dt);

    // Initialize random button update
    buildHeap_initializeRandomButton->Update(dt);

    // Input box build heap update
    buildHeap_inputValuesButton->Update(dt);

    // Show input box build heap update
    if(inputBuildHeapButtonFlag) 
    {
        if(raylib::Mouse::IsButtonPressed(MOUSE_LEFT_BUTTON) && !raylib::Mouse::GetPosition().CheckCollision(buildHeap_inputValuesButton->buttonRect) && !raylib::Mouse::GetPosition().CheckCollision(inputBoxBuildHeap->rect))
        {
            inputBuildHeapButtonFlag = false;
            inputBoxBuildHeap->Reset();
        } else 
        {
            inputBoxBuildHeap->Update(dt);
            if(!inputBoxBuildHeap->processing)
            {
                auto values = inputBoxBuildHeap->values;
                BuildHeap(values);
                inputBuildHeapButtonFlag = false;
                inputBoxBuildHeap->Reset();
            }
        }
    }

    // Load from file button update
    buildHeap_loadFromFileButton->Update(dt);

    if(waitingForLoadFromFile && IsFileDropped())
    {
        std::vector<std::string> droppedFiles = raylib::LoadDroppedFiles();
        std::string filePath = droppedFiles[0];
        std::vector<int> vals = GetValuesFromFiles(filePath);
        BuildHeap(vals);
    }

    // Push new value button update
    pushValueButton->Update(dt);
    
    // Show input box push new value update
    if (inputPushNewValueButtonFlag)
    {
        if(raylib::Mouse::IsButtonPressed(MOUSE_LEFT_BUTTON) && !raylib::Mouse::GetPosition().CheckCollision(pushValueButton->buttonRect) && !raylib::Mouse::GetPosition().CheckCollision(inputBoxPushNewValue->rect)) 
        {
            inputPushNewValueButtonFlag = false;
            inputBoxPushNewValue->Reset();
        } else 
        {
            inputBoxPushNewValue->Update(dt);
            if(!inputBoxPushNewValue->processing) 
            {
                auto values = inputBoxPushNewValue->values;
                PushNewValue(values[0]);
                inputBoxPushNewValue->Reset();
                inputPushNewValueButtonFlag = false;
            }
        }
    }

    // Pop max value button update
    popMaxValueButton->Update(dt);

    // Clear heap button update
    clearHeapButton->Update(dt);

    // Animation update
    animationTimer += (!animationPaused) * animationSpeed * dt;
    if(animationTimer <= -1.0f && animationStep > 1)
    {
        --animationStep;
        animationTimer = 0.0f;

        changeStateOperatorButton(false);
        if(animationStep + 1 < sz(animation_steps))
        {
            undoingStep(animationStep + 1, false);
        }

        isLastStepForward = false;
        undoingStep(animationStep, true);
    }
    else
        if(animationTimer >= 1.0f && animationStep < sz(animation_steps)) 
        {
            ++animationStep;
            animationTimer = 0.0f;

            doingStep(animationStep - 1, true);
            if(animationStep < sz(animation_steps)) 
            {
                doingStep(animationStep, false);
            } 
            else 
            {
                changeStateOperatorButton(true);
                animationText.SetText("No animation is currently on display.");
                pseudoCodeProcessText.SetText("End of timeline.");
            }

            isLastStepForward = true;
        }

    // Show pseudo code button update
    showPseudoCodeDisplayButton->Update(dt);

    // Pseudo code display update
    pseudoCodeDisplay->Update(dt);

    // Pause/resume button update
    pauseResumeButton->Update(dt);

    // Step forward button update
    stepForwardButton->Update(dt);

    // Step back button update
    stepBackButton->Update(dt);

    // Home button update
    homeButton->Update(dt);

    // Adjust speed update
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 mousePos = GetMousePosition();
        if (isDraggingSpeedKnob || CheckCollisionPointCircle(mousePos, {speedKnobX, speedBarY + speedBarHeight / 2}, speedKnobRadius))
        {
            isDraggingSpeedKnob = true;
            speedKnobX = Clamp(mousePos.x, speedBarX, speedBarX + speedBarWidth);

            // Update speed while dragging 
            float t = (speedKnobX - speedBarX) / speedBarWidth;
            animationSpeed = speedMin + t * (speedMax - speedMin);
        }
    }
    else 
    {
        isDraggingSpeedKnob = false;
    }

    // vector nodes update
    for(auto &node : nodes)
    {
        node->Update(dt);
    }
}

void HeapVisualizer::Render() 
{
    // notification title draw
    //notificationTitle.Draw(NOTIFICATION_FRAME_COORDX + 10, NOTIFICATION_FRAME_COORDY + 10);
    
    // Working Frame render
    //workingFrame.DrawRoundedLines(0.0f, 10, raylib::Color::Gray());
    
    // Pseudo Code Frame render
    if(pseudoCodeFrameVisible)
    {
        pseudoCodeFrame.DrawRoundedLines(0.1f, 10, raylib::Color::Pink());
    }
    
    // title draw
    int titleTextLen = MeasureTextEx(title.GetFont(), title.GetText().c_str(), title.GetFontSize(), 0).x;
    title.Draw((1280 - titleTextLen) / 2.0, 25);
    
    // Notification Frame render
    //notificationFrame->Render();

    // Show operator button render
    showOperatorButton->Render();

    // Build heap button render
    buildHeapButton->Render();

    // Initialize random to build heap button render
    buildHeap_initializeRandomButton->Render();

    // Input values to build heap button render
    buildHeap_inputValuesButton->Render();  

    // Input box of input values to build heap button render
    inputBoxBuildHeap->Render();

    // Load from file to build heap button render
    buildHeap_loadFromFileButton->Render();

    // Input box push new value button render
    inputBoxPushNewValue->Render();

    // Insert new value button render
    pushValueButton->Render();

    // Pop max value button render
    popMaxValueButton->Render();

    // Clear heap button render
    clearHeapButton->Render();

    // Notification render
    // if(notification != nullptr) { notification->Render(); }

    // Animation text render
    animationText.Draw(5, windowHeight - 30);

    // Show pseudo code button render
    showPseudoCodeDisplayButton->Render();

    // Pseudo code display render
    pseudoCodeDisplay->Render();

    // Pseudo code process text render
    int pseudoCodeProcessTextLen = MeasureTextEx(pseudoCodeProcessText.GetFont(), pseudoCodeProcessText.GetText().c_str(), pseudoCodeProcessText.GetFontSize(), 0).x;
    pseudoCodeProcessText.Draw(windowWidth - 5 - pseudoCodeProcessTextLen, windowHeight - 30);

    // Pause/resume button render
    pauseResumeButton->Render();

    // Step forward button render
    stepForwardButton->Render();

    // Step back button render
    stepBackButton->Render();

    // Home button render
    homeButton->Render();

    // Animation timeline bar render
    if(animationTimelineBarVisible) 
    {
        DrawTimelineBar(animationTimelineBar, animationTimelineBarBackgroundColor, animationTimelineBarFilledColor, animationStep, sz(animation_steps));
    }

    // Speed bar render
    DrawSpeedBar();

    // Edges render
    for (int i = 1; i < sz(nodes); ++i) 
    {
        int par_i = parent(i);
        raylib::Vector2 startPos = nodes[par_i]->position;
        raylib::Vector2 endPos = nodes[i]->position;
        startPos.DrawLine(endPos, 2.5f, WHITE);
    }

    // Vector nodes render
    for(auto &node : nodes)
    {
        node->Render();
    }
}

void ds_viz::pages::HeapVisualizer::BuildHeap(const vector<float>& val)
{
    std::vector<int> intVal(val.size());
    for (size_t i = 0; i < val.size(); ++i)
    {
        intVal[i] = static_cast<int>(val[i]);
    }
    BuildHeap(intVal);
}
