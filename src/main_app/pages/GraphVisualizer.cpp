#include "./GraphVisualizer.hpp"
#include "main_app/main_window.hpp"
#include "./main_menu.hpp"
#include "./page.hpp"
#include <algorithm>
#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>
#include <cstring>
#include <cassert>

using namespace ds_viz::pages;

const int ds_viz::pages::GraphVisualizer::PSEUDOCODE_LINE_WIDTH;
const int ds_viz::pages::GraphVisualizer::PSEUDOCODE_LINE_HEIGHT;
const int ds_viz::pages::GraphVisualizer::NOTIFICATION_COORDX;
const int ds_viz::pages::GraphVisualizer::NOTIFICATION_COORDY;


ds_viz::pages::GraphVisualizer::GraphVisualizer(ds_viz::MainWindow &context) : ds_viz::Page(context) 
{
    // font and title initialize
    font = std::make_unique<raylib::Font>("./ttf/InterDisplay-Black.ttf", 128, nullptr, 250);
    // title = raylib::Text("Graph Visualizer", 100, raylib::Color(255, 255, 255, 128), *font, 0);
    title = raylib::Text("", 100, raylib::Color(255, 255, 255, 128), *font, 0);
    
    // workingFrame initialize
    workingFrame = std::make_unique<raywtk::DisplayFrame>(raylib::Rectangle(WORKING_FRAME_COORDX, WORKING_FRAME_COORDY, WORKING_FRAME_WIDTH, WORKING_FRAME_HEIGHT), raylib::Color::Gray(), 5.0f);
    // notificationFrame initialize
    notificationFrame = std::make_unique<raywtk::DisplayFrame>(raylib::Rectangle(NOTIFICATION_FRAME_COORDX, NOTIFICATION_FRAME_COORDY, NOTIFICATION_FRAME_WIDTH, NOTIFICATION_FRAME_HEIGHT), raylib::Color::Gray(), 5.0f);

    // Main menu button initialize
    mainMenuButton = std::make_unique<raywtk::GraphButton>();
    mainMenuButton->buttonText = "";
    mainMenuButton->buttonRect = raylib::Rectangle(MAIN_MENU_BUTTON_POSX, MAIN_MENU_BUTTON_POSY, MAIN_MENU_BUTTON_WIDTH, MAIN_MENU_BUTTON_HEIGHT);
    mainMenuButton->Click.append([this]() { _context->ChangePage(std::make_shared<ds_viz::pages::MainMenuPage>(*_context)); });
    mainMenuButton->style = std::make_unique<ds_viz::themes::dark_simple::GraphButtonStyle>();

    // Load file button initialize
    loadFileButton = std::make_unique<raywtk::GraphButton>();
    loadFileButton->buttonText = "Load File";
    loadFileButton->buttonRect = raylib::Rectangle(LOAD_FILE_BUTTON_POSX, LOAD_FILE_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    loadFileButton->Click.append([this]() { if (freeFlag) {  notification = std::make_unique<raywtk::Notification>(
                "Please drag and drop the file here. The file format\n" 
                "should be:\n"
                "1. First two lines: two numbers n and m (number of\n"
                "vertices and edges).\n"
                "2. Next m lines: (u, v, w) representing edges with\n"
                "weights.",
                raywtk::NotificationType::INFO, NOTIFICATION_COORDX, NOTIFICATION_COORDY ); waitingForFile = true; }});
    loadFileButton->style = std::make_unique<ds_viz::themes::dark_simple::GraphButtonStyle>();

    // Initialize graph button and input box
    initializeGraphButton = std::make_unique<raywtk::GraphButton>();
    initializeGraphButton->buttonText = "Initialize Graph";
    initializeGraphButton->buttonRect = raylib::Rectangle(INIT_GRAPH_BUTTON_POSX, INIT_GRAPH_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    initializeGraphButton->Click.append([this]() { if (freeFlag) {
                                                    inputInitializeGraphFlag = true; inputBoxInitializeGraph->processing = true; freeFlag = false;
                                                    notification = std::make_unique<raywtk::Notification>("Enter the number of nodes to initialize the graph", raywtk::NotificationType::INFO, NOTIFICATION_COORDX, NOTIFICATION_COORDY ); }
                                                });
    initializeGraphButton->style = std::make_unique<ds_viz::themes::dark_simple::GraphButtonStyle>();

    // Insert new node button initialize
    insertNodeButton = std::make_unique<raywtk::GraphButton>();
    insertNodeButton->buttonText = "Insert New Node";
    insertNodeButton->buttonRect = raylib::Rectangle(INSERT_NODE_BUTTON_POSX, INSERT_NODE_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    insertNodeButton->Click.append([this]() { if(freeFlag) InsertNewNode(); });
    insertNodeButton->style = std::make_unique<ds_viz::themes::dark_simple::GraphButtonStyle>();

    // Insert new edge button initialize
    inputInsertEdgeButtonFlag = false;
    insertEdgeButton = std::make_unique<raywtk::GraphButton>();
    insertEdgeButton->buttonText = "Insert New Edge";
    insertEdgeButton->buttonRect = raylib::Rectangle(INSERT_EDGE_BUTTON_POSX, INSERT_EDGE_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    insertEdgeButton->Click.append([this]() { if(freeFlag){
                                                inputInsertEdgeButtonFlag = true; inputBoxInsertEdge->processing = true; freeFlag = false;
                                                notification = std::make_unique<raywtk::Notification>("Insert new edge", raywtk::NotificationType::INFO, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
                                            } });
    insertEdgeButton->style = std::make_unique<ds_viz::themes::dark_simple::GraphButtonStyle>();

    // Kruskal initialize
    KruskalButton = std::make_unique<raywtk::GraphButton>();
    KruskalButton->buttonText = "Kruskal";
    KruskalButton->buttonRect = raylib::Rectangle(KRUSKAL_BUTTON_POSX, KRUSKAL_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    KruskalButton->Click.append([this]() { if(freeFlag){ freeFlag = false; Kruskal(); } });
    KruskalButton->style = std::make_unique<ds_viz::themes::dark_simple::GraphButtonStyle>();

    // Clear graph button initialize
    clearGraphButton = std::make_unique<raywtk::GraphButton>();
    clearGraphButton->buttonText = "Clear Graph";
    clearGraphButton->buttonRect = raylib::Rectangle(CLEAR_GRAPH_BUTTON_POSX, CLEAR_GRAPH_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    clearGraphButton->Click.append([this]() { if (freeFlag) { ClearGraph(); }});
    clearGraphButton->style = std::make_unique<ds_viz::themes::dark_simple::GraphButtonStyle>();

    // Delete node button initialize
    inputDeleteNodeButtonFlag = false;
    deleteNodeButton = std::make_unique<raywtk::GraphButton>();
    deleteNodeButton->buttonText = "Delete Node";
    deleteNodeButton->buttonRect = raylib::Rectangle(DELETE_NODE_BUTTON_POSX, DELETE_NODE_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    deleteNodeButton->Click.append([this]() { if(freeFlag){
                                                inputDeleteNodeButtonFlag = true; inputBoxDeleteNode->processing = true; freeFlag = false;
                                                notification = std::make_unique<raywtk::Notification>("Delete node", raywtk::NotificationType::INFO, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
                                            } });
    deleteNodeButton->style = std::make_unique<ds_viz::themes::dark_simple::GraphButtonStyle>();

    // Delete edges button initialize
    inputDeleteEdgeButtonFlag = false;
    deleteEdgeButton = std::make_unique<raywtk::GraphButton>();
    deleteEdgeButton->buttonText = "Delete Edge";
    deleteEdgeButton->buttonRect = raylib::Rectangle(DELETE_EDGE_BUTTON_POSX, DELETE_EDGE_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    deleteEdgeButton->Click.append([this]() { if(freeFlag){
                                                inputDeleteEdgeButtonFlag = true; inputBoxDeleteEdge->processing = true; freeFlag = false;
                                                notification = std::make_unique<raywtk::Notification>("Delete edge", raywtk::NotificationType::INFO, NOTIFICATION_COORDX, NOTIFICATION_COORDY);
                                            } });
    deleteEdgeButton->style = std::make_unique<ds_viz::themes::dark_simple::GraphButtonStyle>();

    // Undo button initialize
    undoButton = std::make_unique<raywtk::GraphButton>();
    undoButton->buttonText = "Undo";
    undoButton->buttonRect = raylib::Rectangle(UNDO_BUTTON_POSX, UNDO_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    undoButton->Click.append([this]() { if(freeFlag) Undo(); });
    undoButton->style = std::make_unique<ds_viz::themes::dark_simple::GraphButtonStyle>();

    // Redo button initialize
    redoButton = std::make_unique<raywtk::GraphButton>();
    redoButton->buttonText = "Redo";
    redoButton->buttonRect = raylib::Rectangle(REDO_BUTTON_POSX, REDO_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    redoButton->Click.append([this]() { if(freeFlag) Redo(); });
    redoButton->style = std::make_unique<ds_viz::themes::dark_simple::GraphButtonStyle>();

    // Adjust speed button initialize
    adjustSpeedButton = std::make_unique<raywtk::GraphButton>();
    adjustSpeedButton->buttonText = "Adjust Speed";
    adjustSpeedButton->buttonRect = raylib::Rectangle(ADJUST_SPEED_BUTTON_POSX, ADJUST_SPEED_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    adjustSpeedButton->Click.append([this]() { if (freeFlag || kruskalFlag) { 
                                                    inputAdjustSpeedFlag = true; inputBoxAdjustSpeed->processing = true; freeFlag = false; 
                                                    notification = std::make_unique<raywtk::Notification>( "Enter the number of steps per second for the\n"
                                                                                                            "Kruskal animation.", raywtk::NotificationType::INFO, NOTIFICATION_COORDX, NOTIFICATION_COORDY );
                                                }});
    adjustSpeedButton->style = std::make_unique<ds_viz::themes::dark_simple::GraphButtonStyle>();

    // Toggle button initialize
    toggleButton = std::make_unique<raywtk::GraphButton>();
    toggleButton->buttonText = ">";
    toggleButton->buttonRect = raylib::Rectangle(TOGGLE_BUTTON_POSX, TOGGLE_BUTTON_POSY, TOGGLE_BUTTON_WIDTH, TOGGLE_BUTTON_HEIGHT);
    toggleButton->Click.append([this]() { showOperatorButtons = !showOperatorButtons; // Toggle visibility
                                          toggleButton->buttonText = showOperatorButtons ? ">" : "<"; // Update button text
                                        });
    toggleButton->style = std::make_unique<ds_viz::themes::dark_simple::GraphButtonStyle>();

    // Toggle button for pseudo-code box
    pseudoCodeToggleButton = std::make_unique<raywtk::GraphButton>();
    pseudoCodeToggleButton->buttonText = "<";
    pseudoCodeToggleButton->buttonRect = raylib::Rectangle(PSEUDO_CODE_TOGGLE_BUTTON_POSX, PSEUDO_CODE_TOGGLE_BUTTON_POSY, PSEUDO_CODE_TOGGLE_BUTTON_WIDTH, PSEUDO_CODE_TOGGLE_BUTTON_HEIGHT);
    pseudoCodeToggleButton->Click.append([this]() { showPseudoCodeBox = !showPseudoCodeBox; // Toggle visibility
                                                    pseudoCodeToggleButton->buttonText = showPseudoCodeBox ? "<" : ">"; // Update button text
                                                    }); 
    pseudoCodeToggleButton->style = std::make_unique<ds_viz::themes::dark_simple::GraphButtonStyle>();

    // Previous Step Button
    prevStepButton = std::make_unique<raywtk::GraphButton>();
    prevStepButton->buttonText = "Previous";
    prevStepButton->buttonRect = raylib::Rectangle(PREV_STEP_BUTTON_POSX, PREV_STEP_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    prevStepButton->Click.append([this]() { if (currentAnimationStep > 0) { currentAnimationStep--; animationRunning = false; } });
    prevStepButton->style = std::make_unique<ds_viz::themes::dark_simple::GraphButtonStyle>();

    // Next Step Button
    nextStepButton = std::make_unique<raywtk::GraphButton>();
    nextStepButton->buttonText = "Next";
    nextStepButton->buttonRect = raylib::Rectangle(NEXT_STEP_BUTTON_POSX, NEXT_STEP_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    nextStepButton->Click.append([this]() { if (currentAnimationStep < totalSteps - 1) { currentAnimationStep++; animationRunning = false; }});
    nextStepButton->style = std::make_unique<ds_viz::themes::dark_simple::GraphButtonStyle>();

    // Continue Button
    continueButton = std::make_unique<raywtk::GraphButton>();
    continueButton->buttonText = "Continue";
    continueButton->buttonRect = raylib::Rectangle(CONTINUE_BUTTON_POSX, CONTINUE_BUTTON_POSY, BUTTON_WIDTH, BUTTON_HEIGHT);
    continueButton->Click.append([this]() { animationRunning = true; });
    continueButton->style = std::make_unique<ds_viz::themes::dark_simple::GraphButtonStyle>();

    // Input box initialize graph
    inputBoxInitializeGraph = std::make_unique<raywtk::InputBox>(raylib::Rectangle(INPUT_BOX_INIT_GRAPH_POSX, INPUT_BOX_INIT_GRAPH_POSY, INPUT_BOX_WIDTH, INPUT_BOX_HEIGHT), raylib::Color::White(), raylib::Color::Black(), raylib::Color::Pink(), 1, false, false);
    // Input box insert new edge initialize
    inputBoxInsertEdge = std::make_unique<raywtk::InputBox>(raylib::Rectangle(INPUT_BOX_INSERT_EDGE_POSX, INPUT_BOX_INSERT_EDGE_POSY, INPUT_BOX_WIDTH, INPUT_BOX_HEIGHT), raylib::Color::White(), raylib::Color::Black(), raylib::Color::Pink(), 3, false, false);
    // Input box delete node initialize
    inputBoxDeleteNode = std::make_unique<raywtk::InputBox>(raylib::Rectangle(INPUT_BOX_DELETE_NODE_POSX, INPUT_BOX_DELETE_NODE_POSY, INPUT_BOX_WIDTH, INPUT_BOX_HEIGHT), raylib::Color::White(), raylib::Color::Black(), raylib::Color::Pink(), 1, false, false);
    // Input box delete edge initialize
    inputBoxDeleteEdge = std::make_unique<raywtk::InputBox>(raylib::Rectangle(INPUT_BOX_DELETE_EDGE_POSX, INPUT_BOX_DELETE_EDGE_POSY, INPUT_BOX_WIDTH, INPUT_BOX_HEIGHT), raylib::Color::White(), raylib::Color::Black(), raylib::Color::Pink(), 2, false, false);
    // Input box for adjusting speed
    inputBoxAdjustSpeed = std::make_unique<raywtk::InputBox>(raylib::Rectangle(INPUT_BOX_ADJUST_SPEED_POSX, INPUT_BOX_ADJUST_SPEED_POSY, INPUT_BOX_WIDTH, INPUT_BOX_HEIGHT), raylib::Color::White(), raylib::Color::Black(), raylib::Color::Pink(), 1, false, true);

    // Kruskal index processing
    indexProcessing = 0;
    animationTimer = 0.0;

    kruskalPseudoCode = {
        "   For each edge (u, v, w) in increasing order of weight:",
        "   If (u, v, w) does not form a cycle in MST:",
        "      Add edge (u, v, w) to MST.",
        "   else: ignore edge (u, v, w).",
        "   Stop when MST contains (N-1) edges."
    };

    pseudoCodeDisplay = std::make_unique<raywtk::PseudoCodeDisplay>(raylib::Vector2(PSEUDO_CODE_POS_X, PSEUDO_CODE_POS_Y), kruskalPseudoCode.size(), PSEUDOCODE_LINE_WIDTH, PSEUDOCODE_LINE_HEIGHT, raylib::Color::White(), raylib::Color::Yellow(), raylib::Color::Green());    
    pseudoCodeDisplay->SetPseudoCodeLines(kruskalPseudoCode);

}

void ds_viz::pages::GraphVisualizer::LoadGraphFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        notification = std::make_unique<raywtk::Notification>(
            "Failed to open the file.",
            raywtk::NotificationType::ERROR,
            NOTIFICATION_COORDX,
            NOTIFICATION_COORDY
        );
        return;
    }

    int n, m;
    file >> n >> m;

    if (n <= 0 || m < 0) {
        notification = std::make_unique<raywtk::Notification>(
            "Invalid file format: n and m must be positive integers.",
            raywtk::NotificationType::ERROR,
            NOTIFICATION_COORDX,
            NOTIFICATION_COORDY
        );
        return;
    }

    // Clear the existing graph
    ClearGraph();

    // Create nodes
    for (int i = 0; i < n; ++i) {
        auto newNode = std::make_unique<raywtk::GraphNode>(i);
        newNode->position = raylib::Vector2(
            WORKING_FRAME_COORDX + rand() % WORKING_FRAME_WIDTH,
            WORKING_FRAME_COORDY + rand() % WORKING_FRAME_HEIGHT
        );
        nodes.push_back(std::move(newNode));
    }

    // Create edges
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        file >> u >> v >> w;
        if (u < 0 || u >= n || v < 0 || v >= n) {
            notification = std::make_unique<raywtk::Notification>(
                "Invalid edge in file: vertex indices out of range.",
                raywtk::NotificationType::ERROR,
                NOTIFICATION_COORDX,
                NOTIFICATION_COORDY
            );
            return;
        }
        edges.push_back({{u, v}, w});
    }

    notification = std::make_unique<raywtk::Notification>(
        "Graph loaded successfully from file.",
        raywtk::NotificationType::SUCCESS,
        NOTIFICATION_COORDX,
        NOTIFICATION_COORDY
    );
    waitingForFile = false; // Reset the flag
}

void ds_viz::pages::GraphVisualizer::InitializeGraph(int n) {
    PushToUndoStack(); // Save the current state before modifying

    // Clear existing nodes and edges
    nodes.clear();
    edges.clear();

    // Create `n` nodes with random positions
    auto getRandomInt = [](int minValue, int maxValue) -> int {
        std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
        return rng() % (maxValue - minValue + 1) + minValue;
    };

    for (int i = 0; i < n; ++i) {
        auto newNode = std::make_unique<raywtk::GraphNode>(i);
        raylib::Vector2 randomPosition = raylib::Vector2(
            getRandomInt(WORKING_FRAME_COORDX, WORKING_FRAME_COORDX + WORKING_FRAME_WIDTH),
            getRandomInt(WORKING_FRAME_COORDY, WORKING_FRAME_COORDY + WORKING_FRAME_HEIGHT)
        );
        newNode->position = randomPosition;
        nodes.push_back(std::move(newNode));
    }

    // Create random edges between nodes
    int maxEdges = n * (n - 1) / 4; // Limit the number of edges to avoid clutter
    for (int i = 0; i < maxEdges; ++i) {
        int u = getRandomInt(0, n - 1);
        int v = getRandomInt(0, n - 1);
        if (u != v && std::none_of(edges.begin(), edges.end(), [u, v](const auto& edge) {
                return edge.first == std::make_pair(u, v) || edge.first == std::make_pair(v, u);
            })) {
            int weight = getRandomInt(1, 100); // Random weight between 1 and 100
            edges.push_back({{u, v}, weight});
        }
    }

    // Notification for successful initialization
    notification = std::make_unique<raywtk::Notification>(
        "Graph initialized with " + std::to_string(n) + " nodes and " + std::to_string(edges.size()) + " edges",
        raywtk::NotificationType::SUCCESS,
        NOTIFICATION_COORDX,
        NOTIFICATION_COORDY
    );
}

void ds_viz::pages::GraphVisualizer::InsertNewNode() {
    PushToUndoStack(); // Save the current state before modifying
    
    int value = -1;
    while(true){
        bool found = false;
        ++value;
        for(auto &nodes : nodes){
            if(nodes->value == value) found = true;
        }
        if(!found) break;
    }
    auto getRandomInt = [&](int minValue, int maxValue) -> int {
        std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
        return rng() % (maxValue - minValue + 1) + minValue;
    };
    std::unique_ptr<raywtk::GraphNode> newNode = std::make_unique<raywtk::GraphNode>(value);
    raylib::Vector2 randomPosition = raylib::Vector2(getRandomInt(WORKING_FRAME_COORDX, WORKING_FRAME_COORDX + WORKING_FRAME_WIDTH), getRandomInt(WORKING_FRAME_COORDY, WORKING_FRAME_COORDY + WORKING_FRAME_HEIGHT));
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
    freeFlag = true;
}

void ds_viz::pages::GraphVisualizer::InsertNewEdge(int u, int v, int c) {
    for(auto &edge : edges){
        if(std::make_pair(u, v) == edge.first || std::make_pair(v, u) == edge.first){
            // Notification for edge addition failure (duplicate edge)
            notification = std::make_unique<raywtk::Notification>(
                "There's already an edge connect between " + std::to_string(u) + " and " + std::to_string(v) + ".",
                raywtk::NotificationType::ERROR,
                NOTIFICATION_COORDX,
                NOTIFICATION_COORDY
            );
            return;
        }
    }

    PushToUndoStack(); // Save the current state before modifying

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
    animationTimer = 0.0;
    currentAnimationStep = 0;
    totalSteps = animationSteps.size();
    animationRunning = true;


    // Union-Find logic
    std::function<int(int)> findPar = [&](int x) -> int {
        return x == par[x] ? x : par[x] = findPar(par[x]);
    };

    // Check if the graph is disconnected
    int mstEdges = 0;
    for (const auto &edge : edges) {
        int u = edge.first.first;
        int v = edge.first.second;

        if (findPar(u) != findPar(v)) {
            mstEdges++;
            par[findPar(u)] = findPar(v);
        }
    }

    if (mstEdges < nodes.size() - 1) {
        // Graph is disconnected
        kruskalFlag = false;
        notification = std::make_unique<raywtk::Notification>(
            "Kruskal's algorithm failed: Graph is disconnected.",
            raywtk::NotificationType::ERROR,
            NOTIFICATION_COORDX,
            NOTIFICATION_COORDY
        );
        freeFlag = true;
    } else{
        animationSteps.clear();
        for(size_t i = 0; i < nodes.size(); i++) par[i] = i, sz[i] = 1;
        kruskalFlag = true;
        int current_edge_index;               // Index of the edge being processed
        std::set<int> edges_in_mst;           // Indices of edges in the MST
        std::set<int> nodes_in_mst;           // Indices of nodes in the MST
        int pseudo_code_line;                 // Index of the pseudo-code line to highlight
        for(size_t i = 0; i < edges.size(); i++){
            current_edge_index = i;
            int u = edges[i].first.first;
            int v = edges[i].first.second;
            int w = edges[i].second;
            pseudo_code_line = 1; // Highlight the line for considering edge
            animationSteps.push_back({current_edge_index, edges_in_mst, nodes_in_mst, pseudo_code_line});

            if (findPar(u) != findPar(v)) {
                edges_in_mst.insert(i);
                nodes_in_mst.insert(u);
                nodes_in_mst.insert(v);
                par[findPar(u)] = findPar(v);
                sz[findPar(v)] += sz[findPar(u)];
                pseudo_code_line = 2; // Highlight the line for adding edge to MST
                animationSteps.push_back({current_edge_index, edges_in_mst, nodes_in_mst, pseudo_code_line});
            } else {
                pseudo_code_line = 3; // Highlight the line for ignoring edge
                animationSteps.push_back({current_edge_index, edges_in_mst, nodes_in_mst, pseudo_code_line});
            }
        }        
        animationSteps.push_back({-1, edges_in_mst, nodes_in_mst, 4}); // Highlight the line for stopping when MST is complete  
    }
    totalSteps = animationSteps.size() - 1;
}

void ds_viz::pages::GraphVisualizer::RenderAnimationStep(const AnimationStep& step, const std::vector<std::pair<std::pair<int, int>, int>>& sortedEdges) {
    // Render edges
    for (size_t i = 0; i < sortedEdges.size(); ++i) {
        raylib::Vector2 startPos, endPos;
        for (auto& node : nodes) {
            if (node->value == sortedEdges[i].first.first) startPos = node->position;
            if (node->value == sortedEdges[i].first.second) endPos = node->position;
        }

        raylib::Color edgeColor = WHITE; // Default color
        if (step.edges_in_mst.find(i) != step.edges_in_mst.end()) {
            edgeColor = GREEN; // "In MST" color
        } else if (i == step.current_edge_index) {
            if(step.pseudo_code_line == 3){
                edgeColor = RED;
            }
            else{
                edgeColor = YELLOW; // "Considering" color
            }
        }

        startPos.DrawLine(endPos, 2.5f, edgeColor);
        raylib::Vector2 posText = {(startPos.x + endPos.x) / 2, (startPos.y + endPos.y) / 2};
        std::string weight = std::to_string(sortedEdges[i].second);
        raylib::DrawText(weight.c_str(), posText.x, posText.y, 20, edgeColor);
    }

    // Render nodes
    for (auto& node : nodes) {
        raylib::Color nodeColor = BLUE; // Default color
        if (step.nodes_in_mst.find(node->value) != step.nodes_in_mst.end()) {
            nodeColor = GREEN; // "In MST" color
        } else if (node->value == sortedEdges[step.current_edge_index].first.first ||
                   node->value == sortedEdges[step.current_edge_index].first.second) {
            nodeColor = YELLOW; // "Considering" color
        }

        node->color = nodeColor;
        node->Render();
    }

    // Highlight pseudo-code line
    for (size_t i = 0; i < kruskalPseudoCode.size(); ++i) {
        if (i == step.pseudo_code_line) {
            pseudoCodeDisplay->SetLineState(i, raywtk::PseudoCodeDisplay::LineState::HIGHLIGHTED);
        } else {
            pseudoCodeDisplay->SetLineState(i, raywtk::PseudoCodeDisplay::LineState::DEFAULT);
        }
    }
}

void ds_viz::pages::GraphVisualizer::DeleteNode(int node) {
    PushToUndoStack(); // Save the current state before modifying

    bool found = false;
    for (auto &nodes : nodes) {
        if (nodes->value == node) found = true;
    }
    if (found) {
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
    freeFlag = true;
}

void ds_viz::pages::GraphVisualizer::DeleteEdge(int u, int v) {
    PushToUndoStack(); // Save the current state before modifying

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
    freeFlag = true;
}

void ds_viz::pages::GraphVisualizer::ClearGraph() {
    PushToUndoStack(); // Save the current state before modifying

    // Clear all nodes and edges
    nodes.clear();
    edges.clear();

    // Notification for successful clearing
    notification = std::make_unique<raywtk::Notification>(
        "Graph cleared successfully.",
        raywtk::NotificationType::SUCCESS,
        NOTIFICATION_COORDX,
        NOTIFICATION_COORDY
    );

    freeFlag = true;
}

void ds_viz::pages::GraphVisualizer::PushToUndoStack() {
    // Create a snapshot of the current state
    std::vector<std::unique_ptr<raywtk::GraphNode>> nodeSnapshot;
    for (const auto& node : nodes) {
        nodeSnapshot.push_back(std::make_unique<raywtk::GraphNode>(*node));
    }

    undoStack.push({std::move(nodeSnapshot), edges});
    redoStack = {}; // Clear redoStack
}

void ds_viz::pages::GraphVisualizer::Undo() {
    if (!undoStack.empty()) {
        // Push the current state to redoStack
        std::vector<std::unique_ptr<raywtk::GraphNode>> nodeSnapshot;
        for (const auto& node : nodes) {
            nodeSnapshot.push_back(std::make_unique<raywtk::GraphNode>(*node));
        }
        redoStack.push({std::move(nodeSnapshot), edges});

        // Restore the last state from undoStack
        auto lastState = std::move(undoStack.top());
        undoStack.pop();
        nodes = std::move(lastState.first);
        edges = std::move(lastState.second);
    }
}

void ds_viz::pages::GraphVisualizer::Redo() {
    if (!redoStack.empty()) {
        // Push the current state to undoStack
        std::vector<std::unique_ptr<raywtk::GraphNode>> nodeSnapshot;
        for (const auto& node : nodes) {
            nodeSnapshot.push_back(std::make_unique<raywtk::GraphNode>(*node));
        }
        undoStack.push({std::move(nodeSnapshot), edges});

        // Restore the last state from redoStack
        auto lastState = std::move(redoStack.top());
        redoStack.pop();
        nodes = std::move(lastState.first);
        edges = std::move(lastState.second);
    }
}

void ds_viz::pages::GraphVisualizer::RenderProgressBar() {
    // Draw the progress bar background
    raylib::Rectangle progressBarBackground = {PROGRESS_BAR_POSX, PROGRESS_BAR_POSY, PROGRESS_BAR_LENGTH, PROGRESS_BAR_HEIGHT};
    DrawRectangleRec(progressBarBackground, raylib::Color::LightGray());

    // Calculate the progress based on the current step
    float progressWidth = (currentAnimationStep / (float)totalSteps) * PROGRESS_BAR_LENGTH;
    raylib::Rectangle progressBar = {PROGRESS_BAR_POSX, PROGRESS_BAR_POSY, progressWidth, PROGRESS_BAR_HEIGHT};
    DrawRectangleRec(progressBar, raylib::Color::Green());
}

void ds_viz::pages::GraphVisualizer::Update(float dt)
{
    // Display Frame update
    workingFrame->Update(dt);

    // Update the toggle button
    toggleButton->Update(dt);

    if (waitingForFile && IsFileDropped()) {
        std::vector<std::string> droppedFiles = raylib::LoadDroppedFiles();
        std::string filePath = droppedFiles[0];
        LoadGraphFromFile(filePath);
    }

    // Update operator buttons only if they are visible
    if (showOperatorButtons) {
        loadFileButton->Update(dt);
        initializeGraphButton->Update(dt);
        insertNodeButton->Update(dt);
        insertEdgeButton->Update(dt);
        deleteNodeButton->Update(dt);
        deleteEdgeButton->Update(dt);
        KruskalButton->Update(dt);
        undoButton->Update(dt);
        redoButton->Update(dt);
        clearGraphButton->Update(dt);
        adjustSpeedButton->Update(dt);
    }

    prevStepButton->Update(dt);
    nextStepButton->Update(dt);
    continueButton->Update(dt);

    // Input box initialize graph button update
    if (inputInitializeGraphFlag) {
        inputBoxInitializeGraph->Update(dt);
        if (!inputBoxInitializeGraph->processing) {
            int numNodes = inputBoxInitializeGraph->values[0];
            if (numNodes > 0) {
                InitializeGraph(numNodes);
            } else {
                notification = std::make_unique<raywtk::Notification>(
                    "Invalid number of nodes. Please enter a positive integer.",
                    raywtk::NotificationType::ERROR,
                    NOTIFICATION_COORDX,
                    NOTIFICATION_COORDY
                );
            }
            inputInitializeGraphFlag = false;
            inputBoxInitializeGraph->Reset();
            freeFlag = true;
        }
    }

    // Input box insert edge button update
    if (inputInsertEdgeButtonFlag) {
        inputBoxInsertEdge->Update(dt);
        if (!inputBoxInsertEdge->processing) {
            auto values = inputBoxInsertEdge->values;
            InsertNewEdge(values[0], values[1], values[2]);
            inputInsertEdgeButtonFlag = false;
            inputBoxInsertEdge->Reset();
            freeFlag = true;
        }
    }

    // Input box delete node button update
    if (inputDeleteNodeButtonFlag) {
        inputBoxDeleteNode->Update(dt);
        if (!inputBoxDeleteNode->processing) {
            int nodeToDelete = inputBoxDeleteNode->values[0];
            DeleteNode(nodeToDelete);
            inputDeleteNodeButtonFlag = false;
            inputBoxDeleteNode->Reset();
            freeFlag = true;
        }
    }

    // Input box delete edge button update
    if (inputDeleteEdgeButtonFlag) {
        inputBoxDeleteEdge->Update(dt);
        if (!inputBoxDeleteEdge->processing) {
            auto values = inputBoxDeleteEdge->values;
            DeleteEdge(values[0], values[1]);
            inputDeleteEdgeButtonFlag = false;
            inputBoxDeleteEdge->Reset();
            freeFlag = true;
        }
    }

    // Input box adjust speed button update
    if (inputAdjustSpeedFlag) {
        inputBoxAdjustSpeed->Update(dt);
        if (!inputBoxAdjustSpeed->processing) {
            float stepsPerSecond = inputBoxAdjustSpeed->values[0];
            if (stepsPerSecond > 0) {
                animationStepDuration = 1.0f / stepsPerSecond; // Update animation step duration
                notification = std::make_unique<raywtk::Notification>(
                    "Animation speed updated to " + std::to_string(stepsPerSecond) + " steps per second.",
                    raywtk::NotificationType::SUCCESS,
                    NOTIFICATION_COORDX,
                    NOTIFICATION_COORDY
                );
            } else {
                notification = std::make_unique<raywtk::Notification>(
                    "Invalid speed. Please enter a positive number.",
                    raywtk::NotificationType::ERROR,
                    NOTIFICATION_COORDX,
                    NOTIFICATION_COORDY
                );
            }
            inputAdjustSpeedFlag = false;
            inputBoxAdjustSpeed->Reset();
            freeFlag = true;
        }
    }

    // Update the toggle button
    pseudoCodeToggleButton->Update(dt);

    // Update the pseudo-code box only if it is visible
    if (showPseudoCodeBox) {
        pseudoCodeDisplay->Update(dt);
    }

    // Animation step update
    if (kruskalFlag && currentAnimationStep < animationSteps.size() - 1) {
        if(animationRunning){
            animationTimer += dt;
            if (animationTimer >= animationStepDuration) {
                animationTimer = 0.0f;
                ++currentAnimationStep;
            }
        }
    } else{
        if(kruskalFlag){
            notification = std::make_unique<raywtk::Notification>(
                "Press enter to continue",
                raywtk::NotificationType::INFO,
                NOTIFICATION_COORDX,
                NOTIFICATION_COORDY
            );
            if(IsKeyPressed(KEY_ENTER)){
                kruskalFlag = false;
                animationSteps.clear();
                for(auto &node : nodes){
                    node->color = BLUE;
                }
                freeFlag = true;
            }
        }
    }

    // Update the Main Menu button
    mainMenuButton->Update(dt);

    // Update all nodes
    for (auto &node : nodes) {
        //node->Update(dt);
        node->Update(dt, nodes, raylib::Rectangle(WORKING_FRAME_COORDX, WORKING_FRAME_COORDY, WORKING_FRAME_WIDTH, WORKING_FRAME_HEIGHT));
    }
}

void ds_viz::pages::GraphVisualizer::Render() 
{
    title.Draw(200, 100);

    // display frame render
    workingFrame->Render();

    // notification frame render
    notificationFrame->Render();

    // toggle button render
    toggleButton->Render();

    if (showOperatorButtons) {
        loadFileButton->Render();
        initializeGraphButton->Render();
        insertNodeButton->Render();
        insertEdgeButton->Render();
        deleteNodeButton->Render();
        deleteEdgeButton->Render();
        KruskalButton->Render();
        undoButton->Render();
        redoButton->Render();
        clearGraphButton->Render();
        adjustSpeedButton->Render();
    }

    if(kruskalFlag){
        prevStepButton->Render();
        nextStepButton->Render();
        continueButton->Render();
        RenderProgressBar();
    }

    // Render the toggle button
    pseudoCodeToggleButton->Render();

    // Render the pseudo-code box only if it is visible
    if (showPseudoCodeBox) {
        pseudoCodeDisplay->Render();
    }
    
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

    // Input box initialize graph render
    inputBoxInitializeGraph->Render();

    // Input box insert new edge render
    inputBoxInsertEdge->Render();

    // Input box delete node render
    inputBoxDeleteNode->Render();

    // Input box delete edge render
    inputBoxDeleteEdge->Render();

    // Input box adjust speed render
    inputBoxAdjustSpeed->Render();

    // Notification render
    if(notification != nullptr){
        notification->Render();
    }

    // Animation step render
    if (currentAnimationStep < animationSteps.size()) {
        RenderAnimationStep(animationSteps[currentAnimationStep], edges);
    }

    // Render the Main Menu button
    mainMenuButton->Render();

    // vector nodes render
    for(auto &node : nodes){
        node->Render();
    }
}