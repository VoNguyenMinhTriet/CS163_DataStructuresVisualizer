#include "./GraphVisualizer.hpp"
#include "main_app/main_window.hpp"
#include "./main_menu.hpp"
#include "../themes/dark_simple/back_button.hpp"
#include <algorithm>
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
    insertNodeButton->buttonRect = raylib::Rectangle(INSERT_NODE_BUTTON_POSX, INSERT_NODE_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    insertNodeButton->Click.append([this]() { InsertNewNode(); });
    insertNodeButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // text input insert edge string initialize
    textInputInsertEdge[0] = '\0';

    // Insert new edge initialize
    inputInsertEdgeButtonFlag = false;
    insertEdgeButton = std::make_unique<raywtk::Button>();
    insertEdgeButton->buttonText = "Insert New Edge";
    insertEdgeButton->buttonRect = raylib::Rectangle(INSERT_EDGE_BUTTON_POSX, INSERT_EDGE_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    insertEdgeButton->Click.append([this]() { inputInsertEdgeButtonFlag = true; inputBoxInsertEdge->processing = true; });
    insertEdgeButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Input box insert new edge initialize
    inputBoxInsertEdge = std::make_unique<raywtk::InputBox>(raylib::Rectangle(INPUT_BOX_INSERT_EDGE_POSX, INPUT_BOX_INSERT_EDGE_POSY, INPUT_BOX_WIDTH, INPUT_BOX_HEIGHT), raylib::Color::Black(), raylib::Color::White(), raylib::Color::Gray(), 3, false);

    // Kruskal initialize
    KruskalButton = std::make_unique<raywtk::Button>();
    KruskalButton->buttonText = "Kruskal";
    KruskalButton->buttonRect = raylib::Rectangle(KRUSKAL_BUTTON_POSX, KRUSKAL_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    KruskalButton->Click.append([this]() { Kruskal(); });
    KruskalButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Delete node initialize
    inputDeleteNodeButtonFlag = false;
    deleteNodeButton = std::make_unique<raywtk::Button>();
    deleteNodeButton->buttonText = "Delete Node";
    deleteNodeButton->buttonRect = raylib::Rectangle(DELETE_NODE_BUTTON_POSX, DELETE_NODE_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    deleteNodeButton->Click.append([this]() { inputDeleteNodeButtonFlag = true; });
    deleteNodeButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // text input delete node string initialize
    textInputDeleteNode[0] = '\0';

    // Delete node initialize
    inputDeleteEdgeButtonFlag = false;
    deleteEdgeButton = std::make_unique<raywtk::Button>();
    deleteEdgeButton->buttonText = "Delete Edge";
    deleteEdgeButton->buttonRect = raylib::Rectangle(DELETE_EDGE_BUTTON_POSX, DELETE_EDGE_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    deleteEdgeButton->Click.append([this]() { inputDeleteEdgeButtonFlag = true; });
    deleteEdgeButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // text input delete Edge string initialize
    textInputDeleteEdge[0] = '\0';

    // Kruskal index processing
    indexProcessing = 0;
    animationStep = 0;
    animationTimer = 0.0;

}

void ds_viz::pages::GraphVisualizer::InsertNewNode(){
    int value = 0;
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

void ds_viz::pages::GraphVisualizer::Kruskal(){
    std::sort(edges.begin(), edges.end(), [](const std::pair<std::pair<int, int>, int> &x, const std::pair<std::pair<int, int>, int> &y){
        return x.second < y.second;
    });
    par.resize(nodes.size()); sz.resize(nodes.size());
    for(size_t i = 0; i < nodes.size(); i++) par[i] = i, sz[i] = 1;
    kruskalFlag = true;
    inMstList.clear();
    indexProcessing = 0;
    animationStep = 0;
    animationTimer = 0.0;
}

void ds_viz::pages::GraphVisualizer::DeleteNode(int node){
    auto it = setValue.find(node);
    if(it != setValue.end()){
        setValue.erase(it);
        for(size_t i = 0; i < nodes.size(); i++){
            if(nodes[i]->value == node){
                nodes.erase(nodes.begin() + i);
                break;
            }
        }
        while(true){
            bool found = 0;
            for(size_t i = 0; i < edges.size(); i++){
                if(edges[i].first.first == node || edges[i].first.second == node){
                    edges.erase(edges.begin() + i);
                    found = 1;
                    break;
                }
            }
            if(!found) break;
        }
    }
}

void ds_viz::pages::GraphVisualizer::DeleteEdge(int u, int v){
    for(size_t i = 0; i < edges.size(); i++){
        if(edges[i].first == std::make_pair(u, v) || edges[i].first == std::make_pair(v, u)){
            edges.erase(edges.begin() + i);
            break;
        }
    }
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
        // int x = INSERT_EDGE_BUTTON_POSX + BUTTON_WIDTH, y = INSERT_EDGE_BUTTON_POSY, height = 100, width = 200;
        // raylib::Rectangle rect = raylib::Rectangle(x, y, height, width);
        // raylib::Color textColor = raylib::Color::Black(), bgColor = raylib::Color::White(), borderColor = raylib::Color::Gray();
        // DrawRectangleRec(rect, bgColor);
        // DrawRectangleLinesEx(rect, 2, borderColor);
        // bool focused = true;
        // int spaceLimit = 2;

        // if (raylib::Mouse::IsButtonPressed(MOUSE_LEFT_BUTTON)) {
        //     focused = raylib::Mouse::GetPosition().CheckCollision(rect);
        // }

        // if (focused) {
        //     int key = GetCharPressed();
        //     while (key > 0) {
        //         bool getNewChar = false;
        //         if(key == (int)' '){
        //             if(std::count(textInputInsertEdge, textInputInsertEdge + strlen(textInputInsertEdge), ' ') + 1 <= spaceLimit){
        //                 getNewChar = 1;
        //             }
        //         }
        //         else{
        //             if ((key >= '0') && (key <= '9') && (strlen(textInputInsertEdge) < 127)) {
        //                 getNewChar = 1;
        //             }
        //         }
        //         if(getNewChar){
        //             int x = strlen(textInputInsertEdge);
        //             int k = strlen(textInputInsertEdge);
        //             textInputInsertEdge[k] = (char)key;
        //             textInputInsertEdge[k + 1] = '\0';
        //         }
        //         key = GetCharPressed();
        //     }

        //     if (IsKeyPressed(KEY_BACKSPACE)) {
        //         if (strlen(textInputInsertEdge) > 0) {
        //             textInputInsertEdge[strlen(textInputInsertEdge) - 1] = '\0';
        //         }
        //     }
        //     if(IsKeyPressed(KEY_ENTER) && std::count(textInputInsertEdge, textInputInsertEdge + strlen(textInputInsertEdge), ' ') == 2){
        //         inputInsertEdgeButtonFlag = false;
        //     }
        //     raylib::DrawText(textInputInsertEdge, (int)(rect.x + 10), (int)(rect.y + 10), 20, textColor);
        // }
        inputBoxInsertEdge->Update(dt);
        if(!inputBoxInsertEdge->processing){
            auto values = inputBoxInsertEdge->values;
            InsertNewEdge(values[0], values[1], values[2]);
            inputInsertEdgeButtonFlag = false;
            inputBoxInsertEdge->Reset();
        }
        // if(!inputInsertEdgeButtonFlag){
        //     int values[3]; for(int i = 0; i < 3; i++) values[i] = 0;
        //     int cnt = 0;
        //     for(size_t i = 0; textInputInsertEdge[i] != '\0'; i++){
        //         if(textInputInsertEdge[i] != ' '){
        //             values[cnt] = values[cnt] * 10 + (textInputInsertEdge[i] - '0');
        //         }
        //         else ++cnt;
        //     }
        //     textInputInsertEdge[0] = '\0';
        //     InsertNewEdge(values[0], values[1], values[2]);
        // }
        

    }

    // Kruskal update
    KruskalButton->Update(dt);

    if(kruskalFlag){
        auto findPar = [&](auto self, int u) -> int {
            return u == par[u] ? u : par[u] = self(self, par[u]);
        };
    
        auto unions = [&](int x, int y) -> bool {
            x = findPar(findPar, x);
            y = findPar(findPar, y);
            if (x == y) return false;
            if(sz[x] < sz[y]) std::swap(x, y);
            par[y] = x;
            sz[x] += sz[y];
            return true;
        };
        
        animationTimer += dt;
        if(animationTimer <= animationDelay){
            if(animationStep < edges.size()){
                indexProcessing = animationStep;
            }
        } else{
            if(animationTimer <= 2.0 * animationDelay){
                if(animationStep < edges.size()){
                    int i = animationStep;
                    auto [u, v] = edges[i].first;
                    if(unions(u, v)){
                        inMstList.insert(i);
                    }
                } else{
                    kruskalFlag = false;
                }
            } else{
                ++animationStep;
                if(animationStep == edges.size()){
                    kruskalFlag = false;
                }
                animationTimer = 0.0f;
            }
        }
    }

    deleteNodeButton->Update(dt);

    //input box delete node button update
    if(inputDeleteNodeButtonFlag){
        int x = DELETE_NODE_BUTTON_POSX + BUTTON_WIDTH, y = DELETE_NODE_BUTTON_POSY, height = 100, width = 200;
        raylib::Rectangle rect = raylib::Rectangle(x, y, height, width);
        raylib::Color textColor = raylib::Color::Black(), bgColor = raylib::Color::White(), borderColor = raylib::Color::Gray();
        DrawRectangleRec(rect, bgColor);
        DrawRectangleLinesEx(rect, 2, borderColor);
        bool focused = true;
        int spaceLimit = 2;

        if (raylib::Mouse::IsButtonDown(MOUSE_LEFT_BUTTON)) {
            focused = raylib::Mouse::GetPosition().CheckCollision(rect);
        }

        if (focused) {
            int key = GetCharPressed();
            while (key > 0) {
                bool getNewChar = false;
                if ((key >= '0') && (key <= '9') && (strlen(textInputDeleteNode) < 127)) {
                    getNewChar = 1;
                }
                if(getNewChar){
                    int x = strlen(textInputDeleteNode);
                    int k = strlen(textInputDeleteNode);
                    textInputDeleteNode[k] = (char)key;
                    textInputDeleteNode[k + 1] = '\0';
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (strlen(textInputDeleteNode) > 0) {
                    textInputDeleteNode[strlen(textInputDeleteNode) - 1] = '\0';
                }
            }
            if(IsKeyPressed(KEY_ENTER)){
                inputDeleteNodeButtonFlag = false;
            }
            raylib::DrawText(textInputDeleteNode, (int)(rect.x + 10), (int)(rect.y + 10), 20, textColor);
        }
        if(!inputDeleteNodeButtonFlag){
            int nodeToDelete = 0;
            for(size_t i = 0; textInputDeleteNode[i] != '\0'; i++){
                nodeToDelete = nodeToDelete * 10 + textInputDeleteNode[i] - '0';
            }
            textInputDeleteNode[0] = '\0';
            DeleteNode(nodeToDelete);
        }
        

    }

    deleteEdgeButton->Update(dt);

    // input box delete edge button update
    if(inputDeleteEdgeButtonFlag){
        int x = DELETE_EDGE_BUTTON_POSX + BUTTON_WIDTH, y = DELETE_EDGE_BUTTON_POSY, height = 100, width = 200;
        raylib::Rectangle rect = raylib::Rectangle(x, y, height, width);
        raylib::Color textColor = raylib::Color::Black(), bgColor = raylib::Color::White(), borderColor = raylib::Color::Gray();
        DrawRectangleRec(rect, bgColor);
        DrawRectangleLinesEx(rect, 2, borderColor);
        bool focused = true;
        int spaceLimit = 1;

        if (raylib::Mouse::IsButtonPressed(MOUSE_LEFT_BUTTON)) {
            focused = raylib::Mouse::GetPosition().CheckCollision(rect);
        }

        if (focused) {
            int key = GetCharPressed();
            while (key > 0) {
                bool getNewChar = false;
                if(key == (int)' '){
                    if(std::count(textInputDeleteEdge, textInputDeleteEdge + strlen(textInputDeleteEdge), ' ') + 1 <= spaceLimit){
                        getNewChar = 1;
                    }
                }
                else{
                    if ((key >= '0') && (key <= '9') && (strlen(textInputDeleteEdge) < 127)) {
                        getNewChar = 1;
                    }
                }
                if(getNewChar){
                    int x = strlen(textInputDeleteEdge);
                    int k = strlen(textInputDeleteEdge);
                    textInputDeleteEdge[k] = (char)key;
                    textInputDeleteEdge[k + 1] = '\0';
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (strlen(textInputDeleteEdge) > 0) {
                    textInputDeleteEdge[strlen(textInputDeleteEdge) - 1] = '\0';
                }
            }
            if(IsKeyPressed(KEY_ENTER) && std::count(textInputDeleteEdge, textInputDeleteEdge + strlen(textInputDeleteEdge), ' ') == spaceLimit){
                inputDeleteEdgeButtonFlag = false;
            }
            raylib::DrawText(textInputDeleteEdge, (int)(rect.x + 10), (int)(rect.y + 10), 20, textColor);
        }
        if(!inputDeleteEdgeButtonFlag){
            int values[2]; for(int i = 0; i < 2; i++) values[i] = 0;
            int cnt = 0;
            for(size_t i = 0; textInputDeleteEdge[i] != '\0'; i++){
                if(textInputDeleteEdge[i] != ' '){
                    values[cnt] = values[cnt] * 10 + (textInputDeleteEdge[i] - '0');
                }
                else ++cnt;
            }
            textInputDeleteEdge[0] = '\0';
            DeleteEdge(values[0], values[1]);
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
    if(!kruskalFlag){
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
    } else{
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
            raylib::Color edgeColor = WHITE;
            if(i == indexProcessing) edgeColor = YELLOW;
            else if(inMstList.find(i) != inMstList.end()) edgeColor = RED;
            startPos.DrawLine(endPos, 2.5f, edgeColor);
            raylib::Vector2 posText = {(startPos.x + endPos.x) / 2, (startPos.y + endPos.y) / 2};
            std::string weight = std::to_string(edges[i].second);
            raylib::DrawText(weight.c_str(), posText.x, posText.y, 20, edgeColor);
        }
    }

    // Kruskal render
    KruskalButton->Render();

    // Delete node render
    deleteNodeButton->Render();

    // Delete edge render
    deleteEdgeButton->Render();

    // Input box insert new edge render
    inputBoxInsertEdge->Render();

    // vector nodes render
    for(auto &node : nodes){
        node->Render();
    }
}

