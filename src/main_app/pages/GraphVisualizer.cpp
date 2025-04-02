#include "./GraphVisualizer.hpp"
#include "main_app/main_window.hpp"
#include "./main_menu.hpp"
#include "../themes/dark_simple/back_button.hpp"
#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>
#include <cstring>

using namespace ds_viz::pages;



ds_viz::pages::GraphVisualizer::GraphVisualizer() 
{
    // font and title initialize
    font = std::make_unique<raylib::Font>("./ttf/InterDisplay-Black.ttf", 128, nullptr, 250);
    title = raylib::Text("Graph Visualizer", 64, raylib::Color::White(), *font, 0);
    
    // displayFrame initialize
    displayFrame = std::make_unique<raywtk::DisplayFrame>();

    // Insert new node initialize
    insertNodeButton = std::make_unique<raywtk::Button>();
    insertNodeButton->buttonText = "Insert New Node";
    insertNodeButton->buttonRect = raylib::Rectangle(100, 800, 300, 200);
    insertNodeButton->Click.append([this]() { InsertNewNode(); });
    insertNodeButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Show input bar initialize
    showInputBar = false;
    inputBoxRect = raylib::Rectangle(insertNodeButton->buttonRect.x + insertNodeButton->buttonRect.width + 10, insertNodeButton->buttonRect.y, 150, 50);
    inputBoxText[0] = '\0';
}

void ds_viz::pages::GraphVisualizer::InsertNewNode(){
    int value = 1;
    while(setValue.find(value) != setValue.end()) value++;
    setValue.insert(value);
    std::unique_ptr<raywtk::NodeWidget> newNode = std::make_unique<raywtk::NodeWidget>(value);
    raylib::Vector2 randomPosition = raylib::Vector2(WORKING_FRAME_COORDX + rand() % WORKING_FRAME_WIDTH, WORKING_FRAME_COORDY + rand() % WORKING_FRAME_HEIGHT);
    if(randomPosition.x - newNode->radius < WORKING_FRAME_COORDX) randomPosition.x += newNode->radius;
    if(randomPosition.x + newNode->radius > WORKING_FRAME_COORDX + WORKING_FRAME_WIDTH) randomPosition.x -= newNode->radius;
    if(randomPosition.y - newNode->radius < WORKING_FRAME_COORDY) randomPosition.y += newNode->radius;
    if(randomPosition.y + newNode->radius > WORKING_FRAME_COORDY + WORKING_FRAME_HEIGHT) randomPosition.y -= newNode->radius;
    newNode->position = randomPosition;
    nodes.push_back(std::move(newNode));

}

void ds_viz::pages::GraphVisualizer::Update(float dt)
{
    // Display Frame render
    displayFrame->Render();

    // insert node button update
    insertNodeButton->Update(dt);

    // show input bar update
    if (showInputBar)
    {
        int key = GetCharPressed();
        //int value = 0;
        while (key > 0)
        {
            if ((key >= 32) && (key <= 125) && (strlen(inputBoxText) < 127))
            {
                inputBoxText[strlen(inputBoxText)] = (char)key;
                inputBoxText[strlen(inputBoxText) + 1] = '\0';
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
            int nodeValue = atoi(inputBoxText);
            if(nodeValue != 0 || (strlen(inputBoxText) == 1 && inputBoxText[0] == '0'))
            {
                raylib::Vector2 randomPosition = raylib::Vector2(rand() % 1280, rand() % 720);
                std::unique_ptr<raywtk::NodeWidget> newNode = std::make_unique<raywtk::NodeWidget>(nodeValue);
                newNode->position = randomPosition;
                nodes.push_back(std::move(newNode));
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

void ds_viz::pages::GraphVisualizer::Render() 
{
    title.Draw(200, 100);

    // Insert new node render
    insertNodeButton->Render();

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

