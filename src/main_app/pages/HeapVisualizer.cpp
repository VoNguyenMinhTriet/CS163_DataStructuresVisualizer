#include "./HeapVisualizer.hpp"
#include "main_app/main_window.hpp"
#include "./main_menu.hpp"
#include "../themes/dark_simple/back_button.hpp"
#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>
#include <cstring>
#include <cassert>

using namespace ds_viz::pages;

HeapVisualizer::HeapVisualizer() 
{
    // font and title initialize
    font = std::make_unique<raylib::Font>("./ttf/InterDisplay-Black.ttf", 128, nullptr, 250);
    title = raylib::Text("Heap Visualizer", 40, raylib::Color::White(), *font, 0);

    // notification title
    //notificationTitle = raylib::Text("Notification: ", 40, raylib::Color::Yellow(), *font, 0);

    // Working Frame initialize
    workingFrame = std::make_unique<raywtk::DisplayFrame>(raylib::Rectangle(WORKING_FRAME_COORDX, WORKING_FRAME_COORDY, WORKING_FRAME_WIDTH, WORKING_FRAME_HEIGHT), raylib::Color::Gray(), 5.0f);

    // Pseudo Code Frame initialize
    pseudoCodeFrame = std::make_unique<raywtk::DisplayFrame>(raylib::Rectangle(PSEUDO_CODE_FRAME_COORDX, PSEUDO_CODE_FRAME_COORDY, PSEUDO_CODE_FRAME_WIDTH, PSEUDO_CODE_FRAME_HEIGHT), raylib::Color::Pink(), 2.0f);
    pseudoCodeFrame->visible = false;

    // Notification Frame initialize
    //notificationFrame = std::make_unique<raywtk::DisplayFrame>(raylib::Rectangle(NOTIFICATION_FRAME_COORDX, NOTIFICATION_FRAME_COORDY, NOTIFICATION_FRAME_WIDTH, NOTIFICATION_FRAME_HEIGHT), raylib::Color::Gray(), 5.0f);

    // Build heap button initialize
    buildHeapButton = std::make_unique<raywtk::Button>();
    buildHeapButton->showing = false;
    buildHeapButton->buttonText = "Build heap";
    buildHeapButton->buttonRect = raylib::Rectangle(BUILD_HEAP_BUTTON_COORDX, BUILD_HEAP_BUTTON_COORDY, OPERATOR_BUTTON_WIDTH, OPERATOR_BUTTON_HEIGHT);
    buildHeapButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();
    buildHeapButton->Click.append([this]() 
    {
        inputBuildHeapButtonFlag = true; inputBoxBuildHeap->processing = true; 
        //notification = std::make_unique<raywtk::Notification>("Insert numbers (<= 31 numbers) to build a new heap.", raywtk::NotificationType::INFO, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
    });

    // Push new value button initialize
    pushValueButton = std::make_unique<raywtk::Button>();
    pushValueButton->showing = false;
    pushValueButton->buttonText = "Push new value";
    pushValueButton->buttonRect = raylib::Rectangle(PUSH_VALUE_BUTTON_COORDX, PUSH_VALUE_BUTTON_COORDY, OPERATOR_BUTTON_WIDTH, OPERATOR_BUTTON_HEIGHT);
    pushValueButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();
    pushValueButton->Click.append([this]() 
    {
        inputPushNewValueButtonFlag = true; inputBoxPushNewValue->processing = true; 
        //notification = std::make_unique<raywtk::Notification>("Insert a number to push to heap.", raywtk::NotificationType::INFO, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
    });
    
    // Pop max value button initialize
    popMaxValueButton = std::make_unique<raywtk::Button>();
    popMaxValueButton->showing = false;
    popMaxValueButton->buttonText = "Pop max value";
    popMaxValueButton->buttonRect = raylib::Rectangle(POP_VALUE_BUTTON_COORDX, POP_VALUE_BUTTON_COORDY, OPERATOR_BUTTON_WIDTH, OPERATOR_BUTTON_HEIGHT);
    popMaxValueButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();
    popMaxValueButton->Click.append([this]() 
    {
        PopMaxValue();
    });

    // Clear heap button initialize
    clearHeapButton = std::make_unique<raywtk::Button>();
    clearHeapButton->showing = false;
    clearHeapButton->buttonText = "Clear heap";
    clearHeapButton->buttonRect = raylib::Rectangle(CLEAR_HEAP_BUTTON_COORDX, CLEAR_HEAP_BUTTON_COORDY, OPERATOR_BUTTON_WIDTH, OPERATOR_BUTTON_HEIGHT);
    clearHeapButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();
    clearHeapButton->Click.append([this]() 
    {
        ClearHeap();
    });
    
    // Show operator button
    showOperatorButton = std::make_unique<raywtk::Button>();
    showOperatorButton->buttonText = "";
    showOperatorButton->buttonRect = raylib::Rectangle(SHOW_OPERATOR_BUTTON_COORDX, SHOW_OPERATOR_BUTTON_COORDY, SHOW_OPERATOR_BUTTON_WIDTH, SHOW_OPERATOR_BUTTON_HEIGHT);
    showOperatorButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();
    showOperatorButton->Click.append([this]()
    {
        buildHeapButton->showing = pushValueButton->showing = popMaxValueButton->showing = clearHeapButton->showing = 1 - clearHeapButton->showing;
    });

    // Input box build heap initialize
    inputBuildHeapButtonFlag = false;
    inputBoxBuildHeap = std::make_unique<raywtk::InputBox>(raylib::Rectangle(INPUT_BOX_BUILD_HEAP_COORDX, INPUT_BOX_BUILD_HEAP_COORDY, INPUT_BOX_BUILD_HEAP_WIDTH, INPUT_BOX_BUILD_HEAP_HEIGHT), raylib::Color::White(), raylib::Color::Black(), raylib::Color::Pink(), 31, false);
    
    // Input box push new value initialize
    inputPushNewValueButtonFlag = false;
    inputBoxPushNewValue = std::make_unique<raywtk::InputBox>(raylib::Rectangle(INPUT_BOX_PUSH_VALUE_COORDX, INPUT_BOX_PUSH_VALUE_COORDY, INPUT_BOX_PUSH_VALUE_WIDTH, INPUT_BOX_PUSH_VALUE_HEIGHT), raylib::Color::White(), raylib::Color::Black(), raylib::Color::Pink(), 1, false);

    // Animation text initialize
    animationText = raylib::Text("No animation is currently on display.", 18, raylib::Color::White(), *font, 0);

    // Animation initialize
    animation_steps.clear();
    animationStep = 0, animationTimer = 1.0f;
    animation_steps.push_back(raywtk::Step(raywtk::StepType::None, -1, -1));

    // Pseudo code display initialize
    pseudoCodeDisplay = std::make_unique<raywtk::PseudoCodeDisplay>(raylib::Vector2(PSEUDO_CODE_FRAME_COORDX + 5, PSEUDO_CODE_FRAME_COORDY), 8, PSEUDO_CODE_LINE_WIDTH, PSEUDO_CODE_LINE_HEIGHT, raylib::Color::White(), raylib::Color::Yellow(), raylib::Color::Green());

    // Show pseudo code display button initialize
    showPseudoCodeDisplayButton = std::make_unique<raywtk::Button>();
    showPseudoCodeDisplayButton->buttonText = "";
    showPseudoCodeDisplayButton->buttonRect = raylib::Rectangle(SHOW_PSEUDO_CODE_DISPLAY_BUTTON_COORDX, SHOW_PSEUDO_CODE_DISPLAY_BUTTON_COORDY, SHOW_PSEUDO_CODE_DISPLAY_BUTTON_WIDTH, SHOW_PSEUDO_CODE_DISPLAY_BUTTON_HEIGHT);
    showPseudoCodeDisplayButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();
    showPseudoCodeDisplayButton->Click.append([this]()
    {
        pseudoCodeFrame->visible = pseudoCodeDisplay->visible = 1 - pseudoCodeFrame->visible;
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
    animationStep = 0, animationTimer = 1.0f;
    animation_steps.push_back(raywtk::Step(raywtk::StepType::None, -1, -1));
}

void HeapVisualizer::doingStep(raywtk::Step step) 
{
    if(step.type == raywtk::StepType::InsertNode) 
    {
        nodes.push_back(std::move(step.node));
    }
    
    if(step.type == raywtk::StepType::DeleteNode)
    {
        nodes.pop_back();
    }
    
    if(step.type == raywtk::StepType::Swap2Nodes) 
    {
        swap(nodes[step.u]->position, nodes[step.v]->position);
        swap(nodes[step.u], nodes[step.v]);
    }

    if(step.type == raywtk::StepType::Highlight2Nodes)
    {
        nodes[step.u]->highlighted ^= 1;
        nodes[step.v]->highlighted ^= 1;
    }
}

void HeapVisualizer::swapNodes(int i, int j) 
{
    swap(values[i], values[j]);
    animation_steps.push_back(raywtk::Step(raywtk::StepType::Swap2Nodes, i, j));
    //swap(nodes[i]->position, nodes[j]->position);
    //swap(nodes[i], nodes[j]);
}

void HeapVisualizer::maxHeapify(int i) 
{
    if(left_child(i) >= sz(values))
        return;

    int L = left_child(i), R = right_child(i);
    if(L >= sz(values))
        return;

    if(R < sz(values))
    {
        animation_steps.push_back(raywtk::Step(raywtk::StepType::Highlight2Nodes, L, R));
        if(values[L] < values[R])
            L = R;
    }

    animation_steps.push_back(raywtk::Step(raywtk::StepType::Highlight2Nodes, i, L));
    if(values[L] > values[i]) {
        swapNodes(i, L);
        maxHeapify(L);
    }
}

raylib::Vector2 HeapVisualizer::GetPositionInDisplay(int index, int depth) 
{
    while(index >= (1 << depth)) 
    {
        index -= (1 << depth);
        ++depth;
    }
    
    raylib::Vector2 position = raylib::Vector2(WORKING_FRAME_COORDX, WORKING_FRAME_COORDY);
    position.x += 60 + double(WORKING_FRAME_WIDTH - 120) / ((1 << depth)) / 2 + double(WORKING_FRAME_WIDTH - 120) / ((1 << depth)) * (index);
    position.y += (WORKING_FRAME_HEIGHT) / 6 * (depth + 1);
    return position;
}

void HeapVisualizer::BuildHeap(const vector<int> &val) 
{
    ClearHeap();
    animationText.SetText("Building heap with these input values.");
    for (int i = 0; i < min(31, sz(val)); ++i) {
        std::unique_ptr<raywtk::NodeWidget> newNode = std::make_unique<raywtk::NodeWidget>(val[i]);
        newNode->position = GetPositionInDisplay(sz(values), 0);
        values.push_back(val[i]);

        animation_steps.push_back(raywtk::Step(raywtk::StepType::InsertNode, i, -1, std::move(newNode)));
        //nodes.push_back(std::move(newNode));
    }

    int k = parent(sz(values) - 1); // parent of last leaf node on heap
    for (int i = k; i >= 0; --i)
        maxHeapify(i);

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
    animationText.SetText("Pushing value " + to_string(value) + " into heap.");

    std::unique_ptr<raywtk::NodeWidget> newNode = std::make_unique<raywtk::NodeWidget>(value);
    newNode->position = GetPositionInDisplay(sz(values), 0);
    values.push_back(value);

    animation_steps.push_back(raywtk::Step(raywtk::StepType::InsertNode, sz(values) - 1, -1, std::move(newNode)));
    //nodes.push_back(std::move(newNode));

    int i = sz(values) - 1;
    while(i > 0) 
    {
        animation_steps.push_back(raywtk::Step(raywtk::StepType::Highlight2Nodes, i, parent(i)));
        if(values[i] > values[parent(i)]) 
        {
            swapNodes(i, parent(i));
            i = parent(i);
        } else {
            break;
        }
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
    animationText.SetText("Popping the element with maximum value (value = " + to_string(values[0]) + ") out of heap.");
    
    int i = sz(values) - 1;
    animation_steps.push_back(raywtk::Step(raywtk::StepType::Highlight2Nodes, i, 0));
    swapNodes(i, 0);
    
    //notification = std::make_unique<raywtk::Notification>("Pop max value (value = " + std::to_string(values[i]) + ") out of heap successfully.", raywtk::NotificationType::SUCCESS, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
    values.pop_back();
    animation_steps.push_back(raywtk::Step(raywtk::StepType::DeleteNode, -1, -1));
    //nodes.pop_back();
    
    maxHeapify(0);
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
    pushValueButton->enabled = popMaxValueButton->enabled = buildHeapButton->enabled = clearHeapButton->enabled = state;
}

void HeapVisualizer::Update(float dt) 
{
    // Working Frame update
    workingFrame->Update(dt);

    // Pseudo Code Frame update
    pseudoCodeFrame->Update(dt);

    // Notification Frame update
    //notificationFrame->Update(dt);

    // Show operator button update
    showOperatorButton->Update(dt);

    // Build heap button update
    buildHeapButton->Update(dt);

    // Show input box build heap update
    if(inputBuildHeapButtonFlag) 
    {
        if(raylib::Mouse::IsButtonPressed(MOUSE_LEFT_BUTTON) && !raylib::Mouse::GetPosition().CheckCollision(buildHeapButton->buttonRect) && !raylib::Mouse::GetPosition().CheckCollision(inputBoxBuildHeap->rect))
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
    animationTimer += dt;
    if(animationStep >= sz(animation_steps)) {
        animationTimer = 1.0f;
    } else {
        if(animationTimer >= 1.0f) {
            animationTimer -= 1.0f;
            ++animationStep;
            if(animation_steps[animationStep - 1].type == raywtk::StepType::Highlight2Nodes) 
            {
                raywtk::Step tmp;
                tmp.type = animation_steps[animationStep - 1].type;
                tmp.u = animation_steps[animationStep - 1].u;
                tmp.v = animation_steps[animationStep - 1].v;
                tmp.node = std::move(animation_steps[animationStep - 1].node);
                doingStep(raywtk::Step(tmp.type, tmp.u, tmp.v, std::move(tmp.node)));
            }

            if(animationStep < sz(animation_steps)) 
            {
                raywtk::Step tmp;
                tmp.type = animation_steps[animationStep].type;
                tmp.u = animation_steps[animationStep].u;
                tmp.v = animation_steps[animationStep].v;
                tmp.node = std::move(animation_steps[animationStep].node);
                doingStep(raywtk::Step(tmp.type, tmp.u, tmp.v, std::move(tmp.node)));
            } else {
                animationTimer = 1.0f;
                changeStateOperatorButton(true);
                animationText.SetText("No animation is currently on display.");
            }
        }
    }

    // Show pseudo code button update
    showPseudoCodeDisplayButton->Update(dt);

    // vector nodes update
    for(auto &node : nodes)
    {
        node->Update(dt);
    }
}

void HeapVisualizer::Render() 
{
    // title draw
    title.Draw(100, 25);
    
    // notification title draw
    //notificationTitle.Draw(NOTIFICATION_FRAME_COORDX + 10, NOTIFICATION_FRAME_COORDY + 10);

    // Working Frame render
    workingFrame->Render();

    // Pseudo Code Frame render
    pseudoCodeFrame->Render();

    // Notification Frame render
    //notificationFrame->Render();

    // Show operator button render
    showOperatorButton->Render();

    // Build heap button render
    buildHeapButton->Render();

    // Input box push new value button render
    inputBoxPushNewValue->Render();

    // Insert new value button render
    pushValueButton->Render();
    
    // Input box build heap button render
    inputBoxBuildHeap->Render();

    // Pop max value button render
    popMaxValueButton->Render();

    // Clear heap button render
    clearHeapButton->Render();

    // Edges render
    for (int i = 1; i < sz(nodes); ++i) 
    {
        int par_i = parent(i);
        raylib::Vector2 startPos = nodes[par_i]->position;
        raylib::Vector2 endPos = nodes[i]->position;
        startPos.DrawLine(endPos, 2.5f, WHITE);
    }

    // Notification render
    /*if(notification != nullptr)
    {
        notification->Render();
    }*/

    // Animation text render
    animationText.Draw(5, 690);

    // Show pseudo code button render
    showPseudoCodeDisplayButton->Render();

    // Vector nodes render
    for(auto &node : nodes)
    {
        node->Render();
    }
}