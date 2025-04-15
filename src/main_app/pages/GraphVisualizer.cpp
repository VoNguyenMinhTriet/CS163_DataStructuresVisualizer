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
    
    // workingFrame initialize
    workingFrame = std::make_unique<raywtk::DisplayFrame>(raylib::Rectangle(WORKING_FRAME_COORDX, WORKING_FRAME_COORDY, WORKING_FRAME_WIDTH, WORKING_FRAME_HEIGHT), raylib::Color::Gray(), 5.0f);
    // notificationFrame initialize
    notificationFrame = std::make_unique<raywtk::DisplayFrame>(raylib::Rectangle(NOTIFICATION_FRAME_COORDX, NOTIFICATION_FRAME_COORDY, NOTIFICATION_FRAME_WIDTH, NOTIFICATION_FRAME_HEIGHT), raylib::Color::Gray(), 5.0f);

    // Insert new node button initialize
    insertNodeButton = std::make_unique<raywtk::Button>();
    insertNodeButton->buttonText = "Insert New Node";
    insertNodeButton->buttonRect = raylib::Rectangle(INSERT_NODE_BUTTON_POSX, INSERT_NODE_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    insertNodeButton->Click.append([this]() { InsertNewNode(); });
    insertNodeButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Insert new edge button initialize
    inputInsertEdgeButtonFlag = false;
    insertEdgeButton = std::make_unique<raywtk::Button>();
    insertEdgeButton->buttonText = "Insert New Edge";
    insertEdgeButton->buttonRect = raylib::Rectangle(INSERT_EDGE_BUTTON_POSX, INSERT_EDGE_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    insertEdgeButton->Click.append([this]() { if(inputInsertEdgeButtonFlag == false && inputDeleteNodeButtonFlag == false && inputDeleteEdgeButtonFlag == false){
                                                inputInsertEdgeButtonFlag = true; inputBoxInsertEdge->processing = true;
                                                notification = std::make_unique<raywtk::Notification>("Insert new edge", raywtk::NotificationType::INFO, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
                                            } });
    insertEdgeButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Kruskal initialize
    KruskalButton = std::make_unique<raywtk::Button>();
    KruskalButton->buttonText = "Kruskal";
    KruskalButton->buttonRect = raylib::Rectangle(KRUSKAL_BUTTON_POSX, KRUSKAL_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    KruskalButton->Click.append([this]() { Kruskal(); });
    KruskalButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Delete node button initialize
    inputDeleteNodeButtonFlag = false;
    deleteNodeButton = std::make_unique<raywtk::Button>();
    deleteNodeButton->buttonText = "Delete Node";
    deleteNodeButton->buttonRect = raylib::Rectangle(DELETE_NODE_BUTTON_POSX, DELETE_NODE_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    deleteNodeButton->Click.append([this]() { if(inputInsertEdgeButtonFlag == false && inputDeleteNodeButtonFlag == false && inputDeleteEdgeButtonFlag == false){
                                                inputDeleteNodeButtonFlag = true; inputBoxDeleteNode->processing = true;
                                                notification = std::make_unique<raywtk::Notification>("Delete node", raywtk::NotificationType::INFO, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
                                            } });
    deleteNodeButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Delete edges button initialize
    inputDeleteEdgeButtonFlag = false;
    deleteEdgeButton = std::make_unique<raywtk::Button>();
    deleteEdgeButton->buttonText = "Delete Edge";
    deleteEdgeButton->buttonRect = raylib::Rectangle(DELETE_EDGE_BUTTON_POSX, DELETE_EDGE_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    deleteEdgeButton->Click.append([this]() { if(inputInsertEdgeButtonFlag == false && inputDeleteNodeButtonFlag == false && inputDeleteEdgeButtonFlag == false){
                                                inputDeleteEdgeButtonFlag = true; inputBoxDeleteEdge->processing = true; 
                                                notification = std::make_unique<raywtk::Notification>("Delete edge", raywtk::NotificationType::INFO, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
                                            } });
    deleteEdgeButton->style = std::make_unique<ds_viz::themes::dark_simple::ButtonStyle>();

    // Input box insert new edge initialize
    inputBoxInsertEdge = std::make_unique<raywtk::InputBox>(raylib::Rectangle(INPUT_BOX_INSERT_EDGE_POSX, INPUT_BOX_INSERT_EDGE_POSY, INPUT_BOX_WIDTH, INPUT_BOX_HEIGHT), raylib::Color::Black(), raylib::Color::White(), raylib::Color::Gray(), 3, false);
    // Input box delete node initialize
    inputBoxDeleteNode = std::make_unique<raywtk::InputBox>(raylib::Rectangle(INPUT_BOX_DELETE_NODE_POSX, INPUT_BOX_DELETE_NODE_POSY, INPUT_BOX_WIDTH, INPUT_BOX_HEIGHT), raylib::Color::Black(), raylib::Color::White(), raylib::Color::Gray(), 1, false);
    // Input box delete edge initialize
    inputBoxDeleteEdge = std::make_unique<raywtk::InputBox>(raylib::Rectangle(INPUT_BOX_DELETE_EDGE_POSX, INPUT_BOX_DELETE_EDGE_POSY, INPUT_BOX_WIDTH, INPUT_BOX_HEIGHT), raylib::Color::Black(), raylib::Color::White(), raylib::Color::Gray(), 2, false);

    // Kruskal index processing
    indexProcessing = 0;
    animationStep = 0;
    animationTimer = 0.0;

    kruskalPseudoCode = {
        "1. Sort all edges in non-decreasing order of their weight.",
        "2. Initialize parent and size arrays for union-find.",
        "3. For each edge (u, v, w):",
        "   a. If u and v are in different sets:",
        "      i. Add edge (u, v, w) to MST.",
        "      ii. Union the sets of u and v.",
        "4. Stop when MST contains (N-1) edges."
    };

    pseudoCodeDisplay = std::make_unique<raywtk::PseudoCodeDisplay>(raylib::Vector2(WORKING_FRAME_COORDX + WORKING_FRAME_WIDTH, WORKING_FRAME_COORDY), kruskalPseudoCode.size(), PSEUDOCODE_LINE_WIDTH, PSEUDOCODE_LINE_HEIGHT, raylib::Color::White(), raylib::Color::Yellow(), raylib::Color::Green());    
    pseudoCodeDisplay->SetPseudoCodeLines(kruskalPseudoCode);

}

void ds_viz::pages::GraphVisualizer::InsertNewNode() {
    int value = 0;
    while (setValue.find(value) != setValue.end()) value++;
    setValue.insert(value);
    std::unique_ptr<raywtk::NodeWidget> newNode = std::make_unique<raywtk::NodeWidget>(value);
    raylib::Vector2 randomPosition = raylib::Vector2(WORKING_FRAME_COORDX + rand() % WORKING_FRAME_WIDTH, WORKING_FRAME_COORDY + rand() % WORKING_FRAME_HEIGHT);
    if (randomPosition.x - newNode->radius < WORKING_FRAME_COORDX) randomPosition.x += newNode->radius;
    if (randomPosition.x + newNode->radius > WORKING_FRAME_COORDX + WORKING_FRAME_WIDTH) randomPosition.x -= newNode->radius;
    if (randomPosition.y - newNode->radius < WORKING_FRAME_COORDY) randomPosition.y += newNode->radius;
    if (randomPosition.y + newNode->radius > WORKING_FRAME_COORDY + WORKING_FRAME_HEIGHT) randomPosition.y -= newNode->radius;
    newNode->position = randomPosition;
    nodes.push_back(std::move(newNode));

    // Notification for node addition
    notification = std::make_unique<raywtk::Notification>(
        "Node " + std::to_string(value) + " added successfully",
        raywtk::NotificationType::SUCCESS,
        NOTIFICATION_COORDX,
        NOTIFICATION_COORDY
    );
}

void ds_viz::pages::GraphVisualizer::InsertNewEdge(int u, int v, int c) {
    bool foundU = false, foundV = false;
    for (auto &node : nodes) {
        foundU |= node->value == u;
        foundV |= node->value == v;
    }
    if (foundU && foundV) {
        edges.push_back(std::make_pair(std::make_pair(u, v), c));
        // Notification for edge addition success
        notification = std::make_unique<raywtk::Notification>(
            "Edge (" + std::to_string(u) + ", " + std::to_string(v) + ", " + std::to_string(c) + ") added successfully",
            raywtk::NotificationType::SUCCESS,
            NOTIFICATION_COORDX,
            NOTIFICATION_COORDY
        );
    } else {
        // Notification for edge addition failure
        notification = std::make_unique<raywtk::Notification>(
            "Failed to add edge (" + std::to_string(u) + ", " + std::to_string(v) + ", " + std::to_string(c) + ")",
            raywtk::NotificationType::ERROR,
            NOTIFICATION_COORDX,
            NOTIFICATION_COORDY
        );
    }
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

void ds_viz::pages::GraphVisualizer::DeleteNode(int node) {
    auto it = setValue.find(node);
    if (it != setValue.end()) {
        setValue.erase(it);
        for (size_t i = 0; i < nodes.size(); i++) {
            if (nodes[i]->value == node) {
                nodes.erase(nodes.begin() + i);
                break;
            }
        }
        while (true) {
            bool found = false;
            for (size_t i = 0; i < edges.size(); i++) {
                if (edges[i].first.first == node || edges[i].first.second == node) {
                    edges.erase(edges.begin() + i);
                    found = true;
                    break;
                }
            }
            if (!found) break;
        }
        // Notification for node deletion success
        notification = std::make_unique<raywtk::Notification>(
            "Node " + std::to_string(node) + " deleted successfully",
            raywtk::NotificationType::SUCCESS,
            NOTIFICATION_COORDX,
            NOTIFICATION_COORDY
        );
    } else {
        // Notification for node deletion failure
        notification = std::make_unique<raywtk::Notification>(
            "Failed to delete node " + std::to_string(node),
            raywtk::NotificationType::ERROR,
            NOTIFICATION_COORDX,
            NOTIFICATION_COORDY
        );
    }
}

void ds_viz::pages::GraphVisualizer::DeleteEdge(int u, int v) {
    bool deleted = false;
    for (size_t i = 0; i < edges.size(); i++) {
        if (edges[i].first == std::make_pair(u, v) || edges[i].first == std::make_pair(v, u)) {
            edges.erase(edges.begin() + i);
            deleted = true;
            break;
        }
    }
    if (deleted) {
        // Notification for edge deletion success
        notification = std::make_unique<raywtk::Notification>(
            "Edge (" + std::to_string(u) + ", " + std::to_string(v) + ") deleted successfully",
            raywtk::NotificationType::SUCCESS,
            NOTIFICATION_COORDX,
            NOTIFICATION_COORDY
        );
    } else {
        // Notification for edge deletion failure
        notification = std::make_unique<raywtk::Notification>(
            "Failed to delete edge (" + std::to_string(u) + ", " + std::to_string(v) + ")",
            raywtk::NotificationType::ERROR,
            NOTIFICATION_COORDX,
            NOTIFICATION_COORDY
        );
    }
}

void ds_viz::pages::GraphVisualizer::Update(float dt)
{
    // Display Frame update
    workingFrame->Update(dt);

    // Insert node button update
    insertNodeButton->Update(dt);

    // Insert edge button update
    insertEdgeButton->Update(dt);

    // Input box insert edge button update
    if (inputInsertEdgeButtonFlag && !inputDeleteNodeButtonFlag && !inputDeleteEdgeButtonFlag) {
        inputBoxInsertEdge->Update(dt);
        if (!inputBoxInsertEdge->processing) {
            auto values = inputBoxInsertEdge->values;
            InsertNewEdge(values[0], values[1], values[2]);
            inputInsertEdgeButtonFlag = false;
            inputBoxInsertEdge->Reset();
        }
    }

    // Kruskal button update
    KruskalButton->Update(dt);

    // Kruskal animation update
    // if (kruskalFlag) {
    //     auto findPar = [&](auto self, int u) -> int {
    //         return u == par[u] ? u : par[u] = self(self, par[u]);
    //     };

    //     auto unions = [&](int x, int y) -> bool {
    //         x = findPar(findPar, x);
    //         y = findPar(findPar, y);
    //         if (x == y) return false;
    //         if (sz[x] < sz[y]) std::swap(x, y);
    //         par[y] = x;
    //         sz[x] += sz[y];
    //         return true;
    //     };

    //     animationTimer += dt;
    //     if (animationTimer <= animationDelay) {
    //         if (animationStep < edges.size()) {
    //             indexProcessing = animationStep;
    //             pseudoCodeDisplay->SetLineState(0, raywtk::PseudoCodeDisplay::LineState::HIGHLIGHTED);
    //         }
    //     } else {
    //         if (animationTimer <= 2.0 * animationDelay) {
    //             if (animationStep < edges.size()) {
    //                 int i = animationStep;
    //                 auto [u, v] = edges[i].first;
    //                 // Highlight step 3: Processing edge
    //                 pseudoCodeDisplay->SetLineState(2, raywtk::PseudoCodeDisplay::LineState::HIGHLIGHTED);
    //                 if (unions(u, v)) {
    //                     pseudoCodeDisplay->SetLineState(3, raywtk::PseudoCodeDisplay::LineState::HIGHLIGHTED);
    //                     inMstList.insert(i);
    //                 }
    //             } else {
    //                 kruskalFlag = false;
    //             }
    //         } else {
    //             ++animationStep;
    //             if (animationStep == edges.size()) {
    //                 kruskalFlag = false;
    //             }
    //             animationTimer = 0.0f;
    //         }
    //     }
    // }

    if (kruskalFlag) {
        auto findPar = [&](auto self, int u) -> int {
            return u == par[u] ? u : par[u] = self(self, par[u]);
        };

        auto unions = [&](int x, int y) -> bool {
            x = findPar(findPar, x);
            y = findPar(findPar, y);
            if (x == y) return false;
            if (sz[x] < sz[y]) std::swap(x, y);
            par[y] = x;
            sz[x] += sz[y];
            return true;
        };

        animationTimer += dt;

        if (animationTimer <= animationDelay) {
            // Highlight step 1: Sorting edges
            pseudoCodeDisplay->SetLineState(0, raywtk::PseudoCodeDisplay::LineState::HIGHLIGHTED);
        } else if (animationStep < edges.size()) {
            int i = animationStep;
            auto [u, v] = edges[i].first;

            if (animationTimer <= 2.0 * animationDelay) {
                // Highlight step 3: Processing edge
                pseudoCodeDisplay->SetLineState(2, raywtk::PseudoCodeDisplay::LineState::HIGHLIGHTED);

                if (unions(u, v)) {
                    // Highlight step 3a: Adding edge to MST
                    pseudoCodeDisplay->SetLineState(3, raywtk::PseudoCodeDisplay::LineState::HIGHLIGHTED);
                    inMstList.insert(i);
                }
            } else {
                // Move to the next edge
                ++animationStep;
                if (animationStep == edges.size()) {
                    kruskalFlag = false;
                }
                animationTimer = 0.0f;
            }
        } else {
            // Highlight step 4: MST complete
            pseudoCodeDisplay->SetLineState(4, raywtk::PseudoCodeDisplay::LineState::HIGHLIGHTED);
            kruskalFlag = false;
        }
    }

    // Delete node button update
    deleteNodeButton->Update(dt);

    // Input box delete node button update
    if (inputDeleteNodeButtonFlag && !inputInsertEdgeButtonFlag && !inputDeleteEdgeButtonFlag) {
        inputBoxDeleteNode->Update(dt);
        if (!inputBoxDeleteNode->processing) {
            int nodeToDelete = inputBoxDeleteNode->values[0];
            DeleteNode(nodeToDelete);
            inputDeleteNodeButtonFlag = false;
            inputBoxDeleteNode->Reset();
        }
    }

    // Delete edge button update
    deleteEdgeButton->Update(dt);

    // Input box delete edge button update
    if (inputDeleteEdgeButtonFlag && !inputInsertEdgeButtonFlag && !inputDeleteNodeButtonFlag) {
        inputBoxDeleteEdge->Update(dt);
        if (!inputBoxDeleteEdge->processing) {
            auto values = inputBoxDeleteEdge->values;
            DeleteEdge(values[0], values[1]);
            inputDeleteEdgeButtonFlag = false;
            inputBoxDeleteEdge->Reset();
        }
    }

    // Update all nodes
    for (auto &node : nodes) {
        node->Update(dt);
    }
}

void ds_viz::pages::GraphVisualizer::Render() 
{
    title.Draw(200, 100);

    // display frame render
    workingFrame->Render();

    // notification frame render
    notificationFrame->Render();

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

    // Input box delete node render
    inputBoxDeleteNode->Render();

    // Input box delete edge render
    inputBoxDeleteEdge->Render();

    // Notification render
    if(notification != nullptr){
        notification->Render();
    }

    // Psuedo code display render
    pseudoCodeDisplay->Render();

    // vector nodes render
    for(auto &node : nodes){
        node->Render();
    }
}

