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

    // displayFrame initialize
    displayFrame = std::make_unique<raywtk::DisplayFrame>();
 
    // Push new value button initialize
    inputPushNewValueButtonFlag = false;
    pushValueButton = std::make_unique<raywtk::Button>();
    pushValueButton->buttonText = "Push New Value";
    pushValueButton->buttonRect = raylib::Rectangle(75, 250, 250, 170);
    pushValueButton->Click.append([this]() { inputPushNewValueButtonFlag = true; inputBoxPushNewValue->processing = true; });
    pushValueButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();
 
    // Pop max value button initialize
    popMaxValueButton = std::make_unique<raywtk::Button>();
    popMaxValueButton->buttonText = "Pop Max Value";
    popMaxValueButton->buttonRect = raylib::Rectangle(75, 450, 250, 170);
    popMaxValueButton->Click.append([this]() { PopMaxValue(); });
    popMaxValueButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Input box push new value initialize
    inputPushNewValueButtonFlag = false;
    inputBoxPushNewValue = std::make_unique<raywtk::InputBox>(raylib::Rectangle(330, 280, 150, 100), raylib::Color::Black(), raylib::Color::White(), raylib::Color::Gray(), 1, false);
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

void HeapVisualizer::swapNodes(int i, int j) {
    swap(values[i], values[j]);
    swap(nodes[i]->position, nodes[j]->position);
    swap(nodes[i], nodes[j]);
}

#define sz(x) int((x).size())
void HeapVisualizer::PushNewValue(int value) 
{
    int index = sz(values), depth = 0;
    if(index > 31)
        return;

    while(index >= (1 << depth)) {
        index -= (1 << depth);
        ++depth;
    }

    std::unique_ptr<raywtk::NodeWidget> newNode = std::make_unique<raywtk::NodeWidget>(value);
    raylib::Vector2 position = raylib::Vector2(WORKING_FRAME_COORDX, WORKING_FRAME_COORDY);
    position.x += 60 + double(WORKING_FRAME_WIDTH - 120) / ((1 << depth)) / 2 + double(WORKING_FRAME_WIDTH - 120) / ((1 << depth)) * (index);
    position.y += double(WORKING_FRAME_HEIGHT) / 6 * (depth + 1);
    
    newNode->position = position;
    nodes.push_back(std::move(newNode));

    values.push_back(value);
    
    int i = sz(values) - 1;
    while(i > 0) {
        int par_i = parent(i);
        if(values[i] > values[par_i]) {
            swapNodes(i, par_i);
            i = par_i;
        } else {
            break;
        }
    }
}

void HeapVisualizer::PopMaxValue()
{
    if(sz(values) == 0)
        return;

    int i = sz(values) - 1;
    swapNodes(i, 0);
    values.pop_back();
    nodes.pop_back();

    i = 0;
    while(1) {
        int L = left_child(i), R = right_child(i);
        int pos_max = i;

        if(L < sz(values) && values[L] > values[pos_max])
            pos_max = L;

        if(R < sz(values) && values[R] > values[pos_max])
            pos_max = R;

        if(i != pos_max) {
            swapNodes(i, pos_max);
            i = pos_max;
        } else {
            break;
        }
    }

}

void HeapVisualizer::Update(float dt) 
{
    // Display Frame render
    displayFrame->Render();
    
    // Push new value button update
    pushValueButton->Update(dt);

    // Pop max value button update
    popMaxValueButton->Update(dt);
    
    // Show input box push new value update
    if (inputPushNewValueButtonFlag)
    {
        if(raylib::Mouse::IsButtonPressed(MOUSE_LEFT_BUTTON) && !raylib::Mouse::GetPosition().CheckCollision(pushValueButton->buttonRect)) 
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

    // vector nodes update
    for(auto &node : nodes)
    {
        node->Update(dt);
    }
}

void HeapVisualizer::Render() 
{
    title.Draw(200, 100);

    // Insert new value render
    pushValueButton->Render();

    // Pop max value render
    popMaxValueButton->Render();

    // Input box push new value render
    inputBoxPushNewValue->Render();

    // vector nodes render
    for(auto &node : nodes)
    {
        node->Render();
    }
}