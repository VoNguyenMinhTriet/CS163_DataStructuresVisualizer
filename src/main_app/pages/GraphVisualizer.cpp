#include "./GraphVisualizer.hpp"
#include "main_app/main_window.hpp"
#include "./main_menu.hpp"
#include "../themes/dark_simple/back_button.hpp"
#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>
#include <cstring>
#include <cassert>

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
    insertNodeButton->buttonRect = raylib::Rectangle(100, 500, 300, 200);
    insertNodeButton->Click.append([this]() { InsertNewNode(); });
    insertNodeButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // text input insert edge string initialize
    textInputInsertEdge[0] = '\0';

    // Insert new edge initialize
    inputInsertEdgeButtonFlag = false;
    insertEdgeButton = std::make_unique<raywtk::Button>();
    insertEdgeButton->buttonText = "Insert New Edge";
    insertEdgeButton->buttonRect = raylib::Rectangle(100, 850, 300, 200);
    insertEdgeButton->Click.append([this]() { inputInsertEdgeButtonFlag = true; });
    insertEdgeButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Kruskal initialize
    KruskalButton = std::make_unique<raywtk::Button>();
    KruskalButton->buttonText = "Kruskal";
    KruskalButton->buttonRect = raylib::Rectangle(100, 1200, 300, 200);
    KruskalButton->Click.append([this]() { });
    KruskalButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

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

void ds_viz::pages::GraphVisualizer::InsertNewEdge(int u, int v, int c){
    bool foundU = 0, foundV = 0;
    for(auto &node : nodes){
        foundU |= node->value == u;
        foundV |= node->value == v;
    }
    if(foundU && foundV) edges.push_back(std::make_pair(std::make_pair(u, v), c));
}

void ds_viz::pages::GraphVisualizer::Update(float dt)
{
    // Display Frame update
    displayFrame->Update(dt);

    // insert node button update
    insertNodeButton->Update(dt);

    // insert edge button update
    insertEdgeButton->Update(dt);

    //input box insert edge button update
    if(inputInsertEdgeButtonFlag){
        int x = 400, y = 850, height = 100, weight = 200;
        raylib::Rectangle rect = raylib::Rectangle(x, y, height, weight);
        raylib::Color textColor = raylib::Color::Black(), bgColor = raylib::Color::White(), borderColor = raylib::Color::Gray();
        DrawRectangleRec(rect, bgColor);
        DrawRectangleLinesEx(rect, 2, borderColor);
        bool focused = true;
        int spaceLimit = 2;

        if (raylib::Mouse::IsButtonPressed(MOUSE_LEFT_BUTTON)) {
            focused = raylib::Mouse::GetPosition().CheckCollision(rect);
        }

        if (focused) {
            int key = GetCharPressed();
            while (key > 0) {
                bool getNewChar = false;
                if(key == (int)' '){
                    if(std::count(textInputInsertEdge, textInputInsertEdge + strlen(textInputInsertEdge), ' ') + 1 <= spaceLimit){
                        getNewChar = 1;
                    }
                }
                else{
                    if ((key >= '0') && (key <= '9') && (strlen(textInputInsertEdge) < 127)) {
                        getNewChar = 1;
                    }
                }
                if(getNewChar){
                    int x = strlen(textInputInsertEdge);
                    int k = strlen(textInputInsertEdge);
                    textInputInsertEdge[k] = (char)key;
                    textInputInsertEdge[k + 1] = '\0';
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (strlen(textInputInsertEdge) > 0) {
                    textInputInsertEdge[strlen(textInputInsertEdge) - 1] = '\0';
                }
            }
            if(IsKeyPressed(KEY_ENTER) && std::count(textInputInsertEdge, textInputInsertEdge + strlen(textInputInsertEdge), ' ') == 2){
                inputInsertEdgeButtonFlag = false;
            }
            raylib::DrawText(textInputInsertEdge, (int)(rect.x + 10), (int)(rect.y + 10), 20, textColor);
        }
        if(!inputInsertEdgeButtonFlag){
            int values[3]; for(int i = 0; i < 3; i++) values[i] = 0;
            int cnt = 0;
            for(size_t i = 0; textInputInsertEdge[i] != '\0'; i++){
                if(textInputInsertEdge[i] != ' '){
                    values[cnt] = values[cnt] * 10 + (textInputInsertEdge[i] - '0');
                }
                else ++cnt;
            }
            textInputInsertEdge[0] = '\0';
            InsertNewEdge(values[0], values[1], values[2]);
        }
        

    }

    // Kruskal update
    KruskalButton->Update(dt);
    
    // vector nodes update
    for(auto &node : nodes){
        node->Update(dt);
    }

}

void ds_viz::pages::GraphVisualizer::Render() 
{
    title.Draw(200, 100);

    // display frame render
    displayFrame->Render();

    // Insert new node render
    insertNodeButton->Render();

    // input box insert edge button render
    if(inputInsertEdgeButtonFlag){
        
    }

    // Insert new edge render
    insertEdgeButton->Render();
    
    // vector edges render
    for(size_t i = 0; i < edges.size(); i++){
        raylib::Vector2 startPos, endPos;
        for(auto &node : nodes){
            if(node->value == edges[i].first.first){
                startPos = node->position;
            }
            if(node->value == edges[i].first.second){
                endPos = node->position;
            }
        }
        startPos.DrawLine(endPos, 2.5f, WHITE);
        raylib::Vector2 posText = {(startPos.x + endPos.x) / 2, (startPos.y + endPos.y) / 2};
        std::string weight = std::to_string(edges[i].second);
        raylib::DrawText(weight.c_str(), posText.x, posText.y, 20, WHITE);
    }

    // Kruskal render
    KruskalButton->Render();

    // vector nodes render
    for(auto &node : nodes){
        node->Render();
    }
}

