#pragma once

#include <memory>
#include <vector>
#include <stack>
#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/controls/button.hpp"
#include "widget_toolkit/graph_widget/node.hpp"
#include "main_app/themes/dark_simple/text_button.hpp"
#include "./page.hpp"


namespace ds_viz::pages 
{
    class LinkedListPage : public Page 
    {
        // Font and title of page
        std::unique_ptr<raylib::Font> font;
        raylib::Text title;
        
        std::unique_ptr<raywtk::NodeWidget> head = nullptr; // Head node
        std::shared_ptr<raylib::Font> sharedFont = std::make_shared<raylib::Font>(LoadFont("./ttf/InterDisplay-ExtraBold.ttf")); // Node font
        float headX = 80; // Default X-coordinate of head
        float headY = 400; // Default Y-coordinate of head
        float spacing = 100; // Default spacing between nodes
        int size = 0; // Size of Linked List
        
        // For undo and redo functions
        std::stack<std::vector<int>> UndoStack; 
        std::stack<std::vector<int>> RedoStack;

        // List of buttons
        std::unique_ptr<raywtk::Button> repositionButton;
        std::unique_ptr<raywtk::Button> undoButton;
        std::unique_ptr<raywtk::Button> redoButton;
        std::unique_ptr<raywtk::Button> returnButton;

        std::unique_ptr<raywtk::Button> insertButton;
        std::vector<std::unique_ptr<raywtk::Button>> insertDropdownButtons;
        std::vector<std::string> insertOptions = { "At Head", "At Tail", "At Index" };
        bool insertDropdownOpen = false;

        std::unique_ptr<raywtk::Button> deleteButton;
        std::vector<std::unique_ptr<raywtk::Button>> deleteDropdownButtons;
        std::vector<std::string> deleteOptions = { "At Head", "At Tail", "At Index" };
        bool deleteDropdownOpen = false;

        std::unique_ptr<raywtk::Button> searchButton;
        std::vector<std::unique_ptr<raywtk::Button>> searchDropdownButtons;
        std::vector<std::string> searchOptions = { "By Value", "By Index" };
        bool searchDropdownOpen = false;

        std::unique_ptr<raywtk::Button> otherButton;
        std::vector<std::unique_ptr<raywtk::Button>> otherDropdownButtons;
        std::vector<std::string> otherOptions = { "Random", "Clear", "Load file" };
        bool otherDropdownOpen = false;
        
        // For image buttons texture
        raylib::Texture undoButtonTex;
        raylib::Texture redoButtonTex;
        raylib::Texture returnButtonTex;
        
        // flags to show input bars
        bool showInsertAtHead = false;
        std::string inputInsertAtHead;

        bool showInsertAtTail = false;
        std::string inputInsertAtTail;

        bool showInsertAtIndexInput  = false;
        std::string inputValue; // For value inputs
        std::string inputIndex; // For index inputs
        bool isEnteringValue = true;

        bool showRandomInput = false;  
        std::string inputRandom;

        bool showDeleteAtIndexInput = false;
        std::string inputDeleteIndex;

        bool showSearchInput = false;
        bool searchByValue = true; // Toggle between value and index search
        std::string searchByValInput;
        std::string searchByIndInput;

        float animatingTimer = 0.0f; // Timer for animations

        // Insert Animation
        bool animatingInsert = false;
        raywtk::NodeWidget* insertCurrent = nullptr;
        int insertIndex = -1;
        int insertValue = -1000;
        int currentInsertIndex = 0;
        int insertState = 0;

        // Delete Animation
        bool animatingDelete = false;
        raywtk::NodeWidget* deleteCurrent = nullptr;
        raywtk::NodeWidget* ToDelPrev = nullptr;
        raywtk::NodeWidget* deleteShift = nullptr;
        int deleteIndex = -1;
        int currentdeleteIndex = 0;
        int deleteState = 0;

        // Search Animation
        bool animatingSearch = false;
        raywtk::NodeWidget* searchCurrent = nullptr;
        int searchTarget = -1000;
        int searchIndex = -1;
        int currentSearchIndex = 0;
        int searchState = 0;

        // Speed bar
        float speedBarX = 100;  
        float speedBarY = 50;
        float speedBarWidth = 300;
        float speedBarHeight = 10;
        float speedKnobRadius = 8;

        float speedMin = 0.25f;  // Minimum speed factor
        float speedMax = 2.0f;  // Maximum speed factor
        float speedKnobX = speedBarX;  // Start at the lowest speed
        bool isDraggingSpeedKnob = false;

        float animationSpeed = 1.0f;  // Default speed (1.0x)
        std::vector<float> speedSteps = {0.25f, 0.5f, 1.0f, 1.5f, 2.0f};

        // Messages and notifications
        std::string errorMessage = "";
        float errorTimer = 0.0f;
        std::string DropMessage = "";
        float DropTimer = 0.0f;
        std::string FindMessage = "";
        float FindTimer = 0.0f;


    public:

    LinkedListPage(); 
    ~LinkedListPage();

    void OnRandomButtonClick(int numNodes);
    void InsertRandom(int value);
    void OnLoadFileButtonClick();
    void OnClearButtonClick();
    void ResetColor();
    void RepositionNodes();
    void GetListState(std::vector<int> &ListState);
    void Append();
    void OnUndoButtonClick();
    void OnRedoButtonClick();
    void OnReturnButtonClick();
    void InsertAtHead(int value);
    void InsertAtTail(int value);
    void InsertAtIndex(int value, int index); 
    void DeleteAtHead();
    void DeleteAtTail();
    void DeleteAtIndex(int index);
    void SearchByValue(int value);
    void SearchByIndex(int index);
    void AnimateInsert(float dt);
    void AnimateDelete(float dt);
    void AnimateSearch(float dt);
    void CreateNotification(std::string &Message);
    void DrawInputBox(int X, int Y, std::string &input);
    void DrawInputBox2(int X, int Y, std::string &input);
    void DrawSpeedBar();
    void Update(float dt) override;
    void Render() override;
    };
}

