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
    title = raylib::Text("Heap Visualizer", 64, raylib::Color::White(), *font, 0);

    // notification title
    notificationTitle = raylib::Text("Notification: ", 40, raylib::Color::Yellow(), *font, 0);

    // workingFrame initialize
    workingFrame = std::make_unique<raywtk::DisplayFrame>(raylib::Rectangle(WORKING_FRAME_COORDX, WORKING_FRAME_COORDY, WORKING_FRAME_WIDTH, WORKING_FRAME_HEIGHT), raylib::Color::Gray(), 5.0f);

    // notificationFrame initialize
    notificationFrame = std::make_unique<raywtk::DisplayFrame>(raylib::Rectangle(NOTIFICATION_FRAME_COORDX, NOTIFICATION_FRAME_COORDY, NOTIFICATION_FRAME_WIDTH, NOTIFICATION_FRAME_HEIGHT), raylib::Color::Gray(), 5.0f);

    // Build heap button initialize
    buildHeapButton = std::make_unique<raywtk::Button>();
    buildHeapButton->buttonText = "Build Heap";
    buildHeapButton->buttonRect = raylib::Rectangle(BUILD_HEAP_BUTTON_COORDX, BUILD_HEAP_BUTTON_COORDY, OPERATOR_BUTTON_WIDTH, OPERATOR_BUTTON_HEIGHT);
    buildHeapButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();
    buildHeapButton->Click.append([this]() 
    { 
        inputBuildHeapButtonFlag = true; inputBoxBuildHeap->processing = true; 
        notification = std::make_unique<raywtk::Notification>("Insert numbers (<= 31 numbers) to build a new heap.", raywtk::NotificationType::INFO, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
    });

    // Push new value button initialize
    pushValueButton = std::make_unique<raywtk::Button>();
    pushValueButton->buttonText = "Push New Value";
    pushValueButton->buttonRect = raylib::Rectangle(PUSH_VALUE_BUTTON_COORDX, PUSH_VALUE_BUTTON_COORDY, OPERATOR_BUTTON_WIDTH, OPERATOR_BUTTON_HEIGHT);
    pushValueButton->Click.append([this]() 
    { 
        inputPushNewValueButtonFlag = true; inputBoxPushNewValue->processing = true; 
        notification = std::make_unique<raywtk::Notification>("Insert a number to push to heap.", raywtk::NotificationType::INFO, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
    });

    pushValueButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();
 
    // Pop max value button initialize
    popMaxValueButton = std::make_unique<raywtk::Button>();
    popMaxValueButton->buttonText = "Pop Max Value";
    popMaxValueButton->buttonRect = raylib::Rectangle(POP_VALUE_BUTTON_COORDX, POP_VALUE_BUTTON_COORDY, OPERATOR_BUTTON_WIDTH, OPERATOR_BUTTON_HEIGHT);
    popMaxValueButton->Click.append([this]() { PopMaxValue(); });
    popMaxValueButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Clear heap button initialize
    clearHeapButton = std::make_unique<raywtk::Button>();
    clearHeapButton->buttonText = "Clear Heap";
    clearHeapButton->buttonRect = raylib::Rectangle(CLEAR_HEAP_BUTTON_COORDX, CLEAR_HEAP_BUTTON_COORDY, OPERATOR_BUTTON_WIDTH, OPERATOR_BUTTON_HEIGHT);
    clearHeapButton->Click.append([this]() { ClearHeap(); });
    clearHeapButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Input box build heap initialize
    inputBuildHeapButtonFlag = false;
    inputBoxBuildHeap = std::make_unique<raywtk::InputBox>(raylib::Rectangle(INPUT_BOX_BUILD_HEAP_COORDX, INPUT_BOX_BUILD_HEAP_COORDY, INPUT_BOX_BUILD_HEAP_WIDTH, INPUT_BOX_BUILD_HEAP_HEIGHT), raylib::Color::Black(), raylib::Color::White(), raylib::Color::Gray(), 31, false);
    
    // Input box push new value initialize
    inputPushNewValueButtonFlag = false;
    inputBoxPushNewValue = std::make_unique<raywtk::InputBox>(raylib::Rectangle(INPUT_BOX_PUSH_VALUE_COORDX, INPUT_BOX_PUSH_VALUE_COORDY, INPUT_BOX_PUSH_VALUE_WIDTH, INPUT_BOX_PUSH_VALUE_HEIGHT), raylib::Color::Black(), raylib::Color::White(), raylib::Color::Gray(), 1, false);
}

int HeapVisualizer::parent(int i)
{
    return (i - 1) / 2;
}

int HeapVisualizer::left_child(int i) 
{
    return i * 2 + 1;
}

int HeapVisualizer::right_child(int i)
{
    return i * 2 + 2;
}

void HeapVisualizer::swapNodes(int i, int j) 
{
    swap(values[i], values[j]);
    swap(nodes[i]->position, nodes[j]->position);
    swap(nodes[i], nodes[j]);
}

void HeapVisualizer::maxHeapify(int i) 
{
    if(i >= sz(values))
        return;

    int L = left_child(i), R = right_child(i);
    int pos_max = i;

    if(L < sz(values) && values[L] > values[pos_max])
        pos_max = L;

    if(R < sz(values) && values[R] > values[pos_max])
        pos_max = R;

    if(i != pos_max) {
        swapNodes(i, pos_max);
        maxHeapify(pos_max);
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
    for (int i = 0; i < sz(val); ++i) {
        std::unique_ptr<raywtk::NodeWidget> newNode = std::make_unique<raywtk::NodeWidget>(val[i]);
        newNode->position = GetPositionInDisplay(sz(values), 0);
        nodes.push_back(std::move(newNode));
        values.push_back(val[i]);
    }

    int k = parent(sz(values) - 1); // parent of last leaf node on heap
    for (int i = k; i >= 0; --i)
        maxHeapify(i);

    notification = std::make_unique<raywtk::Notification>("Build a new heap with these numbers successfully.", raywtk::NotificationType::SUCCESS, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
}

void HeapVisualizer::PushNewValue(int value) 
{
    if(sz(values) >= 31)
    {
        notification = std::make_unique<raywtk::Notification>("Failed to push a value into heap since heap's size reached 31.", raywtk::NotificationType::ERROR, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
        return;
    }

    std::unique_ptr<raywtk::NodeWidget> newNode = std::make_unique<raywtk::NodeWidget>(value);
    newNode->position = GetPositionInDisplay(sz(values), 0);
    nodes.push_back(std::move(newNode));
    values.push_back(value);
    
    int i = sz(values) - 1;
    while(i > 0) 
    {
        int par_i = parent(i);
        if(values[i] > values[par_i]) {
            swapNodes(i, par_i);
            i = par_i;
        } else {
            break;
        }
    }

    notification = std::make_unique<raywtk::Notification>("Push a number with value " + std::to_string(value) + " into heap successfully.", raywtk::NotificationType::SUCCESS, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
}

void HeapVisualizer::PopMaxValue()
{
    if(sz(values) == 0) 
    {
        notification = std::make_unique<raywtk::Notification>("Failed to pop max value out of an empty heap.", raywtk::NotificationType::ERROR, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
        return;
    }

    int i = sz(values) - 1;
    swapNodes(i, 0);
    
    notification = std::make_unique<raywtk::Notification>("Pop max value (value = " + std::to_string(values[i]) + ") out of heap successfully.", raywtk::NotificationType::SUCCESS, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
    values.pop_back();
    nodes.pop_back();
    
    maxHeapify(0);
}

void HeapVisualizer::ClearHeap()
{
    values.clear();
    nodes.clear();
    notification = std::make_unique<raywtk::Notification>("Clear the heap successfully.", raywtk::NotificationType::SUCCESS, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
}

void HeapVisualizer::Update(float dt) 
{
    // Working Frame render
    workingFrame->Update(dt);
    
    // Notification Frame render
    notificationFrame->Update(dt);

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

    // vector nodes update
    for(auto &node : nodes)
    {
        node->Update(dt);
    }
}

void HeapVisualizer::Render() 
{
    // title draw
    title.Draw(200, 100);
    
    // notification title draw
    notificationTitle.Draw(NOTIFICATION_FRAME_COORDX + 10, NOTIFICATION_FRAME_COORDY + 10);

    // Working Frame render
    workingFrame->Render();

    // Notification Frame render
    notificationFrame->Render();

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
    if(notification != nullptr)
    {
        notification->Render();
    }

    // Vector nodes render
    for(auto &node : nodes)
    {
        node->Render();
    }
}