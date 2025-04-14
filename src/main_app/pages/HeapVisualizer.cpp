#include "./HeapVisualizer.hpp"
#include "main_app/main_window.hpp"
#include "./main_menu.hpp"
#include "../themes/dark_simple/back_button.hpp"
#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>
#include <cstring>

using namespace ds_viz::pages;

HeapVisualizer::HeapVisualizer() 
{
    // font and title initialize
    font = std::make_unique<raylib::Font>("./ttf/InterDisplay-Black.ttf", 128, nullptr, 250);
    title = raylib::Text("Heap Visualizer", 64, raylib::Color::White(), *font, 0);

    // displayFrame initialize
    displayFrame = std::make_unique<raywtk::DisplayFrame>();
 
    // Push new value initialize
    pushValueButton = std::make_unique<raywtk::Button>();
    pushValueButton->buttonText = "Push New Node";
    pushValueButton->buttonRect = raylib::Rectangle(50, 400, 280, 200);
    pushValueButton->Click.append([this]() { showInputBar = true; });
    pushValueButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Show input bar initialize
    showInputBar = false;
    inputBoxRect = raylib::Rectangle(pushValueButton->buttonRect.x + pushValueButton->buttonRect.width + 10, pushValueButton->buttonRect.y, 150, 50);
    inputBoxText[0] = '\0';
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

void HeapVisualizer::maxHeapify(int i, int n) 
{
    if(i >= n)
        return;

    int L = left_child(i), R = right_child(i);
    int pos_max = i;

    if(L < n && values[L] > values[pos_max])
        pos_max = L;

    if(R < n && values[R] > values[pos_max])
        pos_max = R;

    swap(values[i], values[pos_max]);
    if(pos_max != i)
        maxHeapify(pos_max, n); // recursive call to subtree to heapify for subtree
}

void HeapVisualizer::PushNewValue(int value) 
{
    std::unique_ptr<raywtk::NodeWidget> newNode = std::make_unique<raywtk::NodeWidget>(value);
    raylib::Vector2 randomPosition = raylib::Vector2(WORKING_FRAME_COORDX + rand() % WORKING_FRAME_WIDTH, WORKING_FRAME_COORDY + rand() % WORKING_FRAME_HEIGHT);
    if(randomPosition.x - newNode->radius < WORKING_FRAME_COORDX) randomPosition.x += newNode->radius;
    if(randomPosition.x + newNode->radius > WORKING_FRAME_COORDX + WORKING_FRAME_WIDTH) randomPosition.x -= newNode->radius;
    if(randomPosition.y - newNode->radius < WORKING_FRAME_COORDY) randomPosition.y += newNode->radius;
    if(randomPosition.y + newNode->radius > WORKING_FRAME_COORDY + WORKING_FRAME_HEIGHT) randomPosition.y -= newNode->radius;
    newNode->position = randomPosition;
    nodes.push_back(std::move(newNode));

    #define sz(x) int((x).size())

    values.push_back(value);
    maxHeapify(values[sz(values) - 1], sz(values));
}

void HeapVisualizer::Update(float dt) 
{
    // Display Frame render
    displayFrame->Render();
    
    // insert node button update
    pushValueButton->Update(dt);
    
    // show input bar update
    if (showInputBar)
    {
        int key = GetCharPressed();
        while (key > 0)
        {
            if ((key >= 32) && (key <= 125) && (strlen(inputBoxText) < 127))
            {
                inputBoxText[strlen(inputBoxText) + 1] = '\0';
                inputBoxText[strlen(inputBoxText)] = (char)key;
            }
            key = GetCharPressed();
        }
        
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            if (strlen(inputBoxText) > 0)
            {
                
                inputBoxText[strlen(inputBoxText) - 1] = '\0';
            }
        }
        if (IsKeyPressed(KEY_ENTER)) {
            
            int value = atoi(inputBoxText);
            if(value != 0 || (strlen(inputBoxText) == 1 && inputBoxText[0] == '0'))
            {
                PushNewValue(value);
            }
            
            showInputBar = false;
            inputBoxText[0] = '\0';
        }
    }

    // vector nodes update
    for(auto &node : nodes){
        node->Update(dt);
    }
}

void HeapVisualizer::Render() 
{
    title.Draw(200, 100);

    // Insert new value render
    pushValueButton->Render();
 
    // Show input bar render
    if (showInputBar)
    {
        DrawRectangleRec(inputBoxRect, LIGHTGRAY);
        DrawRectangleLinesEx(inputBoxRect, 2, DARKGRAY);
        DrawText(inputBoxText, inputBoxRect.x + 10, inputBoxRect.y + 10, 20, WHITE);
    }

    // vector nodes render
    for(auto &node : nodes){
        node->Render();
    }
}